/*
 * An I2C driver for the Lite-On Three-in-one Environmental Sensor ap3216c
 * Copyright (C) 2019-5 Guangzhou ALIENTEK Electronic Technology Co., Ltd.
 *
 * Author: Deng Tao <773904075@qq.com>
 * date: 2019/05/28
 * Maintainers: http://www.openedv.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/bcd.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/rtc.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/ap3216c.h>

#define  DEVICE_NAME            "ap3216c"

/* I2C Client Device Slave Address */
#define AP3216C_ADDR          0x1E

/*
 * ap3216c Register Address Definition
 */
#define AP3216C_SYSTEMCONG    0x00  /* Configuration register */
#define AP3216C_INTSTATUS     0x01  /* Interrupt status register */
#define AP3216C_INTCLEAR      0x02  /* Interrupt clear register */
#define AP3216C_IRDATALOW     0x0A  /* IR Data Low byte */
#define AP3216C_IRDATAHIGH    0x0B  /* IR Data High byte */
#define AP3216C_ALSDATALOW    0x0C  /* ALS Data Low byte */
#define AP3216C_ALSDATAHIGH   0x0D  /* ALS Data High byte */
#define AP3216C_PSDATALOW     0x0E  /* PS Data Low byte */
#define AP3216C_PSDATAHIGH    0x0F  /* PS Data High byte */

struct ap3216c {
    struct miscdevice misc_dev;
    struct i2c_client *client;   /**< i2c client device */
};

static int ap3216c_open(struct inode *, struct file *);
static int ap3216c_release(struct inode *, struct file *);
static long ap3216c_ioctl(struct file *, unsigned int, unsigned long);


static int ap3216c_read_reg(struct i2c_client *client, u8 reg, u8 *buf)
{
    struct i2c_msg msg[2] = {
        [0] = {
            .addr  = client->addr,
            .flags = 0,
            .len   = 1,
            .buf   = &reg,
        },

        [1] = {
            .addr  = client->addr,
            .flags = I2C_M_RD,
            .len   = 1,
            .buf   = buf,
        },
    };

    if (2 != i2c_transfer(client->adapter, msg, 2))
        return -1;

    return 0;
}

static int ap3216c_write_reg(struct i2c_client *client, u8 reg, u8 data)
{
    u8 buf[2] = {reg, data};

    struct i2c_msg msg = {
        .addr  = client->addr,
        .flags = 0,
        .len   = 2,
        .buf   = buf,
    };

    if (1 != i2c_transfer(client->adapter, &msg, 1))
        return -1;

    return 0;
}

static int ap3216c_read_ir_data(struct i2c_client *client, u16 *data)
{
    u8 low_data, high_data;

    if (ap3216c_read_reg(client, AP3216C_IRDATALOW, &low_data) ||
        ap3216c_read_reg(client, AP3216C_IRDATAHIGH, &high_data))
        return -1;

    if ((u32)low_data & 0x80U)
        return -1;
    else
        *data = ((u32)high_data << 2) | ((u32)low_data & 0x03U);

    return 0;
}

static int ap3216c_read_als_data(struct i2c_client *client, u16 *data)
{
    u8 low_data, high_data;

    if (ap3216c_read_reg(client, AP3216C_ALSDATALOW, &low_data) ||
        ap3216c_read_reg(client, AP3216C_ALSDATAHIGH, &high_data))
        return -1;

    *data = ((u32)high_data << 8) | (u32)low_data;

    return 0;
}

static int ap3216c_read_ps_data(struct i2c_client *client, u16 *data)
{
    u8 low_data, high_data;

    if (ap3216c_read_reg(client, AP3216C_PSDATALOW, &low_data) ||
        ap3216c_read_reg(client, AP3216C_PSDATAHIGH, &high_data))
        return -1;

    if ((u32)low_data & 0x40U)
        return -1;
    else
        *data = (((u32)high_data & 0x3FU) << 4) | ((u32)low_data & 0x0FU);

    return 0;
}

static int ap3216c_init(struct i2c_client *client)
{
    /* Software reset sensor */
    if (ap3216c_write_reg(client, AP3216C_SYSTEMCONG, 0x04))
        return -1;

    mdelay(50);

    /* Enable IR, ALS and PS */
    if (ap3216c_write_reg(client, AP3216C_SYSTEMCONG, 0x03))
        return -1;

    return 0;
}

/*
 * Character device file_operations structure variable
 * open
 * release
 * unlocked_ioctl
 */
