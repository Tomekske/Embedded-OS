# Temperature sensor C-program #
### 1. Check command line arguments to determine in which mode the program has to run ### 
	single read mode: -s
	multiple read mode: -m
### 2. Setup SPI ###
#### 2.1 Temperature sensor datasheet (trac) ###
	Datasheet van de SPI temperatuursensor	
#### 2.2 SPI library documentation ####
	http://www.airspayce.com/mikem/bcm2835/group__spi.html
#### 2.3 Constants library definitions ####
	http://www.airspayce.com/mikem/bcm2835/group__constants.html
#### 2.4 Set bit order (MSB) ####
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
#### 2.5 Set data mode (SPI mode 3)
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);   
#### 2.6 Set clock divider (div 64) ####
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);	
#### 2.7 Chip select (cs1) ####
	bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
#### 2.8 Set chip select polarity  ####
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);      
#### 2.9 Transfer any number of bytes ####
	bcm2835_spi_transfern(buf,4);
#### 2.10 Send command byte and receive data #####
	bcm2835_spi_transfernb(send_data0,read_data0,3); 
#### 2.11 Setting most significant byte on first place ####
	uint16_t temp = (( uint16_t ) read_data0[1] << 8) + read_data0[2]; 
#### 2.12 Setting databits on first place ####
	temp = temp  >> 3;
#### 2.13 Getting temperature in degreen celsius ####
	int celsius = temp / 16;

### 3. Safely exit program
#### 3.1 Inlcude headers ####
	#include <signal.h>
#### 3.2 Function definition ####
	void singhandler(int signum); 
#### 3.3 Function call ####
	signal(SIGINT,singhandler); 

# Compile and run C-program #
#### 1 Run makefile #####
	make
#### 2. Run executable ####
	sudo ./temp
#### 3. Exit program ####
	CTRL + C