#include <efi.h>
#include <efilib.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// 'libc.a' provided by newlib doesn't contain 'select' (see 'ar -t libc.a'), so
// make a stub.
int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout)
{
  Print(L"select stub called\r\n");
  return 0;
}
