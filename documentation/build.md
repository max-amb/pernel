## GCC
```bash
export gnu_efi=$(nix eval --raw nixpkgs#gnu-efi)

gcc main.c                             \
      -c                                 \
      -fno-stack-protector               \
      -fpic                              \
      -fshort-wchar                      \
      -mno-red-zone                      \
      -I ${gnu_efi}/include/efi        \
      -I ${gnu_efi}/include/efi/x86_64 \
      -DEFI_FUNCTION_WRAPPER             \
      -o main.o

ld main.o                         \
     ${gnu_efi}/lib/crt0-efi-x86_64.o     \
     -nostdlib                      \
     -znocombreloc                  \
     -T ${gnu_efi}/lib/elf_x86_64_efi.lds \
     -shared                        \
     -Bsymbolic                     \
     -L ${gnu_efi}/lib/               \
     -l:libgnuefi.a                 \
     -l:libefi.a                    \
     -o main.so

objcopy -j .text                \
          -j .sdata               \
          -j .data                \
          -j .dynamic             \
          -j .dynsym              \
          -j .rel                 \
          -j .rela                \
          -j .reloc               \
          --target=efi-app-x86_64 \
          main.so                 \
          main.efi
```
