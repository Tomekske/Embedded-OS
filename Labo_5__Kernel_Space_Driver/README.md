# Downloading and installing kernel #
### 1. Make sure you are in the root directory ###
	cd ~/
### 2. Installing bc, you need this for 'make modules_prepare' ###
### 3. Installing libncurses5 ###
	sudo apt-get install libncurses5-dev
### Downloading and installing rpi-source ###
	sudo wget https://raw.githubusercontent.com/notro/rpi-source/master/rpi-source -O /usr/bin/rpi-source && sudo chmod +x /usr/bin/rpi-source && /usr/bin/rpi-source -q --tag-update



