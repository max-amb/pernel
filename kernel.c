#include <efilib.h>
#include <efi.h>


/* 
EFI_STATUS: signifies that efi_main will return a UEFI status code.
EFIAPI: a macro that expands to the correct calling convention for
        UEFI on the target architecture.
*/
EFI_STATUS
EFI_API 
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) 
{
    EFI_STATUS Status;
    
    /* 
    System Table is a pointer to the "top-level directory"
    of UEFI services and core interfaces, including:
        - Console I/O
        - Services -> BootServices, RuntimeServices
        - Configuration Tables -> Array of GUID+pointer entries
    */
    ST = SystemTable;

    /*
    The uefi_call_wrapper ensures compatibility between the host 
    system's gcc and UEFI. It's a function from `libefi.a`:
        uefi_call_wrapper(func, numarg, ...)
    */
    Status = uefi_call_wrapper(ST->ConOut->OutputString, 2, 
        ST->ConOut, L"Hello PAIR!\r\n");
    if (EFI_ERROR(Status)) return Status;  /* Check for error */
    /*
    Call OutputString() with two parameters: the ConOut (ConsoleOut)
    stream and the buffer. The "L" prefix signifies wide literal.
    */
    
    /*
    Call Stall() at BootServices to halt the program for 5 seconds.
    */
    Status = uefi_call_wrapper(ST->BootServices->Stall, 1, 5000000);
    if (EFI_ERROR(Status)) return Status;

    return Status;  // Status shall be EFI_SUCCESS here. 
}