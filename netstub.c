/** 
* @file netstub.c 
* @author Beau V.C. Bellamy  
* @version 0.1 
* @brief 
*/
#include <linux/module.h> /* Needed by all modules */ 
#include <linux/kernel.h> /* Needed for KERN_INFO */ 
#include <linux/init.h>   /* Needed for the macros */ 
#include <linux/netdevice.h>


void setupdev(struct net_device * dev);

MODULE_LICENSE("GPL"); 

// The author -- visible when you use modinfo 
MODULE_AUTHOR("Beau V.C. Bellamy"); 

// The description -- see modinfo 
MODULE_DESCRIPTION("A simple network driver stub!"); 

// The version of the module 
MODULE_VERSION("0.1");

#define MAXDEV 10
struct net_device *devices[MAXDEV];
static int numdevices;

static int __init stub_start(void) 
{ 
    struct net_device *dev;
    int ret;
    numdevices = 0;
    
    printk(KERN_INFO "Loading hellonet module...\n"); 
    
    dev = alloc_netdev(0, "stub%d", NET_NAME_ENUM, setupdev);
    
    if (dev == NULL) {
        printk(KERN_INFO "Device allocation is NULL!\n");
        return -1;
    }
    
    devices[0] = dev;
    ret = register_netdevice(dev);
    printk(KERN_INFO "registration: %d\n", ret); 
    
	return 0; 
}

void setupdev(struct net_device * dev) {
    
    
}


static void __exit stub_end(void) 
{ 
    unregister_netdevice(devices[0]);
    free_netdev(devices[0]);
	printk(KERN_INFO "Goodbye Mr.\n"); 
} 

module_init(stub_start); 
module_exit(stub_end); 

