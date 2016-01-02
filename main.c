#include <efi.h>
#include <efilib.h>

void kernel_main();

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    Print(L"efi_main called\r\n");

    kernel_main();

    for (;;); // XXX: loop forever

    return EFI_SUCCESS;
}
