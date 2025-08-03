# Sets variables which store where library files are
export GNU_EFI := $(shell nix eval --raw nixpkgs#gnu-efi)
export OVMF := $(shell nix eval --raw nixpkgs#OVMF.fd)

# Set compiler
export CC := gcc

# Set the flags so that gcc finds all of the libraries and compiles the code into a binary suitable for running on a uefi bootloader
export CFLAGS := -fno-stack-protector -fpic -fshort-wchar -ffreestanding -maccumulate-outgoing-args -I $(GNU_EFI)/include/efi/ -I $(GNU_EFI)/include/efi/x86_64

# Flags used for linking
export LD_FLAGS := $(GNU_EFI)/lib/crt0-efi-x86_64.o -nostdlib -znocombreloc -T $(GNU_EFI)/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L $(GNU_EFI)/lib/ -l:libgnuefi.a -l:libefi.a

# Flags used to configure objcopy
export OBJ_FLAGS := -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10

# For the qemu target to run, we need to run the kernel target
.PHONY: qemu kernel 

# Compiles the kernel
kernel: kernel.c
	$(CC) -c -o kernel.o kernel.c $(CFLAGS)
	ld kernel.o $(LD_FLAGS) -o kernel.so
	objcopy $(OBJ_FLAGS) kernel.so BOOTX64.EFI

# Uses the compiled kernel to run it in qemu
qemu: kernel kernel.c
	# Make the image
	dd if=/dev/zero of=uefi.img bs=512 count=93750
	# Partition the image
	parted uefi.img -s -a minimal mklabel gpt
	parted uefi.img -s -a minimal mkpart EFI FAT16 2048s 93716s
	parted uefi.img -s -a minimal toggle 1 boot
	# Making a tmp image to write to
	dd if=/dev/zero of=/tmp/part.img bs=512 count=91669
	mformat -i /tmp/part.img -h 32 -t 32 -n 64 -c 1
	# Copying the bootx.efi file over to tmp
	mmd -i /tmp/part.img ::/EFI
	mmd -i /tmp/part.img ::/EFI/BOOT
	mcopy -i /tmp/part.img BOOTX64.EFI ::/EFI/BOOT
	# Writing the tmp image to the uefi image
	dd if=/tmp/part.img of=uefi.img bs=512 count=91669 seek=2048 conv=notrunc
	# RUN!
	qemu-system-x86_64 -cpu qemu64 -bios $(OVMF)/FV/OVMF.fd -drive file=uefi.img,if=ide

# cleanup
clean:
	-rm kernel.o BOOTX64.EFI kernel.so uefi.img
