CONFIG_MODULE_SIG=n

obj-m = usbdrv.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

all:
	make -C $(KERNELDIR) M=$(PWD) modules
	# $(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
build:
	# $(MAKE) EXTRAVERSION=$(EXTRAVERSION) --debug=v -C $(KERNELDIR) M=$(PWD) modules
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
