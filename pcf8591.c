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
 
#define I2C_BUS_AVAILABLE   (1)             // I2C Bus available in our Raspberry Pi
#define SLAVE_DEVICE_NAME   ("pcf8591")     // Device and Driver Name
#define SSD1306_SLAVE_ADDR  (0x48)          // PCF8591 Slave Address
 
static struct i2c_adapter *etx_i2c_adapter     = NULL;  // I2C Adapter Structure
static struct i2c_client  *etx_i2c_client_oled = NULL;  // I2C Cient Structure 
static const struct i2c_device_id etx_oled_id[] = 
{
    { SLAVE_DEVICE_NAME, 0 },
    { }
};
static struct i2c_driver etx_oled_driver = 
{
    .driver = 
    {
        .name   = SLAVE_DEVICE_NAME,
        .owner  = THIS_MODULE,
    },
    .probe          = etx_oled_probe,
    .remove         = etx_oled_remove,
    .id_table       = etx_oled_id,
};
static struct i2c_board_info oled_i2c_board_info = 
{
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, SSD1306_SLAVE_ADDR)
};

MODULE_DEVICE_TABLE(i2c, etx_oled_id);

/*
** This function getting called when the slave has been found
** Note : This will be called only once when we load the driver.
*/
static int etx_oled_probe(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
    
    unsigned char buf_rx_i2c = 0x00;
    unsigned char buf_tx_i2c = 0xAF;
    int ret = 0;
    
    //perhaps delay will be needed for i2c peripheral or pcf8591 to settle
    msleep(100);

    ret = i2c_master_recv(etx_i2c_client_oled, &buf_rx_i2c, 1);
    ret = i2c_master_send(etx_i2c_client_oled, &buf_tx_i2c, 1);
    
    pr_info("OLED Probed!!!\n");
    
    return 0;
}
 
/*
** This function getting called when the slave has been removed
** Note : This will be called only once when we unload the driver.
*/
static int etx_oled_remove(struct i2c_client *client)
{   
    pr_info("OLED Removed!!!\n");
    
    return 0;
}
 
/*
** Module Init function
*/
static int __init etx_driver_init(void)
{
    int ret = -1;

    etx_i2c_adapter     = i2c_get_adapter(I2C_BUS_AVAILABLE);
    
    if( etx_i2c_adapter != NULL )
    {
        etx_i2c_client_oled = i2c_new_client_device(etx_i2c_adapter, &oled_i2c_board_info);
        
        if( etx_i2c_client_oled != NULL )
        {
            i2c_add_driver(&etx_oled_driver);
            ret = 0;
        }
        
        i2c_put_adapter(etx_i2c_adapter);
    }
    
    pr_info("Driver Added!!!\n");

    return ret;
}
 
/*
** Module Exit function
*/
static void __exit etx_driver_exit(void)
{
    i2c_unregister_device(etx_i2c_client_oled);
    i2c_del_driver(&etx_oled_driver);
    pr_info("Driver Removed!!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
