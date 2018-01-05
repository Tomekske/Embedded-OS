#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h> //contains constructs that refer to file control, e.g. opening a file, retrieving and changing the permissions of file, locking a file for edit, etc.
#include <time.h>
#include <signal.h>

uint32_t littleToBigEndian(uint32_t adress);
void delay(long miliseconds);
void singhandler(int signum); 

volatile uint8_t g_usStop = 0;

//pagine 90 in datasheet
#define GPFSEL0	0x200000	// GPIO function select registers
#define GPSET0	0x20001C	//GPIO pin output set registers
#define GPCLR0	0x200028	//GPIO pin output clear registers
#define GPLEV0 0x200034

#define ADRESS_SIZE 4
#define ADRESS_NUMBER_ELEMENTS 1

int main()
{
	FILE *fp_ranges;
	int memfd;

	//checking if the file exsists
	if ((fp_ranges = fopen("/proc/device-tree/soc/ranges" , "r")) < 0) 
	{
		printf("can't open /proc/device-tree/soc/ranges\n"); 
		exit(-1);
	}

	uint32_t vc_adress;
	uint32_t arm_adress;
	uint32_t size;

	//reading MMU adressen
	fread(&vc_adress,ADRESS_SIZE,ADRESS_NUMBER_ELEMENTS,fp_ranges); //reading blocks of 8bit 32/4 = 8
	fread(&arm_adress,ADRESS_SIZE,ADRESS_NUMBER_ELEMENTS,fp_ranges);
	fread(&size,ADRESS_SIZE,ADRESS_NUMBER_ELEMENTS,fp_ranges);
	fclose(fp_ranges);

	//Swapping LSB to MSB
	vc_adress = littleToBigEndian(vc_adress);
	arm_adress = littleToBigEndian(arm_adress);
	size = littleToBigEndian(size);

	printf("Videocore adress 0x%x\n",vc_adress);
	printf("Arm adress 0x%x\n",arm_adress);
	printf("Size 0x%x\n",size);

	//Accessing systems physocal memory 
	if ((memfd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
	{
		printf("can't open /dev/mem \n"); 
		exit(-1);
	}

	uint32_t* map = mmap(NULL, size,(PROT_READ | PROT_WRITE), MAP_SHARED, memfd, arm_adress); //map adresses in MMU, accesible via virtual adress

	if (map == MAP_FAILED)
	{
		printf("Mmap failed");
		return -1;
	}
  uint32_t led = 0x1 << 17;

  uint32_t *setGPIO = map + GPFSEL0/4;
  uint32_t *setPin = map + GPSET0/4;
  uint32_t *clearPin = map + GPCLR0/4;

  __sync_synchronize();

  signal(SIGINT,singhandler); //calback functie
    
	while(g_usStop == 0)
	{
		*setPin = led;
		delay(125);
		*clearPin = led;
		delay(125);

    signal(SIGINT,singhandler); //calback functie
	}

	close(memfd);
   
	return 0;
}

/**
 * @brief      Swap least significant byte to most significant byte
 * @param[in]  adress  The adress you want to swap 
 * @return     Swapped adress
 */
uint32_t littleToBigEndian(uint32_t adress)
{
	//http://www.coders-hub.com/2013/04/convert-little-endian-to-big-endian-in-c.html#.Wkvu1FXiZhE
	int byte0, byte1, byte2, byte3;

	byte0 = (adress & 0x000000FF) >> 0 ;
	byte1 = (adress & 0x0000FF00) >> 8 ;
	byte2 = (adress & 0x00FF0000) >> 16 ;
	byte3 = (adress & 0xFF000000) >> 24 ;

	return ((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | (byte3 << 0));
}

/**
 * @brief      Delay function in miliseconds
 * @param[in]  miliseconds  amount of time you want to delay
 */
void delay(long miliseconds)
{
  //https://stackoverflow.com/questions/7684359/how-to-use-nanosleep-in-c-what-are-tim-tv-sec-and-tim-tv-nsec
   struct timespec req, rem;

   if(miliseconds > 999)
   {   
        req.tv_sec = (int)(miliseconds / 1000);                            /* Must be Non-Negative */
        req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
   }   
   else
   {   
        req.tv_sec = 0;                         /* Must be Non-Negative */
        req.tv_nsec = miliseconds * 1000000;    /* Must be in range of 0 to 999999999 */
   }   

   nanosleep(&req , &rem);
}

void singhandler(int signum) 
{
  g_usStop = 1;
}
