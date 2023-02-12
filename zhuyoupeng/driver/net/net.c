#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/interrupt.h> /* mark_bh */
#include <linux/in.h>
#include <linux/netdevice.h>   /* struct device, and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>          /* struct iphdr */
#include <linux/tcp.h>         /* struct tcphdr */
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/in6.h>
#include <asm/uaccess.h>
#include <asm/checksum.h>
#include <linux/platform_device.h>


// 如果需要随机MAC地址则定义该宏
#define  MAC_AUTO


static struct net_device *astonnet_devs;		

//网络设备结构体,作为net_device->priv
struct astonnet_priv {
    struct net_device_stats stats;		//有用的统计信息
    int status;							//网络设备的状态信息，是发完数据包，还是接收到网络数据包
    int rx_packetlen;					//接收到的数据包长度
    u8 *rx_packetdata;					//接收到的数据
    int tx_packetlen;					//发送的数据包长度
    u8 *tx_packetdata;					//发送的数据
    struct sk_buff *skb;				//socket buffer结构体，网络各层之间传送数据都是通过这个结构体来实现的
    spinlock_t lock;					//自旋锁
};

//网络接口的打开函数
int astonnet_open(struct net_device *dev)
{
	printk("astonnet_open\n");
	
#ifndef MAC_AUTO
	int i;
	for (i=0; i<6; i++)
	    dev->dev_addr[i] = 0xaa;
#else
    random_ether_addr(dev->dev_addr); //随机源地址
#endif
    netif_start_queue(dev);		//打开传输队列，这样才能进行数据传输
	
    return 0;
}

int astonnet_release(struct net_device *dev)
{
    printk("astonnet_release\n");
	//当网络接口关闭的时候，调用stop方法，这个函数表示不能再发送数据
    netif_stop_queue(dev);
	
    return 0;
}

//接包函数
void astonnet_rx(struct net_device *dev, int len, unsigned char *buf)
{   
    struct sk_buff *skb;
    struct astonnet_priv *priv = (struct astonnet_priv *) dev->ml_priv;
    skb = dev_alloc_skb(len+2);//分配一个socket buffer,并且初始化skb->data,skb->tail和skb->head
    if (!skb) {
        printk("gecnet rx: low on mem - packet dropped\n");
        priv->stats.rx_dropped++;
        return;
    }
    skb_reserve(skb, 2); /* align IP on 16B boundary */ 
    memcpy(skb_put(skb, len), buf, len);//skb_put是把数据写入到socket buffer
    /* Write metadata, and then pass to the receive level */
    skb->dev = dev;
    skb->protocol = eth_type_trans(skb, dev);//返回的是协议号
    skb->ip_summed = CHECKSUM_UNNECESSARY; //此处不校验
    priv->stats.rx_packets++;//接收到包的个数＋1
    
    priv->stats.rx_bytes += len;//接收到包的长度
    printk("astonnet rx \n");
    netif_rx(skb);//通知内核已经接收到包，并且封装成socket buffer传到上层
    return;
}

//真正的处理的发送数据包
//模拟从一个网络向另一个网络发送数据包
void astonnet_hw_tx(char *buf, int len, struct net_device *dev)

{
    struct net_device *dest;//目标设备结构体，net_device存储一个网络接口的重要信息，是网络驱动程序的核心
    struct astonnet_priv *priv;

    if (len < sizeof(struct ethhdr) + sizeof(struct iphdr)) 
	{
		printk("astonnet: Hmm... packet too short (%i octets)\n", len);
        return;
    }

    dest = astonnet_devs;
    priv = (struct astonnet_priv *)dest->ml_priv;		//目标dest中的priv
    priv->rx_packetlen = len;
    priv->rx_packetdata = buf;

    printk("astonnet tx \n");
    dev_kfree_skb(priv->skb);
}

//发包函数
int astonnet_tx(struct sk_buff *skb, struct net_device *dev)
{
	int len;
    char *data;
    struct astonnet_priv *priv = (struct astonnet_priv *)dev->ml_priv;

    if (skb == NULL)
	{
		printk("net_device %p,  skb %p\n", dev,  skb);
		return 0;
    }

    len = skb->len < ETH_ZLEN ? ETH_ZLEN : skb->len;//ETH_ZLEN是所发的最小数据包的长度
    data = skb->data;//将要发送的数据包中数据部分
    priv->skb = skb;
    astonnet_hw_tx(data, len, dev);//真正的发送函数
	
	return 0; 
}


//设备初始化函数
int astonnet_init(struct net_device *dev)
{
	printk("astoncnet_init\n");

    ether_setup(dev);//填充一些以太网中的设备结构体的项
    
    /* keep the default flags, just add NOARP */
    dev->flags           |= IFF_NOARP;

//为priv分配内存
	dev->ml_priv = kmalloc(sizeof(struct astonnet_priv), GFP_KERNEL);
	if (dev->ml_priv == NULL)
		return -ENOMEM;
    memset(dev->ml_priv, 0, sizeof(struct astonnet_priv));
    spin_lock_init(&((struct astonnet_priv *)dev->ml_priv)->lock);
	
    return 0;
}


static const struct net_device_ops astonnet_netdev_ops = {
	.ndo_open		= astonnet_open,			// 打开网卡	对应 ifconfig xx up
	.ndo_stop		= astonnet_release,			// 关闭网卡	对应 ifconfig xx down
	.ndo_start_xmit	= astonnet_tx,				// 开启数据包传输
	.ndo_init       = astonnet_init,			// 初始化网卡硬件
};

static void	aston_plat_net_release(struct device *dev)
{
	printk("aston_plat_net_release\n");
}

static int __devinit aston_net_probe(struct platform_device *pdev)
{
	int result=0;

	astonnet_devs = alloc_etherdev(sizeof(struct net_device));
	astonnet_devs->netdev_ops = &astonnet_netdev_ops;

	strcpy(astonnet_devs->name, "astonnet0");
	if ((result = register_netdev(astonnet_devs)))
		printk("astonnet: error %i registering device \"%s\"\n", result,  astonnet_devs->name);
	
	return 0;
}

static int  __devexit  aston_net_remove(struct platform_device *pdev)   //设备移除接口
{
        kfree(astonnet_devs->ml_priv);
        unregister_netdev(astonnet_devs);
        return 0;
}


static struct platform_device aston_net= {
	.name	= "aston_net",
	.id		= -1,
    .dev	= {
		.release = aston_plat_net_release,
	},
};

static struct platform_driver aston_net_driver = {
    .probe  = aston_net_probe,                 	
    .remove  = __devexit_p(aston_net_remove),  	

	.driver  = {
		.name ="aston_net",
		.owner = THIS_MODULE,
	},
};


static int __init aston_net_init(void)
{
	printk("aston_net_init \n");
	
	platform_device_register(&aston_net);
	return platform_driver_register(&aston_net_driver );    	
}

static void __exit aston_net_cleanup(void)
{
	platform_driver_unregister(&aston_net_driver );    	
	platform_device_unregister(&aston_net);
}

module_init(aston_net_init);
module_exit(aston_net_cleanup);


MODULE_LICENSE("GPL");

