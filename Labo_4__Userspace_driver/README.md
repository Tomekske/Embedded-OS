# Userspace blinky C-program #
### 1. Accesing MMU adresses via commandline ###
	cat /proc/device-tree/soc/ranges | od -tx4 --endian=big
### 2. Toggeling a led ###
#### 2.1 Register base adress ####
	#define GPFSEL0	0x200000
	#define GPSET0	0x20001C
	#define GPCLR0	0x200028
	#define GPLEV0 0x200034
	#define GPFSEL1 0x200004
#### 2.2 Reading MMU adresses ####
	fp_ranges = fopen("/proc/device-tree/soc/ranges" , "r")
#### 2.3 Stokcing adresses in variables ####
	fread(&vc_adress,ADRESS_SIZE,ADRESS_NUMBER_ELEMENTS,fp_ranges);
	fread(&arm_adress,ADRESS_SIZE,ADRESS_NUMBER_ELEMENTS,fp_ranges);
	fread(&size,ADRESS_SIZE,ADRESS_NUMBER_ELEMENTS,fp_ranges);
#### 2.4 Swapping LSB(byte) to MSB(byte) ####
	vc_adress = littleToBigEndian(vc_adress);
	arm_adress = littleToBigEndian(arm_adress);
	size = littleToBigEndian(size);
#### 2.5 Accessing systems physical memory ####
	memfd = open("/dev/mem", O_RDWR|O_SYNC)
#### 2.6 Mapping the systems physical memory to virtual adress ####
	uint32_t* map = mmap(NULL, size,(PROT_READ | PROT_WRITE), MAP_SHARED, memfd, arm_adress);
#### 2.7 Set output ####
	 uint32_t setOutput = 0x1 << 21;
#### 2.8 Set GPIO ####
    uint32_t *setGPIO = map + GPFSEL1/4;
#### 2.9 Set pin ####
    uint32_t *setGPIO = map + GPFSEL0/4;
#### 2.10 Clear pin ####
    uint32_t *clearPin = map + GPCLR0/4;
#### 2.11 Initialize led position ####
    uint32_t led = 0x1 << 17;
#### 2.12 Read to adres (setGPIO,setPin, clearPin) ####
	*setGPIO = setOutput;
	*setPin = led;
	*clearPin = led;

# Compile and run C-program #
#### 1. Run makefile #####
	make
#### 2. Run executable ####
	sudo ./blinky
#### 3. Exit program ####
	CTRL + C

