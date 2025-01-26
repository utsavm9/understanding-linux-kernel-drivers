
ifneq ($(KERNELRELEASE),)

# If KERNELRELEASE is defined, this Makefile has been invoked from kernel build system.
# Declare conventions needed by kbuild.

# Generates chard.ko from chard.o
# and chard.o is generated from char_device.c
# obj-y would denote built-in object goals whereas obj-m are modules.
obj-m += chard.o
# This line can be omitted if the module has the same name as the .c file.
chard-objs := char_device.o

# Leave behind corresponding intermediate files.
CFLAGS_chard.o := -DDEBUG -save-temps=obj

else
# Context where this Makefile is getting directly invoked by command line.


KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

help:
	$(MAKE) -C $(KERNELDIR) help


endif # KERNELRELEASE
