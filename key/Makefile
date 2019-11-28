FILE = test_key
KDIR = /root/raspberry/linux
obj-m := key_driver.o
TOOLCHAIN = arm-linux-gnueabihf-
CC := $(TOOLCHAIN)gcc
PWD := $(shell pwd)

driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) modules
app:
	$(CC) -o $(FILE) $(FILE).c
install:
	cp -a key_driver.ko /root/raspberry/nfs
	cp -a $(FILE) /root/raspberry/nfs
clean:
	rm -f *.ko
	rm -f *.mod.*
	rm -f *.o
	rm -f $(FILE)
	rm -f modules.order
	rm -f Module.symvers 
