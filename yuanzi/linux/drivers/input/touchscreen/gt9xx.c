/* 
 * Goodix GT9xx touchscreen driver
 *
 * Copyright  (C)  2010 - 2014 Goodix. Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be a reference
 * to you, when you are integrating the GOODiX's CTP IC into your system,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * Base version 2.4
 * Release Date: 2021/11/27
 */
#include <linux/irq.h>
#include "gt9xx.h"
#include <asm/unaligned.h>

#include <linux/input/mt.h>

#define GTP_CONFIG_OF 1
static const char *goodix_ts_name = "goodix-ts";
static const char *goodix_input_phys = "input/ts";
static struct workqueue_struct *goodix_wq;
struct i2c_client * i2c_connect_client = NULL; 
int gtp_rst_gpio;
int gtp_int_gpio;
u8 config[GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH];

static s8 gtp_i2c_test(struct i2c_client *client, struct goodix_ts_data *ts);
void gtp_reset_guitar(struct i2c_client *client, s32 ms);
void gtp_int_sync(s32 ms);
static ssize_t gt91xx_config_read_proc(struct file *, char __user *, size_t, loff_t *);
static ssize_t gt91xx_config_write_proc(struct file *, const char __user *, size_t, loff_t *);

static struct proc_dir_entry *gt91xx_config_proc = NULL;
static const struct file_operations config_proc_ops = {
    .owner = THIS_MODULE,
    .read = gt91xx_config_read_proc,
    .write = gt91xx_config_write_proc,
};

int gtp_parse_dt_cfg(struct device *dev, u8 *cfg, int *cfg_len, u8 sid);


static const struct goodix_chip_data gt1x_chip_data = {
    .config_addr    = GTP_GT1X_REG_CONFIG_DATA,
    .config_len     = GTP_CONFIG_MAX_LENGTH,
};

static const struct goodix_chip_data gt9x_chip_data = {
    .config_addr    = GTP_GT9X_REG_CONFIG_DATA,
    .config_len     = GTP_CONFIG_MAX_LENGTH,
};

static const struct goodix_chip_data *goodix_get_chip_data(u16 id)
{
    switch (id) {
    case 1151:
    case 1158:
    case 5663:
    case 5688:
        config[0] = GTP_GT1X_REG_CONFIG_DATA >> 8;
        config[1] = GTP_GT1X_REG_CONFIG_DATA & 0xff;
        return &gt1x_chip_data;
    default:
        config[0] = GTP_GT9X_REG_CONFIG_DATA >> 8;
        config[1] = GTP_GT9X_REG_CONFIG_DATA & 0xff;
        return &gt9x_chip_data;
    }
}

static int goodix_i2c_read(struct i2c_client *client,
                           u16 reg, u8 *buf, int len)
{
    struct i2c_msg msgs[2];
    __be16 wbuf = cpu_to_be16(reg);
    int ret;

    msgs[0].flags = 0;
    msgs[0].addr  = client->addr;
    msgs[0].len   = 2;
    msgs[0].buf   = (u8 *)&wbuf;

    msgs[1].flags = I2C_M_RD;
    msgs[1].addr  = client->addr;
    msgs[1].len   = len;
    msgs[1].buf   = buf;

    ret = i2c_transfer(client->adapter, msgs, 2);
    return ret < 0 ? ret : (ret != ARRAY_SIZE(msgs) ? -EIO : 0);
}

