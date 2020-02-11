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

struct stub_priv {
    struct net_device_stats stats;
    int status;
    struct snull_packet *ppool;
    struct snull_packet *rx_queue;  /* List of incoming packets */
    int rx_int_enabled;
    int tx_packetlen;
    u8 *tx_packetdata;
    struct sk_buff *skb;
    spinlock_t lock;
};


static int __init stub_start(void) 
{ 
    struct net_device *dev;
    int ret;
    numdevices = 0;
    
    printk(KERN_INFO "Loading hellonet module...\n"); 
    
    dev = alloc_netdev(sizeof(struct stub_priv), "stub%d", NET_NAME_ENUM, setupdev);
    
    if (dev == NULL) {
        printk(KERN_INFO "Device allocation is NULL!\n");
        return -1;
    }
    
    devices[0] = dev;
    ret = register_netdev(dev);
    printk(KERN_INFO "registration: %i on \"%s\"\n", ret, dev->name); 
    
    return 0; 
}




int stub_hdr(struct sk_buff *skb, struct net_device *dev,
                unsigned short type, const void *daddr, const void *saddr,
                unsigned len)
{
    // STUB
    printk(KERN_INFO "netstub: Create Header\n"); 
    return (0);
}


int stub_hdr_parse(const struct sk_buff *skb, unsigned char *haddr) {

    // STUB
    printk(KERN_INFO "netstub: Parse Header\n"); 
    return 0;
}

int stub_hdr_cache(const struct neighbour *neigh, struct hh_cache *hh, __be16 type) {
    
    // STUB
    printk(KERN_INFO "netstub: Header Cache\n"); 
    return 0;
}


void stub_hdr_cache_update(struct hh_cache *hh, const struct net_device *dev, const unsigned char *haddr) {

    // STUB
    printk(KERN_INFO "netstub: Header Cache Update\n"); 
    return;
}

bool stub_hdr_validate(const char *ll_header, unsigned int len) {

    // STUB
    printk(KERN_INFO "netstub: Header Validate\n");
    return 1;
}

__be16 stub_hdr_parse_protocol(const struct sk_buff *skb) {
    
    // STUB
    printk(KERN_INFO "netstub: Header Parse Protocol\n");
    return 0;
    
}




int stub_open(struct net_device *dev)
{
    // STUB
    printk(KERN_INFO "netstub: Open Device \"%s\"\n", dev->name);
    return 0;
}


int stub_change_mtu(struct net_device *dev, int new_mtu)
{
    // STUB
    printk(KERN_INFO "netstub: \"%s\" Change MTU to %i\n", dev->name, new_mtu);
    return 0; /* success */
}

int stub_release(struct net_device *dev)
{
    // STUB
    printk(KERN_INFO "netstub: Release Device \"%s\"\n", dev->name);
    return 0;
}

int stub_tx(struct sk_buff *skb, struct net_device *dev)
{
    // STUB
    printk(KERN_INFO "netstub: TX \"%s\"\n", dev->name);
    return 0;
}

void stub_rx(struct net_device *dev, struct snull_packet *pkt)
{
    // STUB
    printk(KERN_INFO "netstub: RX \"%s\"\n", dev->name);
    return;
}


int stub_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
    printk(KERN_INFO "netstub: IOCTRL \"%s\", cmd:%i \n", dev->name, cmd);
    return 0;
}


int stub_config(struct net_device *dev, struct ifmap *map)
{
    // STUB
    printk(KERN_INFO "netstub: Config \"%s\"\n", dev->name);
    return 0;
}


struct net_device_stats *stub_stats(struct net_device *dev)
{
    struct stub_priv *priv = netdev_priv(dev);
    return &priv->stats;
}

void stub_tx_timeout (struct net_device *dev)
{
    // STUB
    return;
}


static const struct header_ops stub_header_ops = {
    .create  = stub_hdr,
    .parse = stub_hdr_parse,
    .cache = stub_hdr_cache,
    .cache_update = stub_hdr_cache_update,
    .validate = stub_hdr_validate,
    .parse_protocol = stub_hdr_parse_protocol
};

static const struct net_device_ops stub_netdev_ops = {
    .ndo_open            = stub_open,
    .ndo_stop            = stub_release,
    .ndo_start_xmit      = stub_tx,
    .ndo_do_ioctl        = stub_ioctl,
    .ndo_set_config      = stub_config,
    .ndo_get_stats       = stub_stats,
    .ndo_change_mtu      = stub_change_mtu,
    .ndo_tx_timeout      = stub_tx_timeout
};


void setupdev(struct net_device * dev) {
    
    int timeout;
    ether_setup(dev); 

    timeout = 100;
    
    dev->watchdog_timeo = timeout;
	dev->netdev_ops = &stub_netdev_ops;
	dev->header_ops = &stub_header_ops;
	/* keep the default flags, just add NOARP */
	dev->flags           |= IFF_NOARP;
	dev->features        |= NETIF_F_HW_CSUM;
    
}


static void __exit stub_end(void) 
{ 
    unregister_netdev(devices[0]);
    free_netdev(devices[0]);
	printk(KERN_INFO "\n"); 
} 

module_init(stub_start); 
module_exit(stub_end); 

