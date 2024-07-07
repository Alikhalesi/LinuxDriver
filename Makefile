CONFIG_MODULE_SIG=n
MODULE_NAME = usbdrv_module

SRC     := usbdrv.c

$(MODULE_NAME)-objs = $(SRC:.c=.o)

obj-m       := $(MODULE_NAME).o

KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)


EXTRA_CFLAGS := -I$(PWD)/proc

all:
	$(MAKE)  -C $(KERNEL_SRC) M=$(PWD) modules	
modules_install:	
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules_install
clean:
	$(MAKE)  -C $(KERNEL_SRC) M=$(PWD) clean
