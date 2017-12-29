# Download and install Raspbian #
### 1. Download and unzip Rasbian image from official website ###
	https://www.raspberrypi.org/downloads/raspbian/
### 2. Download and install Win32 Disk Imager to mount .img on a SD-card
	https://sourceforge.net/projects/win32diskimager/files/
### 3. Plug SD-card in RPI ###
# Log into your pi via putty or any other terminal supporting SSH #
### 1. get IP-adress via Rasbian ###
#### 1.1 Connect the ethernet cable between the RPI and the computer ####
#### 1.2 Get an external mouse, keyboard and monitor and power up the RPI ####
#### 1.3 Get IP-Adress commando ####
	hostname -I

### 2. Check IP-adress via windows terminal ###
####2.1 Get all IP-adresses connected to the interfaces provided by the current interface (0x7)	####
	arp -a
#### 2.2. If both IP-adresses are identical copy the IP-Adress obtained by the arp commando ####

### 3. Connect via SSH to your RPI ###
#### 3.1 Open any shell supporting SSH and paste the following command ####
	ssh user@hostname
#### 3.2 Edit user and hostname according your own user informarion ####
	ssh pi@169.254.204.220
#### 3.3  Enter password and hit enter ####

# Internet acces on RPI #
### 1. Enable shared internet ###
#### 1.1 Open Network and sharing center ####
#### 1.2 Click on "change adapter settings" ####
#### 1.3 Right-click on "Wi-Fi" and select properties ####
#### 1.4 Select the tab "sharing" ####
#### 1.5 Enable checkbox "Allow other network ..." and select "Ethernet" ####

### 2. Re-establish connection with your RPI ###
#### 2.1 Get all IP-adresses connected to the interfaces provided by the current interface (0x7)	####
	arp -a
#### 2.2 Copy the IP-adress just under the RPI's IP-adress and reconect to the RPI ####
	ssh user@new_hostname

### 3. Internet connection ####
#### 3.1 Test the internet connection
	ping 8.8.8.8
#### 3.2 If data packets are getting transmitted you can exit the program
	CTRL + C


