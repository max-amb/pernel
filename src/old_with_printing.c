#include <efi.h>
#include <efilib.h>

const char ascii[] = [
    "                                                     .--.",
    " ____                              ___             .'_\/_'.",
    "/\  _`\                           /\_ \            '. /\ .'",
    "\ \ \L\ \ __   _ __    ___      __\//\ \             "||"",
    " \ \ ,__/'__`\/\`'__\/' _ `\  /'__`\\ \ \             || /\ ",
    "  \ \ \/\  __/\ \ \/ /\ \/\ \/\  __/ \_\ \_        /\ ||//\)",
    "   \ \_\ \____\\ \_\ \ \_\ \_\ \____\/\____\      (/\\||/",
    "    \/_/\/____/ \/_/  \/_/\/_/\/____/\/____/   ______\||/_______"
]

/* 
EFI_STATUS: signifies that efi_main will return a UEFI status code.
EFIAPI: a macro that expands to the correct calling convention for
        UEFI on the target architecture.
*/
EFI_STATUS
EFIAPI 
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) 
{
    EFI_STATUS Status;

    /* 
    System Table is a pointer to the "top-level directory"
    of UEFI services and core interfaces, including:
        - Console I/O
        - Services -> BootServices, RuntimeServices
        - Configuration Tables -> Array of GUID+pointer entries
        ...
    */
    InitializeLib(ImageHandle, SystemTable);
    /* 
    InitializeLib initializes the library globals like 
        - ST for SystemTable
        - BS for SystemTable -> BootServices
        - RT for SystemTable -> RuntimeServices
        ...
    */
    
    /*
    The uefi_call_wrapper ensures compatibility between the host 
    system's gcc and UEFI. It's a function from `libefi.a`:
        uefi_call_wrapper(func, numarg, ...)
    */
    Status = uefi_call_wrapper(ST->ConOut->OutputString, 2, 
        ST->ConOut, L"Hello PAIR! ");
    if (EFI_ERROR(Status)) return Status;  /* Check for error */
    /*
    Call OutputString() with two parameters: the ConOut (ConsoleOut)
    stream and the buffer. The "L" prefix signifies wide literal.
    */
    
    /*
    Call the procedure Stall() at BootServices to sleep for 5s.
    */
    uefi_call_wrapper(BS->Stall, 1, 1000000);

    for (int i = 0; i < sizeof(ascii)/sizeof(ascii[0]); i++) {
        uefi_call_wrapper(BS->Stall, 1, 100000);
        Status = uefi_call_wrapper(ST->ConOut->OutputString, 2, 
            ST->ConOut, ascii[i] + "\n");
        if (EFI_ERROR(Status)) return Status;  /* Check for error */
    }
        
    return EFI_SUCCESS;  // The process has finished successfully.
}
