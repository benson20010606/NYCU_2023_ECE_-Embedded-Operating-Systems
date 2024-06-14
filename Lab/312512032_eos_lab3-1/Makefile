obj-m += lab3-1_driver.o 
KDIR = /home/benson/linux
all:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR) M=$(shell pwd) modules
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-gcc lab3-1_writer.c -o writer
clean:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR) M=$(shell pwd) clean