static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = ap3216c_open,
    .release        = ap3216c_release,
    .unlocked_ioctl = ap3216c_ioctl,
};


static ssize_t show_ir(struct device *dev,
                       struct device_attribute *attr, char *buf)
{
    u16 data;
    struct ap3216c *ap3216c = (struct ap3216c *)dev_get_drvdata(dev);

    if (ap3216c_read_ir_data(ap3216c->client, &data))
        return sprintf(buf, "%d\n", -1);

    return sprintf(buf, "%d\n", data);
}

static ssize_t show_als(struct device *dev,
                       struct device_attribute *attr, char *buf)
{
    u16 data;
    struct ap3216c *ap3216c = (struct ap3216c *)dev_get_drvdata(dev);

    if (ap3216c_read_als_data(ap3216c->client, &data))
        return sprintf(buf, "%d\n", -1);

    return sprintf(buf, "%d\n", data);
}

static ssize_t show_ps(struct device *dev,
                       struct device_attribute *attr, char *buf)
{
    u16 data;
    struct ap3216c *ap3216c = (struct ap3216c *)dev_get_drvdata(dev);

    if (ap3216c_read_ps_data(ap3216c->client, &data))
        return sprintf(buf, "%d\n", -1);

    return sprintf(buf, "%d\n", data);
}

static DEVICE_ATTR(ir, S_IRUGO, show_ir, NULL);
static DEVICE_ATTR(als, S_IRUGO, show_als, NULL);
static DEVICE_ATTR(ps, S_IRUGO, show_ps, NULL);

static struct attribute *ap3216c_attrs[] = {
    &dev_attr_ir.attr,
    &dev_attr_als.attr,
    &dev_attr_ps.attr,
    NULL
};

static const struct attribute_group ap3216c_attrs_group = {
    .attrs = ap3216c_attrs,
};

static const struct attribute_group *ap3216c_attr_groups[] = {
    &ap3216c_attrs_group,
    NULL
};

static struct ap3216c ap3216c = {
    .misc_dev = {
        .name   = DEVICE_NAME,
        .minor  = MISC_DYNAMIC_MINOR,
        .fops   = &fops,
        .groups = ap3216c_attr_groups,
    },

    .client = NULL,
};


static int ap3216c_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int ap3216c_release(struct inode *inode, struct file *file)
{
    return 0;
}

static long ap3216c_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    u16 data;
    int ret = 0;

    switch (cmd) {

    case AP3216C_GET_IR:
        ret = ap3216c_read_ir_data(ap3216c.client, &data);
        break;

    case AP3216C_GET_ALS:
        ret = ap3216c_read_als_data(ap3216c.client, &data);
        break;

    case AP3216C_GET_PS:
        ret = ap3216c_read_ps_data(ap3216c.client, &data);
        break;

    default:
        ret = -1;
        break;
    };

    if (copy_to_user((void *)arg, &data, sizeof(data)))
        return -1;

    return ret;
}


static int ap3216c_probe(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
    /* Initialize the ap3216c sensor */
    if (ap3216c_init(client)) {
        dev_err(&client->dev, "ap3216c init failed\n");
        return -1;
    }

    /* Register as a misc device */
    misc_register(&ap3216c.misc_dev);

    /* Save data */
    ap3216c.client = client;
    dev_set_drvdata(ap3216c.misc_dev.this_device, (void *)&ap3216c);

    return 0;
}

static int ap3216c_remove(struct i2c_client *client)
{
    misc_deregister(&ap3216c.misc_dev);
    return 0;
}

static const struct i2c_device_id ap3216c_id[] = {
    { DEVICE_NAME, 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, ap3216c_id);

#ifdef CONFIG_OF
static const struct of_device_id ap3216c_of_match[] = {
    { .compatible = "LiteOn,ap3216c", },
    {  }
};
MODULE_DEVICE_TABLE(of, ap3216c_of_match);
#endif

static struct i2c_driver ap3216c_driver = {
    .driver = {
        .owner          = THIS_MODULE,
        .name           = DEVICE_NAME,
        .of_match_table = of_match_ptr(ap3216c_of_match),
    },
    .id_table = ap3216c_id,
    .probe    = ap3216c_probe,
    .remove   = ap3216c_remove,
};

module_i2c_driver(ap3216c_driver);

MODULE_AUTHOR("Deng Tao <773904075@qq.com>");
MODULE_DESCRIPTION("Lite-On Three-in-one Environmental Sensor ap3216c Driver");
MODULE_LICENSE("GPL");
