obj-m += HW1_driver.o 
KDIR = /home/benson/linux
all:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR) M=$(shell pwd) modules
	aarch64-linux-gnu-gcc hw1.c -o hw1
clean:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR) M=$(shell pwd) clean
	rm hw1
