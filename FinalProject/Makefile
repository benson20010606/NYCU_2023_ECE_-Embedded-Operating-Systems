obj-m += driver.o 
KDIR = /home/benson/linux
all:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR) M=$(shell pwd) modules
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-gcc server.c -o server
clean:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR) M=$(shell pwd) clean
	rm server


