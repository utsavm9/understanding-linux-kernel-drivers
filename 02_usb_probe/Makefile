
ifneq ($(KERNELRELEASE),)


obj-m += usb_probe.o
CFLAGS_usb_probe.o := -DDEBUG -save-temps=obj


else


KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean


endif # KERNELRELEASE
