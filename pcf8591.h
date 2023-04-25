/**
*  \file    pcf8591.h
*  \brief   I2C driver (kernel module) for external ADC PCF8591.
*  \details Tested with rpi4b, kernel version 5.15.0-1027-raspi. 
*   Implemented according to: 
*   https://github.com/Embetronicx/Tutorials/tree/master/Linux/Device_Driver/I2C-Linux-Device-Driver/I2C-Client-Driver
*  \author  arvtom
*/

/* kernel headers can be found in /lib/modules/5.15.0-1027-raspi/build/include */

/*------------------------------Includes------------------------------*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/kthread.h>             //kernel threads
#include <linux/sched.h>               //task_struct 

/*------------------------------Defines------------------------------*/
#define SAMPLE_PERIOD_MS_DEFAULT    (500)

#define I2C_BUS                     (1)             // I2C Bus available in our Raspberry Pi
#define SLAVE_NAME                  ("pcf8591")     // Device and Driver Name
#define SLAVE_ADDRESS               (0x48)

/*------------------------------Public function prototypes------------------------------*/
static int __init pcf8591_init(void);
static void __exit pcf8591_exit(void);

/*------------------------------Private function prototypes------------------------------*/
int pcf8591_thread(void *pv);
static int pcf8591_probe_callback(struct i2c_client *client, const struct i2c_device_id *id);
static int pcf8591_remove_callback(struct i2c_client *client);
