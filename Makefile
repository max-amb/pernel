export GNU_EFI := $(nix eval --raw nixpkgs#gnu-efi)
export CC := gcc
export CFLAGS := export CFLAGS := -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -I ${gnu_efi}/include/efi -I ${gnu_efi}/include/efi/x86_64 -DEFI_FUNCTION_WRAPPER

kernel: kernel.c
	$(CC) -o kernel kernel.c $(CFLAGS)