/**
*  \file    pcf8591.c
*  \brief   I2C driver (kernel module) for external ADC PCF8591.
*  \details Tested with rpi4b, kernel version 5.15.0-1027-raspi. 
*   Implemented according to: 
*   https://github.com/Embetronicx/Tutorials/tree/master/Linux/Device_Driver/I2C-Linux-Device-Driver/I2C-Client-Driver
*  \author  arvtom
*/

/* kernel headers can be found in /lib/modules/5.15.0-1027-raspi/build/include */

/*------------------------------Includes------------------------------*/
#include "pcf8591.h"

/*------------------------------Defines------------------------------*/

/*------------------------------Variables / Macro calls------------------------------*/
pcf8591_error_t err = 0U;

MODULE_LICENSE("GPL");

int sample_period_ms = 500;
module_param(sample_period_ms, int, S_IRUGO);
MODULE_PARM_DESC(sample_period_ms, "PCF8591 sample period");

static struct task_struct *s_task_struct;

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

/*------------------------------Public functions------------------------------*/
/**
* \brief Kernel module init function
*/
static int __init pcf8591_init(void)
{
    printk("pcf8591 init");
    printk("pcf8591 sample_period_ms= %d ", sample_period_ms);
    
    int ret = 0;

    s_i2c_adapter = i2c_get_adapter(I2C_BUS);
    if (NULL == s_i2c_adapter)
    {
        error_manager_set_u32(&err, PCF8591_ERROR_NULL_PTR_ADAPTER);
        printk("err= 0x%x", err);
    }

    s_i2c_client = i2c_new_client_device(s_i2c_adapter, &s_i2c_board_info);
    if (NULL == s_i2c_client)
    {
        error_manager_set_u32(&err, PCF8591_ERROR_NULL_PTR_CLIENT);
        printk("err= 0x%x", err);
    }

    i2c_add_driver(&s_i2c_driver);
    i2c_put_adapter(s_i2c_adapter);

    s_task_struct = kthread_create(pcf8591_thread,NULL,"pcf8591_thread");
    if(NULL == s_task_struct)
    {
        error_manager_set_u32(&err, PCF8591_ERROR_NULL_PTR_TASK);
        printk("err= 0x%x", err);
    } 

    if(1 != wake_up_process(s_task_struct))
    {
        error_manager_set_u32(&err, PCF8591_ERROR_WAKE_UP);
        printk("err= 0x%x", err);
    }

    return ret;
}
 
/**
* \brief Kernel module exit function
*/
static void __exit pcf8591_exit(void)
{
    printk("pcf8591 exit");
    
    i2c_unregister_device(s_i2c_client);
    i2c_del_driver(&s_i2c_driver);
    kthread_stop(s_task_struct);
}
 
module_init(pcf8591_init);
module_exit(pcf8591_exit);

/*------------------------------Private functions------------------------------*/
/**
* \brief Periodic tasks of driver
*/
int pcf8591_thread(void *pv)
{
    while(!kthread_should_stop()) 
    {
        if (true == b_flag_i2c_probe)
        {
            printk("pcf8591_thread");

            unsigned char buf_rx_i2c[4];
            memset(&buf_rx_i2c, 0, sizeof(buf_rx_i2c));
            unsigned char buf_tx_i2c = 0b01000000; /* PCF8591 control register value. ADC enabled, four single ended channels. */
            int ret = 0;
            unsigned char i = 0;

            /* Read only ch2 to get higher sample rate */
            // for (; i < 4; i++)
            // {
                
                /* Choose ch2 in control register */
                buf_tx_i2c += 2U;
                ret = i2c_master_send(s_i2c_client, &buf_tx_i2c, 1);

                /* Read two data samples because pcf8591 has to settle */
                ret = i2c_master_recv(s_i2c_client, &buf_rx_i2c[i], 1);
                ret = i2c_master_recv(s_i2c_client, &buf_rx_i2c[i], 1);

                /* Increment control register to read next channel */
                // buf_tx_i2c++;
            // }

            // printk("ch0=%x, ch1=%x, ch2=%x, ch3=%x\n",
            //     buf_rx_i2c[0], buf_rx_i2c[1], buf_rx_i2c[2], buf_rx_i2c[3]);
            printk("pcf8591_value= 0x%x \n", buf_rx_i2c[0]);
        }

        msleep(sample_period_ms);
    }
    return 0;
}
 
/**
* \brief Slave found callback
*/
static int pcf8591_probe_callback(struct i2c_client *client, const struct i2c_device_id *id)
{
    printk("pcf8591 probe callback");
    
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
    printk("pcf8591 remove callback");
    
    return 0;
}
