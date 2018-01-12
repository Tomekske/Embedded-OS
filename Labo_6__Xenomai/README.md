# Downloading and installing virtual machine #
### 1. Downloading virtual machine ###
	http://www.oracle.com/technetwork/server-storage/virtualbox/downloads/index.html
### 2. Downloading Ubunto ISO ###
	https://www.ubuntu.com/download/desktop
### 3. Setting up image and start VM ###

# Downloading basic GIT commands #
	sudo apt-get install git-all

# Downloading toolchain (cross-compiler) for Raspberry PI 3 #
	git clone https://github.com/raspberrypi/tools
### 1. Adding toolchain to $PATH variable ###
#### 1.1 Open file .bashrc ####
	nano ~/.bashrc
#### 1.2 Add te following piece of code at the very end of the file ####
	export PATH=$PATH:~/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin
#### 1.3 Exit nano ####
#### 1.4 Update $PATH varbiable ####
	source ~/.bashrc
#### 1.5 Check if $PATH is updated ####
	echo $PATH

# Xenomai #
### 1. Create directory ###
	mkdir xenomai
	cd xenomai
### 2. Downlaod xenomai ###
	wget http://xenomai.org/downloads/xenomai/stable/xenomai-3.0.3.tar.bz2
### 3. Extract folder ###
	tar xvjf xenomai-3.0.3.tar.bz2
### 4. Get kernel version ###
	ls xenomai-3.0.3/kernel/cobalt/arch/arm/patches/

# Linux kernel #
### 1. Download kernel in xenomai root ###
	cd ~/Desktop/xenomai
	git clone --depth 1 https://github.com/raspberrypi/linux/ -b rpi-4.1.y
### 2. Get kernel version ###
	head -n4 linux/Makefile
### 3. Simulating of a patch file ### 
	cd linux
	patch --dry-run -p1 < ../xenomai-3.0.3/kernel/cobalt/arch/arm/patches/ipipe-core-4.1.18-arm-8.patch

### 4. This gives an error, so we need to fix it ###
#### 4.1 Edit path file ####
	nano ../xenomai-3.0.3/kernel/cobalt/arch/arm/patches/ipipe-core-4.1.18-arm-8.patch
#### 4.2 Edit the next piece of code ####
	@@ -1718,6 +1731,7 @@ config ALIGNMENT_TRAP
	 config UACCESS_WITH_MEMCPY
	        bool "Use kernel mem{cpy,set}() for {copy_to,clear}_user()"
	        depends on MMU
	+       depends on !IPIPE
	        default y if CPU_FEROCEON
	        help
	          Implement faster copy_to_user and clear_user methods for CPU


#### 4.3 Change to ####
	@@ -1718,6 +1731,7 @@ config ALIGNMENT_TRAP
	 config UACCESS_WITH_MEMCPY
	        bool "Use kernel mem{cpy,set}() for {copy_to,clear}_user()"
	        depends on MMU && !CPU_SW_DOMAIN_PAN
	+       depends on !IPIPE
	        default y if CPU_FEROCEON
	        help
	          Implement faster copy_to_user and clear_user methods for CPU

#### 5. Run the patch simulation again and you shouldn't get an error anymore ####
	patch --dry-run -p1 < ../xenomai-3.0.3/kernel/cobalt/arch/arm/patches/ipipe-core-4.1.18-arm-8.patch
#### 6. Run the real patch file ####
	cd ..
	xenomai-3.0.3/scripts/prepare-kernel.sh --linux=linux/ --arch=arm --ipipe=xenomai-3.0.3/kernel/cobalt/arch/arm/patches/ipipe-core-4.1.18-arm-8.patch
#### 7. Downloading last patch created by Mathieu Rondonneau ####
	~/Desktop/xenomai/
	wget http://www.blaess.fr/christophe/files/article-2016-05-22/patch-xenomai-3-on-bcm-2709.patch
#### 8. Enter linux directory ####
	cd linux
#### 9.	Run simulation patch ####
	patch -p1 --dry-run < ../patch-xenomai-3-on-bcm-2709.patch 
#### 10. Run real patch ####
	patch -p1 < ../patch-xenomai-3-on-bcm-2709.patch

# Compiling #
### 1. Install curses.h ###
	sudo apt-get install libncurses5-dev libncursesw5-dev
### 2. Configuring kernel ###
	cd linux
	KERNEL=kernel7
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig
### 3. Textual configuration interface ###
	CPU Power Management -> CPU Frequency scaling  -> [ ] CPU Frequency scaling
	Kernel Features -> [ ] Contiguous Memory Allocator   
	Kernel Features -> [ ] Allow for memory compaction   
	Kernel hacking -> [ ] KGDB: kernel debugger
	Boot options -> Kernel command line type -> Extend bootloader kernel arguments
### 4. Building kernel ###
	time make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs -j4

# Installing on SD-card #
### 1. List which blockdivices are on your sytem before pluging your SD-card in ###
	lsblk
### 2. List which blockdivices are on your sytem after pluging your SD-card in ###
	lsblk
### 3. Creating parent directories ###
	mkdir -p ../mnt/fat32
	mkdir -p ../mnt/ext4
### 4. Mounting ####
	sudo mount /dev/sdb1 ../mnt/fat32
	sudo mount /dev/sdb2 ../mnt/ext4
### 5. Installing modules ###
	cd ~/Desktop/xenomai/linux
	sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=../mnt/ext4 modules_install
### 6. Copying kernel via mkknlimg script ###
	sudo scripts/mkknlimg arch/arm/boot/zImage /home/mnt/fat32/kernel7-xenomai.img
### 7. Copying device tree files ###
	sudo cp arch/arm/boot/dts/*.dtb /home/mnt/fat32/
	sudo cp arch/arm/boot/dts/overlays/*.dtb* /home/mnt/fat32/overlays/
	sudo cp arch/arm/boot/dts/overlays/README /home/mnt/fat32/overlays/
###  8. Add 'kernel=kernel7-xenomai.img' to /home/mnt/fat32/config.txt
	sudo nano /home/mnt/fat32/config.txt

# Test kernel #
### 1. Get kernel version ###
	uname -a
### 2. Search in dmsg for xenomai and I-pipe###
	dmesg --color=always | tail -n10000
### 3. Get xenomai version ###
	cat /proc/xenomai/version
### 4. Get I-pipe version ###
	cat /proc/ipipe/version

# Xenomai userspace and building tools #
### 1. Check if you have autoconfig and libtoolize installed ###
	autoconf --version
	libtoolize --version
### 2. Test latency ### 
	/usr/xenomai/bin/latency -g output.log
### 3. Run realtime C-program ###
	make
