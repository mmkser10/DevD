FILE = test_motor180x
KDIR = /root/raspberry/linux
obj-m := motor180x_driver.o
TOOLCHAIN=arm-linux-gnueabihf-
CC := $(TOOLCHAIN)gcc
PWD := $(shell pwd)

DD:
	make driver
	make app

driver :
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) modules

app :
	$(CC) -o $(FILE) $(FILE).c

install :
	cp -a led_driver.ko /root/raspberry/nfs
	cp -a $(FILE) /root/raspberry/nfs

sdcard:
	cp *.ko /media/mmkser/rootfs/media/motor
	cp $(FILE) /media/mmkser/rootfs/media/motor

clean :	
	rm -f *.ko
	rm -f *.mod.*
	rm -f $(FILE)
	rm -f *.o
	rm -f modules.order
	rm -f Module.symvers 
