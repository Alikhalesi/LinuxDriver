CONFIG_MODULE_SIG=n
MODULE_NAME = usbdrv_module

SRC     := usbdrv.c proc/proc.c

$(MODULE_NAME)-objs = $(SRC:.c=.o)

obj-m       := $(MODULE_NAME).o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)


EXTRA_CFLAGS := -I$(PWD)/proc

all:
	make -C $(KERNELDIR) M=$(PWD) modules
	# $(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
build:
	# $(MAKE) EXTRAVERSION=$(EXTRAVERSION) --debug=v -C $(KERNELDIR) M=$(PWD) modules
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
