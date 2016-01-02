#include <stdio.h>

#include <efi.h>
#include <efilib.h>

void kernel_main() {
  Print(L"kernel_main called\r\n");

  Print(L"printf pointer: %x\r\n", &printf);
  printf("hello from printf\r\n");

  Print(L"after printf\r\n");
}
