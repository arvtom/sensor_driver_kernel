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
** Module Init function
*/
static int __init my_init(void)
{
    
    etx_i2c_adapter     = i2c_get_adapter(I2C_BUS_AVAILABLE);
    etx_i2c_client_oled = i2c_new_client_device(etx_i2c_adapter, &oled_i2c_board_info);

    return 0;
}
module_init(my_init);

/*
static int __init my_init(void)
{
    struct i2c_adapter *s = i2c_get_adapter(1);
    //printk("0x%x\n", &s);
    
    printk("Hello Kernel\n");
    
    return 0;
}

static void __exit my_deinit(void)
{
    printk("Goodbye Kernel\n");
}

module_init(my_init);
module_exit(my_deinit);*/
