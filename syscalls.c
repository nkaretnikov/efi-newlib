#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#undef errno
extern int errno;
#include <sys/time.h>
#include <stdio.h>

#include <efi.h>
#include <efilib.h>

// Close a file. Minimal implementation:
int close(int file) {
  Print(L"close called\r\n");
  return -1;
}

// A pointer to a list of environment variables and their values. For a minimal
// environment, this empty list is adequate:
char *__env[1] = { 0 };
char **environ = __env;

// Transfer control to a new process. Minimal implementation (for a system
// without processes):
int execve(char *name, char **argv, char **env) {
  Print(L"execve called\r\n");
  errno = ENOMEM;
  return -1;
}

// Create a new process. Minimal implementation (for a system without
// processes):
int fork(void) {
  Print(L"fork called\r\n");
  errno = EAGAIN;
  return -1;
}

// Status of an open file. For consistency with other minimal implementations in
// these examples, all files are regarded as character special devices. The
// sys/stat.h header file required is distributed in the include subdirectory
// for this C library.
int fstat(int file, struct stat *st) {
  Print(L"fstat called\r\n");
  return 0;
}
/*
int fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}
*/

// Process-ID; this is sometimes used to generate strings unlikely to conflict
// with other processes. Minimal implementation, for a system without processes:
int getpid(void) {
  Print(L"getpid called\r\n");
  return 1;
}

// Query whether output stream is a terminal. For consistency with the other
// minimal implementations, which only support output to stdout, this minimal
// implementation is suggested:
int isatty(int file) {
  Print(L"isatty called\r\n");
  return 1;
}

// Send a signal. Minimal implementation:
int kill(int pid, int sig) {
  Print(L"kill called\r\n");
  errno = EINVAL;
  return -1;
}

// Establish a new name for an existing file. Minimal implementation:
int link(char *old, char *new) {
  Print(L"link called\r\n");
  errno = EMLINK;
  return -1;
}

// Set position in a file. Minimal implementation:
int lseek(int file, int ptr, int dir) {
  Print(L"lseek called\r\n");
  return 0;
}

// Open a file. Minimal implementation:
/*
int open(const char *name, int flags, int mode) {
  Print(L"open called\r\n");
  return -1;
}
*/

// Read from a file. Minimal implementation:
int read(int file, char *ptr, int len) {
  Print(L"read called\r\n");
  return 0;
}

// Increase program data space. As malloc and related functions depend on this,
// it is useful to have a working implementation. The following suffices for a
// standalone system; it exploits the symbol _end automatically defined by the
// GNU linker.
caddr_t sbrk(int incr) {
// Crashes with this Print call.
//  Print(L"sbrk called\r\n");

  void* res = AllocatePool(incr);
  return (caddr_t) res;

  // Minimal:
  // return (caddr_t) 0;
}
/*
caddr_t sbrk(int incr) {
  extern char _end;		// Defined by the linker
  static char *heap_end;
  char *prev_heap_end;

  if (heap_end == 0) {
    heap_end = &_end;
  }
  prev_heap_end = heap_end;
  if (heap_end + incr > stack_ptr) {
    write (1, "Heap and stack collision\n", 25);
    abort ();
  }

  heap_end += incr;
  return (caddr_t) prev_heap_end;
}
*/

// Status of a file (by name). Minimal implementation:
/*
int stat(char *file, struct stat *st) {
  Print(L"stat called\r\n");
  st->st_mode = S_IFCHR;
  return 0;
}
*/

// Timing information for current process. Minimal implementation:
int times(struct tms *buf) {
  Print(L"times called\r\n");
  return -1;
}

// Remove a file's directory entry. Minimal implementation:
int unlink(char *name) {
  Print(L"unlink called\r\n");
  errno = ENOENT;
  return -1;
}

// Wait for a child process. Minimal implementation:
int wait(int *status) {
  Print(L"wait called\r\n");
  errno = ECHILD;
  return -1;
}

// Write to a file. libc subroutines will use this system routine for output to
// all files, including stdout—so if you need to generate any output, for
// example to a serial port for debugging, you should make your minimal write
// capable of doing this. The following minimal implementation is an incomplete
// example; it relies on a outbyte subroutine (not shown; typically, you must
// write this in assembler from examples provided by your hardware manufacturer)
// to actually perform the output.
int write(int file, char *ptr, int len) {
  Print(L"write called\r\n");

  int i;
  for (i = 0; i < len; i++)
    Print(L"%c", *ptr++);

  return 0;
}
/*
int write(int file, char *ptr, int len) {
  int todo;

  for (todo = 0; todo < len; todo++) {
    outbyte (*ptr++);
  }
  return len;
}
*/
