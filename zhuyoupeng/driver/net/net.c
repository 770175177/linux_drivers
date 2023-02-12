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


// �����Ҫ���MAC��ַ����ú�
#define  MAC_AUTO


static struct net_device *astonnet_devs;		

//�����豸�ṹ��,��Ϊnet_device->priv
struct astonnet_priv {
    struct net_device_stats stats;		//���õ�ͳ����Ϣ
    int status;							//�����豸��״̬��Ϣ���Ƿ������ݰ������ǽ��յ��������ݰ�
    int rx_packetlen;					//���յ������ݰ�����
    u8 *rx_packetdata;					//���յ�������
    int tx_packetlen;					//���͵����ݰ�����
    u8 *tx_packetdata;					//���͵�����
    struct sk_buff *skb;				//socket buffer�ṹ�壬�������֮�䴫�����ݶ���ͨ������ṹ����ʵ�ֵ�
    spinlock_t lock;					//������
};

//����ӿڵĴ򿪺���
int astonnet_open(struct net_device *dev)
{
	printk("astonnet_open\n");
	
#ifndef MAC_AUTO
	int i;
	for (i=0; i<6; i++)
	    dev->dev_addr[i] = 0xaa;
#else
    random_ether_addr(dev->dev_addr); //���Դ��ַ
#endif
    netif_start_queue(dev);		//�򿪴�����У��������ܽ������ݴ���
	
    return 0;
}

int astonnet_release(struct net_device *dev)
{
    printk("astonnet_release\n");
	//������ӿڹرյ�ʱ�򣬵���stop���������������ʾ�����ٷ�������
    netif_stop_queue(dev);
	
    return 0;
}

//�Ӱ�����
void astonnet_rx(struct net_device *dev, int len, unsigned char *buf)
{   
    struct sk_buff *skb;
    struct astonnet_priv *priv = (struct astonnet_priv *) dev->ml_priv;
    skb = dev_alloc_skb(len+2);//����һ��socket buffer,���ҳ�ʼ��skb->data,skb->tail��skb->head
    if (!skb) {
        printk("gecnet rx: low on mem - packet dropped\n");
        priv->stats.rx_dropped++;
        return;
    }
    skb_reserve(skb, 2); /* align IP on 16B boundary */ 
    memcpy(skb_put(skb, len), buf, len);//skb_put�ǰ�����д�뵽socket buffer
    /* Write metadata, and then pass to the receive level */
    skb->dev = dev;
    skb->protocol = eth_type_trans(skb, dev);//���ص���Э���
    skb->ip_summed = CHECKSUM_UNNECESSARY; //�˴���У��
    priv->stats.rx_packets++;//���յ����ĸ�����1
    
    priv->stats.rx_bytes += len;//���յ����ĳ���
    printk("astonnet rx \n");
    netif_rx(skb);//֪ͨ�ں��Ѿ����յ��������ҷ�װ��socket buffer�����ϲ�
    return;
}

//�����Ĵ���ķ������ݰ�
//ģ���һ����������һ�����緢�����ݰ�
void astonnet_hw_tx(char *buf, int len, struct net_device *dev)

{
    struct net_device *dest;//Ŀ���豸�ṹ�壬net_device�洢һ������ӿڵ���Ҫ��Ϣ����������������ĺ���
    struct astonnet_priv *priv;

    if (len < sizeof(struct ethhdr) + sizeof(struct iphdr)) 
	{
		printk("astonnet: Hmm... packet too short (%i octets)\n", len);
        return;
    }

    dest = astonnet_devs;
    priv = (struct astonnet_priv *)dest->ml_priv;		//Ŀ��dest�е�priv
    priv->rx_packetlen = len;
    priv->rx_packetdata = buf;

    printk("astonnet tx \n");
    dev_kfree_skb(priv->skb);
}

//��������
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

    len = skb->len < ETH_ZLEN ? ETH_ZLEN : skb->len;//ETH_ZLEN����������С���ݰ��ĳ���
    data = skb->data;//��Ҫ���͵����ݰ������ݲ���
    priv->skb = skb;
    astonnet_hw_tx(data, len, dev);//�����ķ��ͺ���
	
	return 0; 
}


//�豸��ʼ������
int astonnet_init(struct net_device *dev)
{
	printk("astoncnet_init\n");

    ether_setup(dev);//���һЩ��̫���е��豸�ṹ�����
    
    /* keep the default flags, just add NOARP */
    dev->flags           |= IFF_NOARP;

//Ϊpriv�����ڴ�
	dev->ml_priv = kmalloc(sizeof(struct astonnet_priv), GFP_KERNEL);
	if (dev->ml_priv == NULL)
		return -ENOMEM;
    memset(dev->ml_priv, 0, sizeof(struct astonnet_priv));
    spin_lock_init(&((struct astonnet_priv *)dev->ml_priv)->lock);
	
    return 0;
}


static const struct net_device_ops astonnet_netdev_ops = {
	.ndo_open		= astonnet_open,			// ������	��Ӧ ifconfig xx up
	.ndo_stop		= astonnet_release,			// �ر�����	��Ӧ ifconfig xx down
	.ndo_start_xmit	= astonnet_tx,				// �������ݰ�����
	.ndo_init       = astonnet_init,			// ��ʼ������Ӳ��
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

static int  __devexit  aston_net_remove(struct platform_device *pdev)   //�豸�Ƴ��ӿ�
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

