ifneq ($(KERNELRELEASE),)
#we zitten in de kerneltree
obj-m := procdriver.o 
else
#we zitten niet in de kerneltree
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: unload compiling load print

unload:
	@echo Modules unloaded
	$(shell sudo rmmod procdriver)

compiling: 
	@echo compiling
	@echo $(PWD)
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules 
	rm -r -f .tmp_versions *.mod.c .*.cmd *.o *.symvers 

load:
	$(shell sudo insmod procdriver.ko)
	@echo Modules loaded!

print:
	dmesg --color=always | tail -n100
	cat /proc/procdriver

endif	