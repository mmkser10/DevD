FILE = test_test
KDIR = /root/raspberry/linux
obj-m := motS_driver.o motG_driver.o motR_driver.o mat_driver.o sensor_driver.o
TOOLCHAIN=arm-linux-gnueabihf-
CC := $(TOOLCHAIN)gcc
PWD := $(shell pwd)

DD:
	make driver
	make app

driver :
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) modules

app :
	$(CC) -o $(FILE) $(FILE).c -pthread

install :
	cp -a led_driver.ko /root/raspberry/nfs
	cp -a $(FILE) /root/raspberry/nfs

sdcard:
	cp *.ko /media/yeo/rootfs/media/motor
	cp $(FILE) /media/yeo/rootfs/media/motor

clean :	
	rm -f *.ko
	rm -f *.mod.*
	rm -f $(FILE)
	rm -f *.o
	rm -f modules.order
	rm -f Module.symvers 
