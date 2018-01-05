# Userspace blinky C-program #
### 1. Accesing MMU adresses via commandline ###
	cat /proc/device-tree/soc/ranges | od -tx4 --endian=big
### 2. Toggeling a led ###
#### 2.1 Reading MMU adresses ###
	fp_ranges = fopen("/proc/device-tree/soc/ranges" , "r")
#### 2.2 Stokcing adresses in variables ####
	fread(&vc_adress,ADRESS_SIZE,ADRESS_NUMBER_ELEMENTS,fp_ranges);
	fread(&arm_adress,ADRESS_SIZE,ADRESS_NUMBER_ELEMENTS,fp_ranges);
	fread(&size,ADRESS_SIZE,ADRESS_NUMBER_ELEMENTS,fp_ranges);
#### 2.3 Swapping LSB(byte) to MSB(byte) ####
	vc_adress = littleToBigEndian(vc_adress);
	arm_adress = littleToBigEndian(arm_adress);
	size = littleToBigEndian(size);
#### 2.4 Accessing systems physical memory ####
	memfd = open("/dev/mem", O_RDWR|O_SYNC)
#### 2.5 Mapping the systems physical memory to virtual adress #####
	uint32_t* map = mmap(NULL, size,(PROT_READ | PROT_WRITE), MAP_SHARED, memfd, arm_adress);
#### 2.6 Set GPIO ####
    uint32_t *setGPIO = map + GPFSEL0/4;
#### 2.7 Set pin ####
    uint32_t *setGPIO = map + GPFSEL0/4;
#### 2.8 Clear pin ####
    uint32_t *clearPin = map + GPCLR0/4;
#### 2.9 Initialize led position ####
    uint32_t led = 0x1 << 17;
#### 3.0 Read to adres (setPin, clearPin ) ####
	*setPin = led;
	*clearPin = led;

# Compile and run C-program #
#### 1. Run makefile #####
	make
#### 2. Run executable ####
	sudo ./blinky
#### 3. Exit program ####
	CTRL + C

