#include <stdio.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <poll.h>

#include <efi.h>
#include <efilib.h>

void kernel_main() {
  Print(L"kernel_main called\r\n");

  Print(L"printf pointer: %x\r\n", &printf);
  printf("hello from printf\r\n");
  Print(L"after printf\r\n");

  Print(L"select pointer: %x\r\n", &select);
  int nfds = 1;
  fd_set writefds;
  FD_ZERO(&writefds);
  FD_SET(2, &writefds);
  struct timeval timeout = {.tv_sec = 0, .tv_usec = 0};
  int res = select(nfds, NULL, &writefds, NULL, &timeout);
  if (res == -1)
    printf("ERROR: select()\r\n");
  else if (res)
    printf("Data is available now.\r\n");
  else
    printf("No data available.\r\n");
  Print(L"after select\r\n");

  Print(L"poll pointer: %x\r\n", &poll);
  struct pollfd fds = {.fd = 4, .events = 5, .revents = 6};
  nfds_t nfds_poll = 42;
  int timeout_poll = 7;
  poll(&fds, nfds_poll, timeout_poll);
  Print(L"after poll\r\n");
}
