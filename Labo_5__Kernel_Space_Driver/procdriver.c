#include <linux/module.h> //Dynamic loading of modules into the kernel
#include <linux/of.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/types.h> //Adding linux datatypes
#include <linux/io.h>
#include <linux/syscalls.h> //linux systemcall interface
#include <asm-generic/errno-base.h> //Error handeling

#define MODULE_NAME "procdriver: "
#define BUFSIZE 2

#define	GPFSEL1	0x200004
#define	GPSET0	0x20001C
#define	GPCLR0	0x200028

struct device_node *find_node;
uint32_t dev_adress[3];
phys_addr_t offset;
size_t sizes;

//Global vars because you need to acces them on multiple places
uint32_t vc_adress;
uint32_t arm_adress;
uint32_t size;

uint32_t *map;
uint32_t *setGPIO;
uint32_t *setPin;
uint32_t *cleanPin;
uint32_t setOutput;
uint32_t led;

static int proc_show(struct seq_file *m, void *v) 
{
	seq_printf(m, "Hello procdriver!\n"); //cat /proc/procdriver
	return 0;
}

static int proc_open(struct inode *inode, struct  file *file) 
{
	return single_open(file, proc_show, NULL);
}

ssize_t write_proc(struct file * fp, const char __user *userBuffer, size_t buffersize, loff_t * Offset)
{
	char buffer[BUFSIZE];

	printk(KERN_DEBUG MODULE_NAME "writing to procdriver!\n");

	//Copy a block of data from user space
	if(copy_from_user(buffer, userBuffer, BUFSIZE)!= 0)
	{
		printk(KERN_DEBUG MODULE_NAME  "copy to buffer failed!\n"); //dmesg --color=always | tail -n100
	}

	switch(buffer[0])
	{
		case '0': *cleanPin = led; printk(KERN_DEBUG MODULE_NAME "led on\n"); break;
		case '1': *setPin = led; printk(KERN_DEBUG MODULE_NAME "led off\n"); break;
		default: printk(KERN_DEBUG MODULE_NAME "argument invallid: use 1 or 2\n");
	}
	return buffersize;
}

static const struct file_operations hello_proc_fops = 
{
	.owner = THIS_MODULE,
	.open = proc_open,
	.read = seq_read,
	.write = write_proc,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init proc_init(void) 
{	
	proc_create("procdriver", 0, NULL, &hello_proc_fops);
	printk(KERN_DEBUG MODULE_NAME "initialized\n");
	
	/**
	  *	of_find_node_by_name - Find a node by its "name" property
	  *	@from:	The node to start searching from or NULL, the node
	  *		you pass will not be searched, only the next one
	  *		will; typically, you pass what the previous call
	  *		returned. of_node_put() will be called on it
	  *	@name:	The name string to match against
	  *
	  *	Returns a node pointer with refcount incremented, use
	  *	of_node_put() on it when done.
	*/
	find_node = of_find_node_by_name(NULL, "soc");

	/**
	 * of_property_read_u32_array - Find and read an array of 32 bit integers
	 * from a property.
	 *
	 * @np:		device node from which the property value is to be read.
	 * @propname:	name of the property to be searched.
	 * @out_value:	pointer to return value, modified only if return value is 0.
	 *
	 * Search for a property in a device node and read 32-bit value(s) from
	 * it. Returns 0 on success, -EINVAL if the property does not exist,
	 * -ENODATA if property does not have a value, and -EOVERFLOW if the
	 * property data isn't large enough.
	 *
	 * The out_value is modified only if a valid u32 value can be decoded.
	*/
	
	int checkValue = of_property_read_u32_array(find_node, "ranges", dev_adress, 3);

	//error readin property of node
	if(checkValue == 1)
	{
		printk(KERN_DEBUG MODULE_NAME "vc_adress %x\n", *dev_adress);
	}

	printk(KERN_DEBUG MODULE_NAME "vc_adress %x\n", dev_adress[0]);
	printk(KERN_DEBUG MODULE_NAME "arm_adress %x\n", dev_adress[1]);
	printk(KERN_DEBUG MODULE_NAME "size %x\n", dev_adress[2]);

	vc_adress = dev_adress[0];
	arm_adress = dev_adress[1];
	size = dev_adress[2];
	
	map  = (uint32_t*) ioremap(arm_adress, size); // maps bus memory into CPU space

	setOutput = 0x1 << 21;
	led = 0x1 << 17;

	setGPIO = map + GPFSEL1/4;
	setPin = map + GPSET0/4;
	cleanPin = map + GPCLR0/4;
	
	*setGPIO = setOutput;

	return 0;
}

static void __exit proc_exit(void) 
{
	remove_proc_entry("procdriver", NULL);
	printk(KERN_DEBUG MODULE_NAME "exit\n");
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_exit);