s32 gtp_i2c_read(struct i2c_client *client, u8 *buf, s32 len)
{
    struct i2c_msg msgs[2];
    s32 ret=-1;
    s32 retries = 0;

    GTP_DEBUG_FUNC();

    msgs[0].flags = !I2C_M_RD;
    msgs[0].addr  = client->addr;
    msgs[0].len   = GTP_ADDR_LENGTH;
    msgs[0].buf   = &buf[0];
    
    msgs[1].flags = I2C_M_RD;
    msgs[1].addr  = client->addr;
    msgs[1].len   = len - GTP_ADDR_LENGTH;
    msgs[1].buf   = &buf[GTP_ADDR_LENGTH];

    while (retries < 5) {
        ret = i2c_transfer(client->adapter, msgs, 2);
        if(ret == 2)break;
        retries++;
    }
    if ((retries >= 5)) {
        GTP_ERROR("I2C Read: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((u16)(buf[0] << 8)) | buf[1]), len-2, ret);
        gtp_reset_guitar(client, 10);
    }
    return ret;
}

s32 gtp_i2c_write(struct i2c_client *client,u8 *buf,s32 len)
{
    struct i2c_msg msg;
    s32 ret = -1;
    s32 retries = 0;

    GTP_DEBUG_FUNC();

    msg.flags = !I2C_M_RD;
    msg.addr  = client->addr;
    msg.len   = len;
    msg.buf   = buf;

    while (retries < 5) {
        ret = i2c_transfer(client->adapter, &msg, 1);
        if (ret == 1)break;
        retries++;
    }
    if ((retries >= 5)) {
        GTP_ERROR("I2C Write: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((u16)(buf[0] << 8)) | buf[1]), len-2, ret);
        gtp_reset_guitar(client, 10);
    }
    return ret;
}

s32 gtp_i2c_read_dbl_check(struct i2c_client *client, u16 addr, u8 *rxbuf, int len)
{
    u8 buf[16] = {0};
    u8 confirm_buf[16] = {0};
    u8 retry = 0;
    
    while (retry++ < 3) {
        memset(buf, 0xAA, 16);
        buf[0] = (u8)(addr >> 8);
        buf[1] = (u8)(addr & 0xFF);
        gtp_i2c_read(client, buf, len + 2);
        
        memset(confirm_buf, 0xAB, 16);
        confirm_buf[0] = (u8)(addr >> 8);
        confirm_buf[1] = (u8)(addr & 0xFF);
        gtp_i2c_read(client, confirm_buf, len + 2);
        
        if (!memcmp(buf, confirm_buf, len+2)) {
            memcpy(rxbuf, confirm_buf+2, len);
            return SUCCESS;
        }
    }
    GTP_ERROR("I2C read 0x%04X, %d bytes, double check failed!", addr, len);
    return FAIL;
}

void gtp_irq_disable(struct goodix_ts_data *ts)
{
    unsigned long irqflags;

    GTP_DEBUG_FUNC();

    spin_lock_irqsave(&ts->irq_lock, irqflags);
    if (!ts->irq_is_disable) {
        ts->irq_is_disable = 1;
        disable_irq_nosync(ts->client->irq);
    }
    spin_unlock_irqrestore(&ts->irq_lock, irqflags);
}

void gtp_irq_enable(struct goodix_ts_data *ts)
{
    unsigned long irqflags = 0;

    GTP_DEBUG_FUNC();
    
    spin_lock_irqsave(&ts->irq_lock, irqflags);
    if (ts->irq_is_disable) {
        enable_irq(ts->client->irq);
        ts->irq_is_disable = 0;
    }
    spin_unlock_irqrestore(&ts->irq_lock, irqflags);
}

static void gtp_touch_down(struct goodix_ts_data* ts,s32 id,s32 x,s32 y,s32 w)
{
    input_mt_slot(ts->input_dev, id);
    input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, id);
    input_report_abs(ts->input_dev, ABS_MT_POSITION_X, x);
    input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, y);
    input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, w);
    input_report_abs(ts->input_dev, ABS_MT_WIDTH_MAJOR, w);
}

static void gtp_touch_up(struct goodix_ts_data* ts, s32 id)
{
    input_mt_slot(ts->input_dev, id);
    input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, -1);
    GTP_DEBUG("Touch id[%2d] release!", id);
}

