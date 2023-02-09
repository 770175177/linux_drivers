
/*
 * �ο� drivers\net\cs89x0.c
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/ip.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>

static struct net_device *vnet_dev;

static void emulator_rx_packet(struct sk_buff *skb, struct net_device *dev)
{
	/* �ο�LDD3 */
	unsigned char *type;
	struct iphdr *ih;
	__be32 *saddr, *daddr, tmp;
	unsigned char	tmp_dev_addr[ETH_ALEN];
	struct ethhdr *ethhdr;
	
	struct sk_buff *rx_skb;
		
	// ��Ӳ������/��������
	/* �Ե�"Դ/Ŀ��"��mac��ַ */
	ethhdr = (struct ethhdr *)skb->data;
	memcpy(tmp_dev_addr, ethhdr->h_dest, ETH_ALEN);
	memcpy(ethhdr->h_dest, ethhdr->h_source, ETH_ALEN);
	memcpy(ethhdr->h_source, tmp_dev_addr, ETH_ALEN);

	/* �Ե�"Դ/Ŀ��"��ip��ַ */    
	ih = (struct iphdr *)(skb->data + sizeof(struct ethhdr));
	saddr = &ih->saddr;
	daddr = &ih->daddr;

	tmp = *saddr;
	*saddr = *daddr;
	*daddr = tmp;
	
	//((u8 *)saddr)[2] ^= 1; /* change the third octet (class C) */
	//((u8 *)daddr)[2] ^= 1;
	type = skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr);
	//printk("tx package type = %02x\n", *type);
	// �޸�����, ԭ��0x8��ʾping
	*type = 0; /* 0��ʾreply */
	
	ih->check = 0;		   /* and rebuild the checksum (ip needs it) */
	ih->check = ip_fast_csum((unsigned char *)ih,ih->ihl);
	
	// ����һ��sk_buff
	rx_skb = dev_alloc_skb(skb->len + 2);
	skb_reserve(rx_skb, 2); /* align IP on 16B boundary */	
	memcpy(skb_put(rx_skb, skb->len), skb->data, skb->len);

	/* Write metadata, and then pass to the receive level */
	rx_skb->dev = dev;
	rx_skb->protocol = eth_type_trans(rx_skb, dev);
	rx_skb->ip_summed = CHECKSUM_UNNECESSARY; /* don't check it */
	dev->stats.rx_packets++;
	dev->stats.rx_bytes += skb->len;

	// �ύsk_buff
	netif_rx(rx_skb);
}

static int virt_net_send_packet(struct sk_buff *skb, struct net_device *dev)
{
	static int cnt = 0;
	printk("virt_net_send_packet cnt = %d\n", ++cnt);

	/* ������ʵ������, ��skb�������ͨ���������ͳ�ȥ */
	netif_stop_queue(dev); /* ֹͣ�������Ķ��� */
    /* ...... */           /* ��skb������д������ */

	/* ����һ���ٵ�sk_buff,�ϱ� */
	emulator_rx_packet(skb, dev);

	dev_kfree_skb (skb);   /* �ͷ�skb */
	netif_wake_queue(dev); /* ����ȫ�����ͳ�ȥ��,���������Ķ��� */

	/* ����ͳ����Ϣ */
	dev->stats.tx_packets++;
	dev->stats.tx_bytes += skb->len;
	
	return 0;
}


static int virt_net_init(void)
{
	/* 1. ����һ��net_device�ṹ�� */
	vnet_dev = alloc_netdev(0, "vnet%d", ether_setup);;  /* alloc_etherdev */

	/* 2. ���� */
	vnet_dev->hard_start_xmit = virt_net_send_packet;

	/* ����MAC��ַ */
    vnet_dev->dev_addr[0] = 0x08;
    vnet_dev->dev_addr[1] = 0x89;
    vnet_dev->dev_addr[2] = 0x89;
    vnet_dev->dev_addr[3] = 0x89;
    vnet_dev->dev_addr[4] = 0x89;
    vnet_dev->dev_addr[5] = 0x11;

    /* ���������������pingͨ */
	vnet_dev->flags           |= IFF_NOARP;
	vnet_dev->features        |= NETIF_F_NO_CSUM;	

	/* 3. ע�� */
	//register_netdevice(vnet_dev);
	register_netdev(vnet_dev);
	
	return 0;
}

static void virt_net_exit(void)
{
	unregister_netdev(vnet_dev);
	free_netdev(vnet_dev);
}

module_init(virt_net_init);
module_exit(virt_net_exit);

MODULE_AUTHOR("thisway.diy@163.com,17653039@qq.com");
MODULE_LICENSE("GPL");

