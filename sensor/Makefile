FILE = test_sensor
KDIR = /root/raspberry/linux
obj-m := sensor_driver.o
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
	cp -a sensor_driver.ko /root/raspberry/DD/sensor
	cp -a $(FILE) /root/raspberry/DD/sensor

sdcard:
	cp *.ko /media/yeo/rootfs/media/sensor
	cp $(FILE) /media/yeo/rootfs/media/sensor

clean : 
	rm -f *.ko
	rm -f *.mod.*
	rm -f $(FILE)
	rm -f *.o
	rm -f modules.order
	rm -f Module.symvers 
