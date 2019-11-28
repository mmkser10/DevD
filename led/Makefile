FILE = test_led
KDIR = /root/raspberry/linux
obj-m := led_driver.o
TOOLCHAIN=arm-linux-gnueabihf-
CC := $(TOOLCHAIN)gcc
PWD := $(shell pwd)

DD :
	make driver
	make app
driver :
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) modules
app :
	$(CC) -o $(FILE) $(FILE).c
install :
	cp -a led_driver.ko /root/raspberry/nfs
	cp -a $(FILE) /root/raspberry/nfs
clean :	
	rm -f *.ko
	rm -f *.mod.*
	rm -f $(FILE)
	rm -f *.o
	rm -f modules.order
