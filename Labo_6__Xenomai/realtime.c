	#include <stdio.h>
	#include <signal.h>
	#include <unistd.h>
	#include <sys/mman.h>
	 
	#include <alchemy/task.h>
	#include <alchemy/timer.h>
	 
	void hello(void *arg)
	{
	  RT_TASK *me;
	  RT_TASK_INFO infoAboutMe;
	   
	  rt_printf("Hello World! (from the RT task)\n");
	  
	  // De huidige taak opvragen
	  me=rt_task_self();
	  rt_task_inquire(me,&infoAboutMe);
	 
	  // de naam van de taak afprinten:
	  rt_printf("Mijn naam is : %s \n", infoAboutMe.name);
	}
	 
	int main(int argc, char** argv)
	{
	  RT_TASK hello_task;
	  // initialisatie van de rt_print buffers. 
	  //(Normaal doet libcobolt dit voor ons automatisch)
	  //rt_print_auto_init(1);

	  // Belangrijk!!!
	  // We gaan de pages van deze applicatie locken
	  // Op die manier vermijden we de memory swapping voor deze applicatie
	  mlockall(MCL_CURRENT|MCL_FUTURE);

	  rt_printf("Ik ga een nieuwe taak aanmaken.\n");

	  //Voor info over rt_task_create:
	  // http://www.xenomai.org/documentation/xenomai-3/html/xeno3prm/group__alchemy__task.html#ga03387550693c21d0223f739570ccd992
	  //argumenten: RT_TASK, naam, default-stack-size, priority, mode
	  rt_task_create(&hello_task, "Hello", 0, 50, 0); 

	  //Voor info over rt_task_start:
	  // http://www.xenomai.org/documentation/xenomai-3/html/xeno3prm/group__alchemy__task.html#ga3ee3863a29497e2181d49d5b3ecfd855
	  //argumenten: RT_TASK, functie = de taak, pointer dat de functie als argument binnenkrijgt
	  rt_task_start(&hello_task, hello, 0); 
	}