static void goodix_ts_work_func(struct work_struct *work)
{
    u8  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
    u8  point_data[2 + 1 + 8 * GTP_MAX_TOUCH + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
    u8  touch_num = 0;
    u8  finger = 0;
    static u16 pre_touch = 0;
    static u8 pre_key = 0;
    u8  key_value = 0;
    u8* coor_data = NULL;
    s32 input_x = 0;
    s32 input_y = 0;
    s32 input_w = 0;
    s32 id = 0;
    s32 i  = 0;
    s32 ret = -1;
    struct goodix_ts_data *ts = NULL;

    GTP_DEBUG_FUNC();
    ts = container_of(work, struct goodix_ts_data, work);
    if (ts->enter_update) {
        return;
    }

    ret = gtp_i2c_read(ts->client, point_data, 12);
    if (ret < 0) {
        GTP_ERROR("I2C transfer error. errno:%d\n ", ret);
        if (ts->use_irq) {
            gtp_irq_enable(ts);
        }
        return;
    }
    
    finger = point_data[GTP_ADDR_LENGTH];

    if (finger == 0x00) {
        if (ts->use_irq) {
            gtp_irq_enable(ts);
        }
        return;
    }

    if((finger & 0x80) == 0) {
        goto exit_work_func;
    }

    touch_num = finger & 0x0f;
    if (touch_num > GTP_MAX_TOUCH) {
        goto exit_work_func;
    }

    if (touch_num > 1) {
        u8 buf[8 * GTP_MAX_TOUCH] = {(GTP_READ_COOR_ADDR + 10) >> 8, (GTP_READ_COOR_ADDR + 10) & 0xff};

        ret = gtp_i2c_read(ts->client, buf, 2 + 8 * (touch_num - 1));
        memcpy(&point_data[12], &buf[2], 8 * (touch_num - 1));
    }

    pre_key = key_value;

    //GTP_DEBUG("pre_touch:%02x, finger:%02x.", pre_touch, finger);

    if (pre_touch || touch_num) {
        s32 pos = 0;
        u16 touch_index = 0;
        u8 report_num = 0;
        coor_data = &point_data[3];
        
        if(touch_num) {
            id = coor_data[pos] & 0x0F;
            touch_index |= (0x01<<id);
        }
        
        GTP_DEBUG("id = %d,touch_index = 0x%x, pre_touch = 0x%x\n",id, touch_index,pre_touch);
        for (i = 0; i < GTP_MAX_TOUCH; i++) {
            if ((touch_index & (0x01<<i))) {
                input_x  = coor_data[pos + 1] | (coor_data[pos + 2] << 8);
                input_y  = coor_data[pos + 3] | (coor_data[pos + 4] << 8);
                input_w  = coor_data[pos + 5] | (coor_data[pos + 6] << 8);

                gtp_touch_down(ts, id, input_x, input_y, input_w);
                pre_touch |= 0x01 << i;
                
                report_num++;
                if (report_num < touch_num) {
                    pos += 8;
                    id = coor_data[pos] & 0x0F;
                    touch_index |= (0x01<<id);
                }
            } else {
                gtp_touch_up(ts, i);
                pre_touch &= ~(0x01 << i);
            }
        }
    }

    input_sync(ts->input_dev);

exit_work_func:
    if (!ts->gtp_rawdiff_mode) {
        ret = gtp_i2c_write(ts->client, end_cmd, 3);
        if (ret < 0) {
            GTP_INFO("I2C write end_cmd error!");
        }
    }
    if (ts->use_irq) {
        gtp_irq_enable(ts);
    }
}

static enum hrtimer_restart goodix_ts_timer_handler(struct hrtimer *timer)
{
    struct goodix_ts_data *ts = container_of(timer, struct goodix_ts_data, timer);

    GTP_DEBUG_FUNC();

    queue_work(goodix_wq, &ts->work);
    hrtimer_start(&ts->timer, ktime_set(0, (GTP_POLL_TIME+6)*1000000), HRTIMER_MODE_REL);
    return HRTIMER_NORESTART;
}

static irqreturn_t goodix_ts_irq_handler(int irq, void *dev_id)
{
    struct goodix_ts_data *ts = dev_id;

    GTP_DEBUG_FUNC();

    gtp_irq_disable(ts);

    queue_work(goodix_wq, &ts->work);
    
    return IRQ_HANDLED;
}

void gtp_int_sync(s32 ms)
{
    GTP_GPIO_OUTPUT(gtp_int_gpio, 0);
    msleep(ms);
    GTP_GPIO_AS_INT(gtp_int_gpio);
}


void gtp_reset_guitar(struct i2c_client *client, s32 ms)
{
    GTP_DEBUG_FUNC();
    GTP_INFO("Guitar reset");
    GTP_GPIO_OUTPUT(gtp_rst_gpio, 0);   // begin select I2C slave addr
    msleep(ms);                         // T2: > 10ms
    // HIGH: 0x28/0x29, LOW: 0xBA/0xBB
    GTP_GPIO_OUTPUT(gtp_int_gpio, client->addr == 0x14);
    msleep(2);                          // T3: > 100us
    GTP_GPIO_OUTPUT(gtp_rst_gpio, 1);
    msleep(6);                          // T4: > 5ms
    GTP_GPIO_AS_INPUT(gtp_rst_gpio);    // end select I2C slave addr
    GTP_GPIO_OUTPUT(gtp_rst_gpio, 1);   // pull up rst_gpio
    gtp_int_sync(50);
}

static s32 gtp_init_panel(struct goodix_ts_data *ts)
{
    s32 ret = -1;

    ts->gtp_cfg_len = GTP_CONFIG_MAX_LENGTH;
    ret = gtp_i2c_read(ts->client, config, ts->gtp_cfg_len + GTP_ADDR_LENGTH);
    if (ret < 0) {
        GTP_ERROR("Read Config Failed, Using Default Resolution & INT Trigger!");
        ts->abs_x_max = GTP_MAX_WIDTH;
        ts->abs_y_max = GTP_MAX_HEIGHT;
        ts->int_trigger_type = GTP_INT_TRIGGER;
    }

    if ((ts->abs_x_max == 0) && (ts->abs_y_max == 0)) {
        ts->abs_x_max = (config[RESOLUTION_LOC + 1] << 8) + config[RESOLUTION_LOC];
        ts->abs_y_max = (config[RESOLUTION_LOC + 3] << 8) + config[RESOLUTION_LOC + 2];
        ts->int_trigger_type = (config[TRIGGER_LOC]) & 0x03;
    }

    GTP_INFO("X_MAX: %d, Y_MAX: %d, TRIGGER: 0x%02x", ts->abs_x_max,ts->abs_y_max,ts->int_trigger_type);

    msleep(10);
    return 0;

}

static ssize_t gt91xx_config_read_proc(struct file *file, char __user *page, size_t size, loff_t *ppos)
{
    char *ptr = page;
    char temp_data[GTP_CONFIG_MAX_LENGTH + 2] = {0x80, 0x47};
    int i;
    if (*ppos) {
        return 0;
    }
    ptr += sprintf(ptr, "==== GT9XX config init value====\n");

    for (i = 0 ; i < GTP_CONFIG_MAX_LENGTH ; i++) {
        ptr += sprintf(ptr, "0x%02X ", config[i + 2]);

        if (i % 8 == 7)
            ptr += sprintf(ptr, "\n");
    }

    ptr += sprintf(ptr, "\n");

    ptr += sprintf(ptr, "==== GT9XX config real value====\n");
    gtp_i2c_read(i2c_connect_client, temp_data, GTP_CONFIG_MAX_LENGTH + 2);
    for (i = 0 ; i < GTP_CONFIG_MAX_LENGTH ; i++) {
        ptr += sprintf(ptr, "0x%02X ", temp_data[i+2]);

        if (i % 8 == 7)
            ptr += sprintf(ptr, "\n");
    }

    *ppos += ptr - page;

    return (ptr - page);
}

static ssize_t gt91xx_config_write_proc(struct file *filp, const char __user *buffer, size_t count, loff_t *off)
{
    GTP_DEBUG("write count %d\n", count);

    if (count > GTP_CONFIG_MAX_LENGTH) {
        GTP_ERROR("size not match [%d:%d]\n", GTP_CONFIG_MAX_LENGTH, count);
        return -EFAULT;
    }

    if (copy_from_user(&config[2], buffer, count)) {
        GTP_ERROR("copy from user fail\n");
        return -EFAULT;
    }

    return count;
}

s32 gtp_read_version(struct goodix_ts_data *ts)
{
    int error;
    u8 buf[6];
    char id_str[5];

    error = goodix_i2c_read(ts->client, GTP_REG_VERSION, buf, sizeof(buf));
    if (error) {
        dev_err(&ts->client->dev, "read version failed: %d\n", error);
        return error;
    }

    memcpy(id_str, buf, 4);
    id_str[4] = 0;
    if (kstrtou16(id_str, 10, &ts->id))
        ts->id = 0x1001;

    ts->version = get_unaligned_le16(&buf[4]);

    dev_info(&ts->client->dev, "ID %d, version: %04x\n", ts->id,
             ts->version);

    return 0;
}

static s8 gtp_i2c_test(struct i2c_client *client, struct goodix_ts_data *ts)
{
    u8 test[3] = {ts->chip->config_addr >> 8, ts->chip->config_addr & 0xff};
    u8 retry = 0;
    s8 ret = -1;

    GTP_DEBUG_FUNC();

    while(retry++ < 5) {
        ret = gtp_i2c_read(client, test, 3);
        if (ret > 0) {
            return ret;
        }
        GTP_ERROR("GTP i2c test failed time %d.",retry);
        msleep(10);
    }
    return ret;
}

static s8 gtp_request_io_port(struct goodix_ts_data *ts)
{
    s32 ret = 0;
    GTP_DEBUG_FUNC();
    ret = devm_gpio_request_one(&ts->client->dev, gtp_rst_gpio,
                                GPIOF_OUT_INIT_HIGH, "Goodix-Ts reset PIN");
    if (ret < 0) {
        GTP_ERROR("Failed to request GPIO:%d, ERRNO:%d", (s32)gtp_int_gpio, ret);
        ret = -ENODEV;
	return ret;
    } else {
        GTP_GPIO_AS_INT(gtp_int_gpio);
        ts->client->irq = gpio_to_irq(gtp_int_gpio);
    }
    ret = devm_gpio_request_one(&ts->client->dev, gtp_int_gpio,
                                GPIOF_OUT_INIT_HIGH, "Goodix-Ts IRQ PIN");
    if (ret < 0) {
        GTP_ERROR("Failed to request GPIO:%d, ERRNO:%d",(s32)gtp_rst_gpio, ret);
        ret = -ENODEV;
	return ret;
    }

    GTP_GPIO_AS_INPUT(gtp_rst_gpio);
    gtp_reset_guitar(ts->client, 20);
    
    return ret;
}

static s8 gtp_request_irq(struct goodix_ts_data *ts)
{
    s32 ret = -1;
    const u8 irq_table[] = GTP_IRQ_TAB;

    GTP_DEBUG_FUNC();
    GTP_DEBUG("INT trigger type:%x", ts->int_trigger_type);

    ret  = request_irq(ts->client->irq,
                       goodix_ts_irq_handler,
                       irq_table[ts->int_trigger_type],
            ts->client->name,
            ts);
    if (ret) {
        GTP_ERROR("Request IRQ failed!ERRNO:%d.", ret);
        GTP_GPIO_AS_INPUT(gtp_int_gpio);

        hrtimer_init(&ts->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        ts->timer.function = goodix_ts_timer_handler;
        hrtimer_start(&ts->timer, ktime_set(1, 0), HRTIMER_MODE_REL);
        return -1;
    } else {
        gtp_irq_disable(ts);
        ts->use_irq = 1;
        return 0;
    }
}

static s8 gtp_request_input_dev(struct goodix_ts_data *ts)
{
    s8 ret = -1;

    GTP_DEBUG_FUNC();

    ts->input_dev = input_allocate_device();
    if (ts->input_dev == NULL) {
        GTP_ERROR("Failed to allocate input device.");
        return -ENOMEM;
    }

    ts->input_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) ;
    input_mt_init_slots(ts->input_dev, GTP_MAX_TOUCH, 1);     // in case of "out of memory"
    __set_bit(INPUT_PROP_DIRECT, ts->input_dev->propbit);

    input_set_abs_params(ts->input_dev, ABS_MT_POSITION_X, 0, ts->abs_x_max, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_MT_POSITION_Y, 0, ts->abs_y_max, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_MT_WIDTH_MAJOR, 0, 255, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
    input_set_abs_params(ts->input_dev, ABS_MT_TRACKING_ID, 0, 255, 0, 0);

    ts->input_dev->name = goodix_ts_name;
    ts->input_dev->phys = goodix_input_phys;
    ts->input_dev->id.bustype = BUS_I2C;
    ts->input_dev->id.vendor = 0xDEAD;
    ts->input_dev->id.product = 0xBEEF;
    ts->input_dev->id.version = 10427;
    
    ret = input_register_device(ts->input_dev);
    if (ret) {
        GTP_ERROR("Register %s input device failed", ts->input_dev->name);
        return -ENODEV;
    }

    return 0;
}

static int gtp_parse_dt(struct device *dev)
{
    struct device_node *np = dev->of_node;

    gtp_int_gpio = of_get_named_gpio(np, "goodix,irq-gpio", 0);
    if (!gpio_is_valid(gtp_int_gpio)) {
        dev_err(dev, "Failed to get reset gpio\n");
        return -1;
    }

    gtp_rst_gpio = of_get_named_gpio(np, "goodix,rst-gpio", 0);
    if (!gpio_is_valid(gtp_rst_gpio)) {
        dev_err(dev, "Failed to get reset gpio\n");
        return -1;
    }

    return 0;
}

int gtp_parse_dt_cfg(struct device *dev, u8 *cfg, int *cfg_len, u8 sid)
{
    struct device_node *np = dev->of_node;
    struct property *prop;
    char cfg_name[18];

    snprintf(cfg_name, sizeof(cfg_name), "goodix,cfg-group%d", sid);
    prop = of_find_property(np, cfg_name, cfg_len);
    if (!prop || !prop->value || *cfg_len == 0 || *cfg_len > GTP_CONFIG_MAX_LENGTH) {
        return -1;
    } else {
        memcpy(cfg, prop->value, *cfg_len);
        return 0;
    }
}

static int gtp_power_switch(struct i2c_client *client, int on)
{
    static struct regulator *vdd_ana;
    static struct regulator *vcc_i2c;
    int ret;

    if (!vdd_ana) {
        vdd_ana = regulator_get(&client->dev, "vdd_ana");
        if (IS_ERR(vdd_ana)) {
            GTP_ERROR("regulator get of vdd_ana failed");
            ret = PTR_ERR(vdd_ana);
            vdd_ana = NULL;
            return ret;
        }
    }

    if (!vcc_i2c) {
        vcc_i2c = regulator_get(&client->dev, "vcc_i2c");
        if (IS_ERR(vcc_i2c)) {
            GTP_ERROR("regulator get of vcc_i2c failed");
            ret = PTR_ERR(vcc_i2c);
            vcc_i2c = NULL;
            goto ERR_GET_VCC;
        }
    }

    if (on) {
        GTP_DEBUG("GTP power on.");
        ret = regulator_enable(vdd_ana);
        udelay(2);
        ret = regulator_enable(vcc_i2c);
    } else {
        GTP_DEBUG("GTP power off.");
        ret = regulator_disable(vcc_i2c);
        udelay(2);
        ret = regulator_disable(vdd_ana);
    }
    return ret;

ERR_GET_VCC:
    regulator_put(vdd_ana);
    return ret;
}

static int goodix_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    s32 ret = -1;
    struct goodix_ts_data *ts;
    
    GTP_DEBUG_FUNC();
    
    //do NOT remove these logs
    GTP_INFO("GTP Driver Version: %s", GTP_DRIVER_VERSION);
    GTP_INFO("GTP I2C Address: 0x%02x", client->addr);

    i2c_connect_client = client;

    ts = kzalloc(sizeof(*ts), GFP_KERNEL);
    if (ts == NULL) {
        GTP_ERROR("Alloc GFP_KERNEL memory failed.");
        return -ENOMEM;
    }

    ts->client = client;

    if (client->dev.of_node) {
        ret = gtp_parse_dt(&client->dev);
	if (ret < 0)
	    goto free_ts;
    }

    ret = gtp_request_io_port(ts);
    if (ret < 0) {
        GTP_ERROR("GTP request IO port failed.");
        goto free_ts;
    }

    if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
        GTP_ERROR("I2C check functionality failed.");
        return -ENODEV;
    }

    ret = gtp_power_switch(client, 1);
    if (ret) {
        GTP_ERROR("GTP power on failed.");
        return -EINVAL;
    }

    INIT_WORK(&ts->work, goodix_ts_work_func);
    spin_lock_init(&ts->irq_lock);          // 2.6.39 later
    // ts->irq_lock = SPIN_LOCK_UNLOCKED;   // 2.6.39 & before
    i2c_set_clientdata(client, ts);
    ts->gtp_rawdiff_mode = 0;

    ret = gtp_read_version(ts);
    if (ret < 0) {
        GTP_ERROR("Read version failed.");
    }

    ts->chip = goodix_get_chip_data(ts->id);

    ret = gtp_i2c_test(client, ts);
    if (ret < 0) {
        GTP_ERROR("I2C communication ERROR!");
        goto free_ts;
    }
    
    ret = gtp_init_panel(ts);
    if (ret < 0) {
        GTP_ERROR("GTP init panel failed.");
        ts->abs_x_max = GTP_MAX_WIDTH;
        ts->abs_y_max = GTP_MAX_HEIGHT;
        ts->int_trigger_type = GTP_INT_TRIGGER;
        goto free_ts;
    }
    
    // Create proc file system
    gt91xx_config_proc = proc_create(GT91XX_CONFIG_PROC_FILE, 0666, NULL, &config_proc_ops);
    if (gt91xx_config_proc == NULL) {
        GTP_ERROR("create_proc_entry %s failed\n", GT91XX_CONFIG_PROC_FILE);
    } else {
        GTP_INFO("create proc entry %s success", GT91XX_CONFIG_PROC_FILE);
    }

    ret = gtp_request_input_dev(ts);
    if (ret < 0) {
        GTP_ERROR("GTP request input dev failed");
    }
    
    ret = gtp_request_irq(ts);
    if (ret < 0) {
        GTP_INFO("GTP works in polling mode.");
    } else {
        GTP_INFO("GTP works in interrupt mode.");
    }

    if (ts->use_irq) {
        gtp_irq_enable(ts);
    }

    return 0;
free_ts:
    kfree(ts);
    return ret;
}

