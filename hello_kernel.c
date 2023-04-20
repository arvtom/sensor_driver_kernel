#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>

// Meta information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arvydas Tomkus");
MODULE_DESCRIPTION("Kernel print example");

#define I2C_BUS_AVAILABLE   (          1 )              // I2C Bus available in our Raspberry Pi
#define SLAVE_DEVICE_NAME   ( "ETX_OLED" )              // Device and Driver Name
#define SSD1306_SLAVE_ADDR  (       0x3C )              // SSD1306 OLED Slave Address

static struct i2c_adapter *etx_i2c_adapter     = NULL;  // I2C Adapter Structure
static struct i2c_client  *etx_i2c_client_oled = NULL;  // I2C Cient Structure (In our case it is OLED)

/*
** I2C Board Info strucutre
*/
static struct i2c_board_info oled_i2c_board_info = {
        I2C_BOARD_INFO(SLAVE_DEVICE_NAME, SSD1306_SLAVE_ADDR)
    };

/*
** This function getting called when the slave has been found
** Note : This will be called only once when we load the driver.
*/
static int etx_oled_probe(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
    
    
    return 0;
}

/*
** This function getting called when the slave has been removed
** Note : This will be called only once when we unload the driver.
*/
static int etx_oled_remove(struct i2c_client *client)
{   
    
    return 0;
}

/*
** Structure that has slave device id
*/
static const struct i2c_device_id etx_oled_id[] = {
        { SLAVE_DEVICE_NAME, 0 },
        { }
};
MODULE_DEVICE_TABLE(i2c, etx_oled_id);

/*
** I2C driver Structure that has to be added to linux
*/
static struct i2c_driver etx_oled_driver = {
        .driver = {
            .name   = SLAVE_DEVICE_NAME,
            .owner  = THIS_MODULE,
        },
        .probe          = etx_oled_probe,
        .remove         = etx_oled_remove,
        .id_table       = etx_oled_id,
};

/*
** Module Init function
*/
static int __init my_init(void)
{
    int ret0 = 0;
    int ret1 = 0;
    int ret2 = 0;   
    // unsigned char tx[1] = {0b01000000};
    // unsigned char rx1[1] = {0x00};
    // unsigned char rx2[1] = {0x00};
    unsigned char tx = 0b01000000;
    unsigned char rx1 = 0x00;
    unsigned char rx2 = 0x00;

    etx_i2c_adapter     = i2c_get_adapter(I2C_BUS_AVAILABLE);
    etx_i2c_client_oled = i2c_new_client_device(etx_i2c_adapter, &oled_i2c_board_info);
    i2c_add_driver(&etx_oled_driver);

    printk("Hello Kernel\n");

    ret0 = i2c_master_send(etx_i2c_client_oled, &tx, 1);
    ret1 = i2c_master_recv(etx_i2c_client_oled, &rx1, 1);
    ret2 = i2c_master_recv(etx_i2c_client_oled, &rx2, 1);

    printk("ret0=%d, tx=%d\n", ret0, tx);
    printk("ret1=%d, rx1=%d\n", ret1, rx1);
    printk("ret2=%d, rx2=%d\n", ret2, rx2);

    return 0;
}

static void __exit my_deinit(void)
{
    printk("Goodbye Kernel\n");
}

module_init(my_init);
module_exit(my_deinit);
