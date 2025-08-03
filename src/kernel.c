/*
Tutorials that you don't wanna miss:
    1. https://wiki.osdev.org/Bare_Bones
    2. https://wiki.osdev.org/UEFI_App_Bare_Bones
    3. https://wiki.osdev.org/UEFI
    4. https://wiki.osdev.org/GNU-EFI
*/

#include <efi.h>
#include <efilib.h>

const CHAR16 *Ascii[] = {
    L"                                                     .--.\r\n",
    L" ____                              ___             .'_\\/_'.\r\n",
    L"/\\  _`\\                           /\\_ \\            '. /\\ .'\r\n",
    L"\\ \\ \\L\\ \\ __   _ __    ___      __\\//\\ \\             \"\||\"\r\n",
    L" \\ \\ ,__/'__`\\/\\`'__\\/' _ `\\  /'__`\\\\ \\ \\             || /\\ \r\n",
    L"  \\ \\ \\/\\  __/\\ \\ \\/ /\\ \\/\\ \\/\\  __/ \\_\\ \\_        /\\ ||//\\)\r\n",
    L"   \\ \\_\\ \\____\\\\ \\_\\ \\ \\_\\ \\_\\ \\____\\/\\____\\      (/\\\\||/\r\n",
    L"    \\/_/\\/____/ \\/_/  \\/_/\\/_/\\/____/\\/____/   ______\\||/_______\r\n"
};


/* 
EFI_STATUS: signifies that efi_main will return a UEFI status code.
EFIAPI: a macro that expands to the correct calling convention for
        UEFI on the target architecture.
*/
EFI_STATUS
EFIAPI 
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
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
    /* Call ClearScreen() to clean the existing shell content */
    uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
    /* Call SetCursorPosition to reset the cursor to top-left */
    uefi_call_wrapper(ST->ConOut->SetCursorPosition, 3,
                      ST->ConOut, 0, 0);

    /*
    Call OutputString() with two parameters: the ConOut (ConsoleOut)
    stream and the buffer. The "L" prefix signifies wide literal.
    */
    Status = uefi_call_wrapper(ST->ConOut->OutputString, 2, 
        ST->ConOut, L"Hello PAIR! ");
    if (EFI_ERROR(Status)) return Status;  /* Check for error */
    
    /* Call the procedure Stall() at BootServices to sleep for 1s. */
    uefi_call_wrapper(BS->Stall, 1, 1000000);

    for (UINTN i = 0; i < sizeof(Ascii)/sizeof(Ascii[0]); i++) {
        uefi_call_wrapper(BS->Stall, 1, 100000);
        Status = uefi_call_wrapper(ST->ConOut->OutputString, 2, 
            ST->ConOut, Ascii[i]);
        if (EFI_ERROR(Status)) return Status;  /* Check for error */
    }

    /****
    Next, we implement a mechanism where the system won't quit unless
    the user explicitly key in "q".
    ****/

    /* Clear the input stream */
    Status = uefi_call_wrapper(ST->ConIn->Reset, 2, ST->ConIn, FALSE);
    if (EFI_ERROR(Status)) return Status; 

    /*
    Define a variable that stores the input key value:
        TYPE EFI_INPUT_KEY {
            UINT16: ScanCode
            CHAR16: UnicodeChar
        }
     */
    EFI_INPUT_KEY Key; 
    CHAR16 Line[64];
    UINTN InputIdx;

    Status = uefi_call_wrapper(ST->ConOut->OutputString, 2, 
        ST->ConOut, L"You may type :) Enter to submit, q to quit:\r\n");
    if (EFI_ERROR(Status)) return Status;  /* Check for error */
    
    /* The 1st for-loop keeps the shell running continuously */
    for (;;){
        InputIdx = 0;
        Line[0] = L'\0';  /* Prevent null input */
        /* The 2nd for-loop ensures continuous input */
        for (;;) {
            /* Call ReadKeyStroke() to read in a key */
            Status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2,
                ST->ConIn, &Key);
            /* If there's no key yet, skip */
            if (Status == EFI_NOT_READY) continue;
            /* If there's an error, return the error */
            if (EFI_ERROR(Status)) return Status;
            /* If there's an ENTER, break the input loop */
            if (Key.UnicodeChar == L'\r') {
                Line[InputIdx] = L'\0';  /* NULL termination */
                break;
            }
            
            /* 
            Store the key if space permits;
            save the last index for NULL termination.
            */
            if (InputIdx + 1 < 63) {
                Line[InputIdx++] = Key.UnicodeChar;
            }
        }
        /* If the user types 'q', closes the shell */
        if (InputIdx == 1 && Line[0] == L'q') break;

        /* Print() achieves the same outputing effect, but slower */
        Status = Print(L"You typed: %s\r\n", Line);
        if (EFI_ERROR(Status)) return Status;  /* Check for error */
    }

    return EFI_SUCCESS;  /* The process has finished successfully. */
}
