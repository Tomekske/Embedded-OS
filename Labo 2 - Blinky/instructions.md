# Download and install bmc2835 library #
### 1. Download bmc2835 library from official website ###
#### 1.1 Go to the directory where you want to dowload the library####
	cd ~/Desktop/labo/bcm2835/
#### 1.2 Download bcm3835 library #####
	wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.52.tar.gz
#### 1.3 Check if library is installed #####
	ls -l

### 2. Unzip the library
	tar xvfz bcm2835-1.52.tar.gz 

### 3. Compiling and installing the library ###
#### 3.1 Go in to unzipped library directory ####
	cd bcm2835-1.52
#### 3.2 Run the following script ####
	./configure
#### 3.3 Run the makefile ####
	make
#### 3.4 Run check commando from makefile ####
	sudo make check
#### 3.5 Run install commando from makefile ####
	sudo make install

# Develop C-programs on laptop and run them on the RPI #
### 1. WinSCP FTP client on windows machine ###
#### 1.1 Download WinSCP FTP client on windows machine ###
	https://winscp.net/eng/download.php
#### 1.2 Log into your RPI on WinSCP FTP client ####
#### 1.3 Create a file in correct folder and drag it from the 'source' to 'destination' folder ####
#### 1.4 Double click the dragged file in the 'destination' folder and you're ready to develop
#### 1.5 Make sure to drag back the most recent version of  the file back to the source folder on the windows machine ####

# Blinky C-program #
### 1. Download PI-hat datashield which you can find on trac ###
	Schema van de Pi HAT / shield
### 2. Find GPIO registers where the leds are connected to ###
	D1 = GPIO0
### 3. Find wich pin of the header GPIO is connected to ###
	GPIO0 =  pin_11
### 4. Get macro of the GPIO0 register in your code ###
	GPIO0 =  pin_11 = RPI_V2_GPIO_P1_11

# Compile and run C-program #
#### 1.1 Run makefile #####
	make
#### 1.2 Run executable ####
	sudo ./blinky
#### 1.3 Exit program ####
	CTRL + C



