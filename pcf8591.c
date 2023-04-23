/**
*  \file    pcf8591.c
*  \brief   I2C driver (kernel module) for external ADC PCF8591.
*  \details Tested with rpi4b, kernel version 5.15.0-1027-raspi. 
*   Implemented according to: 
*   https://github.com/Embetronicx/Tutorials/tree/master/Linux/Device_Driver/I2C-Linux-Device-Driver/I2C-Client-Driver
*  \author  arvtom
*/

/* kernel headers can be found in /lib/modules/5.15.0-1027-raspi/build/include */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
 
#define TIMEOUT 1000    //milliseconds

#define I2C_BUS             (1)             // I2C Bus available in our Raspberry Pi
#define SLAVE_NAME          ("pcf8591")     // Device and Driver Name
#define SLAVE_ADDRESS       (0x48)

static struct timer_list etx_timer;
static unsigned int count = 0;

bool b_flag_i2c_probe = false;
static struct i2c_adapter *s_i2c_adapter = NULL;
static struct i2c_client  *s_i2c_client = NULL;
static const struct i2c_device_id s_i2c_device_id[] = 
{
    { SLAVE_NAME, 0 },
    { }
};
static struct i2c_board_info s_i2c_board_info = 
{
    I2C_BOARD_INFO(SLAVE_NAME, SLAVE_ADDRESS)
};

MODULE_DEVICE_TABLE(i2c, s_i2c_device_id);

/**
* \brief Slave found callback
*/
static int pcf8591_probe_callback(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
    pr_info("pcf8591 probe callback\n");
    
    //delay is needed for i2c peripheral
    msleep(100);
    b_flag_i2c_probe = true;

    return 0;
}
 
/**
* \brief Slave removed callback
*/
static int pcf8591_remove_callback(struct i2c_client *client)
{   
    pr_info("pcf8591 remove callback\n");
    
    return 0;
}

static struct i2c_driver s_i2c_driver = 
{
    .driver = 
    {
        .name   = SLAVE_NAME,
        .owner  = THIS_MODULE,
    },
    .probe          = pcf8591_probe_callback,
    .remove         = pcf8591_remove_callback,
    .id_table       = s_i2c_device_id,
};

//Timer Callback function. This will be called when timer expires
void timer_callback(struct timer_list * data)
{
    pr_info("Timer Callback function Called [%d]\n",count++);

    if (true == b_flag_i2c_probe)
    {
        unsigned char buf_rx_i2c = 0x00;
        unsigned char buf_tx_i2c = 0b01000000; /* PCF8591 control register value. ADC enabled, four single ended channels. */
        int ret = 0;

        pr_info("config pcf control register\n");
        ret = i2c_master_send(s_i2c_client, &buf_tx_i2c, 1);

        pr_info("read data sample\n");
        /* Read two data samples because pcf8591 has to settle */
        // ret = i2c_master_recv(s_i2c_client, &buf_rx_i2c, 1);
        // ret = i2c_master_recv(s_i2c_client, &buf_rx_i2c, 1);
    }
    
    /*
       Re-enable timer. Because this function will be called only first time. 
       If we re-enable this will work like periodic timer. 
    */
    mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}
 
/**
* \brief Kernel module init function
*/
static int __init pcf8591_init(void)
{
    pr_info("pcf8591 init\n");
    
    /* setup your timer to call my_timer_callback */
    timer_setup(&etx_timer, timer_callback, 0);       //If you face some issues and using older kernel version, then you can try setup_timer API(Change Callback function's argument to unsingned long instead of struct timer_list *.
 
    /* setup timer interval to based on TIMEOUT Macro */
    mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));
    
    int ret = -1;

    s_i2c_adapter = i2c_get_adapter(I2C_BUS);
    
    if (s_i2c_adapter != NULL)
    {
        s_i2c_client = i2c_new_client_device(s_i2c_adapter, &s_i2c_board_info);
        
        if (s_i2c_client != NULL )
        {
            i2c_add_driver(&s_i2c_driver);
            ret = 0;
        }
        
        i2c_put_adapter(s_i2c_adapter);
    }

    return ret;
}
 
/**
* \brief Kernel module exit function
*/
static void __exit pcf8591_exit(void)
{
    pr_info("pcf8591 exit\n");
    
    i2c_unregister_device(s_i2c_client);
    i2c_del_driver(&s_i2c_driver);
    del_timer(&etx_timer);
}
 
module_init(pcf8591_init);
module_exit(pcf8591_exit);
