# pernel
a simple OS kernel, made at PAIR camp 2025

## The Flow

### Core Elements
* **`Makefile`**
* **`linker.ld`**
* **`kernel.c`** (C entry point that writes “Hello, world!” to the screen)
    * The entry point to a UEFI application is a C file, no other assembly files needed.

### Process
1. **`Makefile`**
   - Defines how `make` compiles each `.S` and `.c` into `.o` object files.
   - Invokes the **linker** with **`linker.ld`** to lay out those objects in memory and set the entry point.
   - Packages the resulting `kernel.efi` into a bootable image (FAT-ESP).
2. **`linker.ld`**
   - Tells the linker where to place the code/data sections and which symbol is the entry point.
3. **Linker** (`i686-elf-ld`)
   - Takes all the `.o` files plus the script, resolves symbols, applies relocations, and produces the final EFI image (`kernel.efi`).
4. **Boot Image**
   - Creates a FAT image (`esp.img`) with `EFI/BOOT/BOOTX64.EFI` → done with QEMU with OVMF

##### Development Process
At the beginning, maybe pangea kinda time, we spent 10 minutes struggling with no c standard library, what the hell?
It turns out, we imported the libraries the wrong way round. Next, what a beautiful error, there was missing library?!
No, we missed the `-c` flag in our `Makefile` for approximately half an hour.
Then, we were on the home stretch, but the `InitializeLib` function was missing :(. How? I swore I included it?
Oh, a missing, underscore.

### Integration
```
[Our Code] <--> [UEFI] <--> [QEMU (simulated hardware)]
```

* UEFI runs our code which is stored in a `bootx.esi` file
* We use **`GNU-EFI`** library to call UEFI Boot Services from C.
  * UEFI Boot Services give rich APIs to do things like memory services without early bootloader codes.
