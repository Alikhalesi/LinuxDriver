CONFIG_MODULE_SIG=n
MODULE_NAME = usbdrv_module

SRC     := usbdrv.c proc/proc.c

$(MODULE_NAME)-objs = $(SRC:.c=.o)

obj-m       := $(MODULE_NAME).o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)


EXTRA_CFLAGS := -I$(PWD)/proc

all:
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) modules	
modules_install:	
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
clean:
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) clean