static int goodix_ts_remove(struct i2c_client *client)
{
    struct goodix_ts_data *ts = i2c_get_clientdata(client);
    
    GTP_DEBUG_FUNC();

    if (ts) {
        if (ts->use_irq) {
            GTP_GPIO_AS_INPUT(gtp_int_gpio);
            free_irq(client->irq, ts);
        } else {
            hrtimer_cancel(&ts->timer);
        }
    }
    
    GTP_INFO("GTP driver removing...");
    i2c_set_clientdata(client, NULL);
    input_unregister_device(ts->input_dev);
    kfree(ts);

    return 0;
}

static int __maybe_unused goodix_ts_suspend(struct device *dev)
{
    struct i2c_client *client = to_i2c_client(dev);

    if (device_may_wakeup(dev))
        enable_irq_wake(client->irq);

    return 0;
}

static int __maybe_unused goodix_ts_resume(struct device *dev)
{
    struct i2c_client *client = to_i2c_client(dev);

    if (device_may_wakeup(dev))
        disable_irq_wake(client->irq);

    return 0;
}


static SIMPLE_DEV_PM_OPS(goodix_ts_pm_ops,
                         goodix_ts_suspend, goodix_ts_resume);

static const struct of_device_id goodix_match_table[] = {
{.compatible = "goodix,gt9xx",},
{.compatible = "goodix,gt1151",},
{.compatible = "goodix,gt9147",},
{.compatible = "goodix,gt9271",},
{ },
};

