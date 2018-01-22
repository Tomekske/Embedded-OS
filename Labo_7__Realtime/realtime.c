#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>

#define	PRIORITY 50

void demo(void *arg) 
{
	RT_TASK *curtask;
	RT_TASK_INFO curtaskinfo;
	curtask=rt_task_self();
	rt_task_inquire(curtask,&curtaskinfo);
	rt_printf("Task name: %s \n", curtaskinfo.name);
}

void demo_integer(void *arg) 
{
	RT_TASK *curtask;
	RT_TASK_INFO curtaskinfo;
	curtask=rt_task_self();
	rt_task_inquire(curtask,&curtaskinfo);
	int numb = *(int*)arg;
	rt_printf("Task integer name: %d \n", numb);
}

int main(int argc, char** argv)
{
	RT_TASK demo1, demo2, demo3, demo4, demo5;
	RT_TASK demo_int1, demo_int2, demo_int3, demo_int4, demo_int5;

	// We gaan de pages van deze applicatie locken
	// Op die manier vermijden we de memory swapping voor deze applicatie
	mlockall(MCL_CURRENT|MCL_FUTURE);

	// http://www.xenomai.org/documentation/xenomai-3/html/xeno3prm/group__alchemy__task.html#ga03387550693c21d0223f739570ccd992
	//argumenten: RT_TASK, naam, default-stack-size, priority, mode
	rt_task_create(&demo1, "demo1", 0, PRIORITY, 0); 
	rt_task_create(&demo2, "demo2", 0, PRIORITY, 0); 
	rt_task_create(&demo3, "demo3", 0, PRIORITY, 0); 
	rt_task_create(&demo4, "demo4", 0, PRIORITY, 0); 
	rt_task_create(&demo5, "demo5", 0, PRIORITY, 0); 

	rt_task_create(&demo_int1, "demo_int_1", 0, 1, 0); 
	rt_task_create(&demo_int2, "demo_int_2", 0, 20, 0); 
	rt_task_create(&demo_int3, "demo_int_3", 0, 30, 0); 
	rt_task_create(&demo_int4, "demo_int_4", 0, 40, 0); 
	rt_task_create(&demo_int5, "demo_int_5", 0, 50, 0); 

	// http://www.xenomai.org/documentation/xenomai-3/html/xeno3prm/group__alchemy__task.html#ga3ee3863a29497e2181d49d5b3ecfd855
	//argumenten: RT_TASK, functie = de taak, pointer dat de functie als argument binnenkrijgt
	rt_task_start(&demo1, demo, 0); 
	rt_task_start(&demo2, demo, 0); 
	rt_task_start(&demo3, demo, 0); 
	rt_task_start(&demo4, demo, 0); 
	rt_task_start(&demo5, demo, 0);
	rt_task_start(&demo_int1, demo_integer, "10"); 
	rt_task_start(&demo_int2, demo_integer, "20"); 
	rt_task_start(&demo_int3, demo_integer, "30"); 
	rt_task_start(&demo_int4, demo_integer, "40"); 
	rt_task_start(&demo_int5, demo_integer, "50"); 
}