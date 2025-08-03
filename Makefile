export GNU_EFI := $(shell nix eval --raw nixpkgs#gnu-efi)
export CC := gcc
export CFLAGS := -fno-stack-protector -fpic -fshort-wchar -ffreestanding -maccumulate-outgoing-args -I $(GNU_EFI)/include/efi/ -I $(GNU_EFI)/include/efi/x86_64

export LD_FLAGS := $(GNU_EFI)/lib/crt0-efi-x86_64.o -nostdlib -znocombreloc -T $(GNU_EFI)/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L $(GNU_EFI)/lib/ -l:libgnuefi.a -l:libefi.a
# export LDFLAGS := -shared -Bsymbolic -Lgnu-efi-dir/x86_64/lib -Lgnu-efi-dir/x86_64/gnuefi -Tgnu-efi-dir/gnuefi/elf_x86_64_efi.lds gnu-efi-dir/x86_64/gnuefi/crt0-efi-x86_64.o main.o -o main.so -lgnuefi -lefi

kernel: kernel.c
	$(CC) -c -o kernel.o kernel.c $(CFLAGS)
	ld kernel.o $(LD_FLAGS) -o kernel.so
	# objcopy -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 kernel.so kernel.efi
