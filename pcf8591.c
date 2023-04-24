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
#include <linux/kthread.h>             //kernel threads
#include <linux/sched.h>               //task_struct 

MODULE_LICENSE("GPL");

#define SAMPLE_PERIOD_MS    (100)

#define I2C_BUS             (1)             // I2C Bus available in our Raspberry Pi
#define SLAVE_NAME          ("pcf8591")     // Device and Driver Name
#define SLAVE_ADDRESS       (0x48)

static struct task_struct *etx_thread;

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

int pcf8591_thread(void *pv)
{
    while(!kthread_should_stop()) 
    {
        if (true == b_flag_i2c_probe)
        {
            pr_info("pcf8591_thread\n");

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

        msleep(SAMPLE_PERIOD_MS);
    }
    return 0;
}
 
/**
* \brief Kernel module init function
*/
static int __init pcf8591_init(void)
{
    pr_info("pcf8591 init\n");
    
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

    etx_thread = kthread_create(pcf8591_thread,NULL,"eTx Thread");
    if(etx_thread) 
    {
        wake_up_process(etx_thread);
    } 
    else 
    {
        pr_err("Cannot create kthread\n");
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
    // del_timer(&etx_timer);
    kthread_stop(etx_thread);
}
 
module_init(pcf8591_init);
module_exit(pcf8591_exit);
