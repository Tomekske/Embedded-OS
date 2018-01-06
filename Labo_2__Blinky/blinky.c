#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>

void singhandler(int signum); 

volatile uint8_t g_usStop = 0;

#define LEDS 8

int main(int argc, char **argv)
{
	if(!bcm2835_init())
	{
		return 1;
	}

	//initialiseren en declareren van een array waarin de registers van de ledjes worden opgeslagen
	int leds[8] = { RPI_V2_GPIO_P1_11,
	                RPI_V2_GPIO_P1_12,
	                RPI_V2_GPIO_P1_13,
	                RPI_V2_GPIO_P1_15,
	                RPI_V2_GPIO_P1_16,
	                RPI_V2_GPIO_P1_18,
	                RPI_V2_GPIO_P1_08,
	                RPI_V2_GPIO_P1_07
				};

	for(int i = 0; i < LEDS; i++)
	{
		bcm2835_gpio_fsel(leds[i],BCM2835_GPIO_FSEL_OUTP); //alle leds instellen als output
		bcm2835_gpio_write(leds[i], LOW); //ervoor zorgen dat ledjes uitstaan voor dat de while(1) wordt opgeroepen
	}

	signal(SIGINT,singhandler); //calback functie

	while(g_usStop == 0)
	{
		for(int i = 0;i < LEDS; i++)
			bcm2835_gpio_write(leds[i], HIGH); //leds aanzetten
		bcm2835_delay(350); //delay

		for(int i = 0;i < LEDS; i++)
			bcm2835_gpio_write(leds[i], LOW); //leds aanzetten
		bcm2835_delay(350); //delay

		signal(SIGINT,singhandler); //fetchen of dat 'ctrl + c' ingedrukt is zodat we programma netjes kunnen afsluiten

	}
	return 0;
}

void singhandler(int signum) 
{
	g_usStop = 1;
}
