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

MODULE_LICENSE("GPL");
 
#define I2C_BUS             (1)             // I2C Bus available in our Raspberry Pi
#define SLAVE_NAME          ("pcf8591")     // Device and Driver Name
#define SLAVE_ADDRESS       (0x48)
 
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
static int slave_found_callback(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
    
    unsigned char buf_rx_i2c = 0x00;
    unsigned char buf_tx_i2c = 0xAF;
    int ret = 0;
    
    //perhaps delay will be needed for i2c peripheral or pcf8591 to settle
    msleep(100);

    ret = i2c_master_recv(s_i2c_client, &buf_rx_i2c, 1);
    ret = i2c_master_send(s_i2c_client, &buf_tx_i2c, 1);
    
    pr_info("OLED Probed!!!\n");
    
    return 0;
}
 
/**
* \brief Slave removed callback
*/
static int slave_removed_callback(struct i2c_client *client)
{   
    pr_info("OLED Removed!!!\n");
    
    return 0;
}

static struct i2c_driver s_i2c_driver = 
{
    .driver = 
    {
        .name   = SLAVE_NAME,
        .owner  = THIS_MODULE,
    },
    .probe          = slave_found_callback,
    .remove         = slave_removed_callback,
    .id_table       = s_i2c_device_id,
};
 
/**
* \brief Kernel module init function
*/
static int __init etx_driver_init(void)
{
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
    
    pr_info("Driver Added!!!\n");

    return ret;
}
 
/**
* \brief Kernel module exit function
*/
static void __exit etx_driver_exit(void)
{
    i2c_unregister_device(s_i2c_client);
    i2c_del_driver(&s_i2c_driver);
    pr_info("Driver Removed!!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
