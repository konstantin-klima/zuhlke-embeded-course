obj-m := nunchuk_reader.o


KDIR := /home/matf/matf-linux-course/buildroot/output/build/linux-4.13/
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
