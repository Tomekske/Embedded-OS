#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void singhandler(int signum); 

volatile uint8_t g_usStop = 0;
int programMode = 0; //default: show once
                     
int main(int argc, char **argv)
{
	 // If you call this, it will not actually access the GPIO
	// Use for testing
	//        bcm2835_set_debug(1);
    if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root?\n");
      return 1;
    }
    if (!bcm2835_spi_begin())
    {
      printf("bcm2835_spi_begin failed. Are you running as root?\n");
      return 1;
    }

    //SPI setup
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);                   // The default    ---- cpol = 1---- cpha= 1  --- mode = 3  see site : http://www.rosseeld.be/DRO/PIC/SPI_Timing.htm
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); // The default	min 10Mhz dus clkdiv 64
    bcm2835_spi_chipSelect(BCM2835_SPI_CS1);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);      // the default
                                                                  
	char buf[4]= {0xFF,0xFF,0xFF,0xFF};
	bcm2835_spi_transfern(buf,4); 
	bcm2835_delay(300);

	signal(SIGINT,singhandler); //calback functie

	//controleren of er te weinig argumenten worden meegegeven
	if(argc < 2)
	{
		printf("Too few arguements:\nSingle read mode: -s\nMultiple read mode: -m\n");
		exit(1);
	}

	else
	{
		//controleren of er een '-' met de argumenten meegegeven wordt
		if(argv[1][0] == '-')
		{
			switch(argv[1][1])
			{
				case 's': programMode = 0; printf("Running program in single read mode\n"); break;
				case 'm': programMode = 1; printf("Running program in multi read mode\n"); break;
				default: printf("Arguments are:\nSingle read mode: -s\nMultiple read mode: -m\n");	exit(1); break;
			}
		}
		else
		{
			printf("Try putting '-' before argument\n");
			exit(1);
		}

		//lus uitvoeren tot als 1 van beide condities niet meer klopt
		do
		{
			char send_data0[2] = {0x50,0X00};
			char read_data0[4];
			bcm2835_spi_transfernb(send_data0,read_data0,3); 

			//temperatuur omzetten
			uint16_t temp = (( uint16_t ) read_data0[1] << 8) + read_data0[2]; //most significant byte op eerste byte zetten
			temp = temp  >> 3; //3 plaatsen naar rechts shiften zodat de databits op de eerste plaats staan
			int celsius = temp / 16; //temperatuur delen door 16 zodat we de temperatuur in graden celsius hebben
			printf("celsius: %d *C\n", celsius );
			signal(SIGINT,singhandler); //fetchen of dat 'ctrl + c' ingedrukt is zodat we programma netjes kunnen afsluiten
			bcm2835_delay(1000);
		} while((g_usStop == 0) && (programMode == 1));

		//
	  	bcm2835_spi_end();
		bcm2835_close();
	    return 0;
	}
   
    
    
    // Send a byte to the slave and simultaneously read a byte back from the slave
    // If you tie MISO to MOSI, you should read back what was sent             command byte == 0b01011000   ==> 0x58    
  	bcm2835_spi_end();
	bcm2835_close();
    return 0;
}

void singhandler(int signum) 
{
	g_usStop = 1;
}
