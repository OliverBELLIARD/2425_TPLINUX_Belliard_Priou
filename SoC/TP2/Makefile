obj-m:=chenillard.o
KERNEL_SOURCE=/home/ensea/linux-socfpga/
CFLAGS_MODULE=-fno-pic
CC=/usr/bin/arm-linux-gnueabihf-gcc

all :
	make -C $(KERNEL_SOURCE) M=$(PWD) modules
clean :
	make -C $(KERNEL_SOURCE) M=$(PWD) clean
install :
	make −C $(KERNEL_SOURCE) M=$(PWD) modules install

