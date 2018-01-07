# Downloading and installing kernel #
### 1. Installing bc, you need this for 'make modules_prepare' ###
### 2. Installing libncurses5 ###
	sudo apt-get install libncurses5-dev
### 3. Downloading and installing rpi-source ###
	sudo wget https://raw.githubusercontent.com/notro/rpi-source/master/rpi-source -O /usr/bin/rpi-source && sudo chmod +x /usr/bin/rpi-source && /usr/bin/rpi-source -q --tag-update

# Printing out printk and seq_printf #
### 1. printk ###
	dmesg --color=always | tail -n100
### 2. seq_printf ###
	cat /proc/procdriver

# Procdriver C-program #
### 1. Including kernel libraries ###
	#include <linux/module.h>
	#include <linux/of.h>
	#include <linux/proc_fs.h>
	#include <linux/seq_file.h>
	#include <linux/types.h>
	#include <linux/io.h>
	#include <linux/syscalls.h>
	#include <asm-generic/errno-base.h>
### 2 Register base adress ###
	#define GPFSEL0	0x200000
	#define GPSET0	0x20001C
	#define GPCLR0	0x200028
	#define GPLEV0 0x200034
	#define GPFSEL1 0x200004
### 3. Find a node by its "name" property ###
	find_node = of_find_node_by_name(NULL, "soc");
### 4. Find and read an array of 32 bit integers from a property (ranges) ###
	int checkValue = of_property_read_u32_array(find_node, "ranges", dev_adress, 3);
### 5. Initializing MMU adresses ###
	vc_adress = dev_adress[0];
	arm_adress = dev_adress[1];
	size = dev_adress[2];
### 6. Mapping bus memory int CPU space ###
	map  = (uint32_t*) ioremap(arm_adress, size);
### 7 Set output ###
	 uint32_t setOutput = 0x1 << 21;
### 8 Set GPIO ###
    uint32_t *setGPIO = map + GPFSEL1/4;
### 9 Set pin ###
    uint32_t *setGPIO = map + GPFSEL0/4;
### 10 Clear pin ###
    uint32_t *clearPin = map + GPCLR0/4;
### 11 Initialize led position ###
    uint32_t led = 0x1 << 17;
### 12 Read to adres (setGPIO,setPin, clearPin) ###
	*setGPIO = setOutput;
	*setPin = led;
	*clearPin = led;

# Compiling and runnin kernel module #
### 1. Make sure the kernel module is unloaded ###
	sudo rmmod procdriver
### 2. Compile kernel module #
	make
### 3. Load kernel module #
	sudo insmod procdriver.ko
### 4. Turn led on ###
	echo 1 | sudo tee /proc/procdriver
### 5. Turn led off ###
	echo 0 | sudo tee /proc/procdriver