export GNU_EFI := $(shell nix eval --raw nixpkgs#gnu-efi)
export CC := gcc
export CFLAGS := -fno-stack-protector -fpic -fshort-wchar -ffreestanding -maccumulate-outgoing-args -I $(GNU_EFI)/include/efi/ -I $(GNU_EFI)/include/efi/x86_64

kernel: kernel.c
	$(CC) -o kernel.o kernel.c $(CFLAGS)
