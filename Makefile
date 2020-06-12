KERNEL_PATH ?= /lib/modules/$(shell uname -r)/build

MODULE_NAME  = op_emu

$(MODULE_NAME)-objs := trap_hook.o opemu.o ssse3.o sse3.o opemu_math.o sse42.o sse41.o fpins.o libudis86/udis86.o libudis86/syn.o libudis86/decode.o libudis86/itab.o libudis86/syn-intel.o libudis86/syn-att.o


obj-m += $(MODULE_NAME).o

#CC=clang
KBUILD_CFLAGS += -DDEBUG -g -O2 -march=native -mtune=native -mmmx -msse -msse2

export KBUILD_CFLAGS

all:
	make -C $(KERNEL_PATH) M=$(PWD) modules

clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
