FILE = test_test
KDIR = /root/raspberry/linux
obj-m := motz_driver.o sensor_driver.o motx_driver.o moty_driver.o mat_driver.o
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
setting :
	mknod /dev/mat_driver c 221 0
	mknod /dev/motx_driver c 222 0
	mknod /dev/moty_driver c 223 0
	mknod /dev/motz_driver c 224 0
	mknod /dev/sensor_driver c 241 0
	insmod mat_driver.ko
	insmod motx_driver.ko
	insmod moty_driver.ko
	insmod motz_driver.ko
	insmod sensor_driver.ko

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
