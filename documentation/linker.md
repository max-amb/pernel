# Explaining the linker
## Purpose
* The linker specifies the format and the layout of the final executable binary
    * This allows for the bootloader to run our kernel

## Code 
* `ENTRY(efi_main)` -> Tells the linker (ld) to mark the entry point of the program as the function `efi_main` 
    * We must use the entry point `efi_main` because it is what the `gnu-efi` library expects

* `OUTPUT_FORMAT(...)` -> Specifies the output format of the binary output
    * We will probably use `elf64-x86-64` or `elf32-i386`

* `STARTUP(crt0-efi-x86_64.o)` -> Specifies the first file to be linked, this file provides initialisation routines for example
    * the `crt0-efi-x86_64.o` is provided by `gnu-efi` and it will initialise for us and call `efi_main`

* `INPUT(f1.o f2.1, ...)` -> Specifies the input files (could be done on command line)

* `SEARCH_DIR(directory)` -> Specifies paths to search for libraries

* `OUTPUT(kernel.efi)` -> Specifies the ouptut

* `MEMORY(...)` -> Characteristics of different sections of memory (like `ROM`)
    * Probably willn't use

* `SECTIONS(...)` -> Specifies the layout of our output binary
    * Needs to conform with `PE32+`
    * Can use `KEEP` to ensure some stuff that would usually get pruned stays in

* `SYMBOLS(...)` -> Used for memory tracking to see where stuff is and can be accessed in C

# References
* [The os dev wiki](https://wiki.osdev.org/Linker_Scripts)