static const struct i2c_device_id goodix_ts_id[] = {
{ GTP_I2C_NAME, 0 },
{ }
};

static struct i2c_driver goodix_ts_driver = {
    .probe      = goodix_ts_probe,
    .remove     = goodix_ts_remove,
    .id_table   = goodix_ts_id,
    .driver = {
        .name     = GTP_I2C_NAME,
        .owner    = THIS_MODULE,
        .of_match_table = goodix_match_table,
        .pm             = &goodix_ts_pm_ops,
    },
};

static int __init goodix_ts_init(void)
{
    s32 ret;

    GTP_DEBUG_FUNC();
    GTP_INFO("GTP driver installing...");
    goodix_wq = create_singlethread_workqueue("goodix_wq");
    if (!goodix_wq) {
        GTP_ERROR("Creat workqueue failed.");
        return -ENOMEM;
    }
    ret = i2c_add_driver(&goodix_ts_driver);
    return ret;
}

static void __exit goodix_ts_exit(void)
{
    GTP_DEBUG_FUNC();
    GTP_INFO("GTP driver exited.");
    i2c_del_driver(&goodix_ts_driver);
    if (goodix_wq) {
        destroy_workqueue(goodix_wq);
    }
}

module_init(goodix_ts_init);
module_exit(goodix_ts_exit);

MODULE_DESCRIPTION("GTP Series Driver");
MODULE_LICENSE("GPL");
