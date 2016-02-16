ARCH            = x86_64

OBJS            = main.o syscalls.o select.o poll.o kernel.o
TARGET          = BOOTX64.efi

TOOLCHAIN       = $(HOME)/x86_64-elf
EFIINC          = $(TOOLCHAIN)/include/efi
EFIINCS         = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
EFILIB          = $(TOOLCHAIN)/lib
EFI_CRT_OBJS    = $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS         = $(EFILIB)/elf_$(ARCH)_efi.lds
NEWLIBINC       = $(TOOLCHAIN)/x86_64-elf/include
NEWLIBLIB       = $(TOOLCHAIN)/x86_64-elf/lib
# Stub.
POLLINC         = $(CURDIR)/include

CFLAGS          = -fPIC $(EFIINCS) -I$(NEWLIBINC) -I$(POLLINC) -fno-stack-protector \
		  -fshort-wchar -mno-red-zone -Wall -O0
ifeq ($(ARCH),x86_64)
  CFLAGS += -DEFI_FUNCTION_WRAPPER
endif

LDFLAGS         = -nostdlib -znocombreloc -T $(EFI_LDS) -shared \
		  -L $(EFILIB) $(EFI_CRT_OBJS) -L $(NEWLIBLIB)

all: $(TARGET)
	# Make a FAT image.
	dd if=/dev/zero of=fat.img bs=1k count=1440
	mformat -i fat.img -f 1440 ::
	mmd -i fat.img ::/EFI
	mmd -i fat.img ::/EFI/BOOT
	mcopy -i fat.img $(TARGET) ::/EFI/BOOT

fetch:
	bash fetch.sh

BOOTX64.so:
	x86_64-elf-gcc $(CFLAGS) -c -o main.o main.c
	x86_64-elf-gcc $(CFLAGS) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -Wall -Wextra
	x86_64-elf-gcc $(CFLAGS) -c -o select.o select.c
	x86_64-elf-gcc $(CFLAGS) -c -o poll.o poll.c
	x86_64-elf-gcc $(CFLAGS) -c syscalls.c -o syscalls.o -std=gnu99 -ffreestanding -Wall -Wextra

	x86_64-elf-ld $(LDFLAGS) $(OBJS) -Bstatic -lc -Bsymbolic -lefi -lgnuefi -o $@

%.efi: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--target=efi-app-$(ARCH) $^ $@

run:
	qemu-system-x86_64 -L OVMF -bios OVMF.fd -usb -usbdevice disk::fat.img

clean:
	rm *.img *.o *.efi *.so
