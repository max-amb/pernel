# pernel
a simple OS kernel, made at PAIR camp 2025

# the flow

### core elements
* `Makefile`
* `Linker.ld`
* `boot.s`
* `something to actually write to screen (e.g. "hello world")

### process
`Makefile` -> compiles all other files as objects --> `linker.ld` --> tells entry point of system --> linker program links files together --> make command makes a `.iso` (binary representation of objects)

### integration
[Our Code] <--> [UEFI] <--> [QEMU (simulated hardware)]

* UEFI runs our code which is stored in a `bootx.esi` file
* The library we use in the C code is `gnuesi` to interact with UEFI.