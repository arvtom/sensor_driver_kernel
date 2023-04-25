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
#include "error_manager.h"

/*------------------------------Defines------------------------------*/
#define SAMPLE_PERIOD_MS_DEFAULT    (500)

#define I2C_BUS                     (1)             // I2C Bus available in our Raspberry Pi
#define SLAVE_NAME                  ("pcf8591")     // Device and Driver Name
#define SLAVE_ADDRESS               (0x48)

/*------------------------------Structures / enumerators------------------------------*/


/* 64bit error enumerator */
typedef enum
{
    PCF8591_ERROR_NULL_PTR_ADAPTER          = 0x00000001U,
    PCF8591_ERROR_NULL_PTR_CLIENT           = 0x00000002U,
    PCF8591_ERROR_NULL_PTR_TASK             = 0x00000004U,
    PCF8591_ERROR_WAKE_UP                   = 0x00000008U,
    // PCF8591_ERROR_BIT = 0x00000010U,
    // PCF8591_ERROR_BIT = 0x00000020U,
    // PCF8591_ERROR_BIT = 0x00000040U,
    // PCF8591_ERROR_BIT = 0x00000080U,
    // PCF8591_ERROR_BIT = 0x00000100U,
    // PCF8591_ERROR_BIT = 0x00000200U,
    // PCF8591_ERROR_BIT = 0x00000400U,
    // PCF8591_ERROR_BIT = 0x00000800U,
    // PCF8591_ERROR_BIT = 0x00001000U,
    // PCF8591_ERROR_BIT = 0x00002000U,
    // PCF8591_ERROR_BIT = 0x00004000U,
    // PCF8591_ERROR_BIT = 0x00008000U,
    // PCF8591_ERROR_BIT = 0x00010000U,
    // PCF8591_ERROR_BIT = 0x00020000U,
    // PCF8591_ERROR_BIT = 0x00040000U,
    // PCF8591_ERROR_BIT = 0x00080000U,
    // PCF8591_ERROR_BIT = 0x00100000U,
    // PCF8591_ERROR_BIT = 0x00200000U,
    // PCF8591_ERROR_BIT = 0x00400000U,
    // PCF8591_ERROR_BIT = 0x00800000U,
    // PCF8591_ERROR_BIT = 0x01000000U,
    // PCF8591_ERROR_BIT = 0x02000000U,
    // PCF8591_ERROR_BIT = 0x04000000U,
    // PCF8591_ERROR_BIT = 0x08000000U,
    // PCF8591_ERROR_BIT = 0x10000000U,
    // PCF8591_ERROR_BIT = 0x20000000U,
    // PCF8591_ERROR_BIT = 0x40000000U,
    // PCF8591_ERROR_BIT = 0x80000000U,
}pcf8591_error_t;

/*------------------------------Public function prototypes------------------------------*/
static int __init pcf8591_init(void);
static void __exit pcf8591_exit(void);

/*------------------------------Private function prototypes------------------------------*/
int pcf8591_thread(void *pv);
static int pcf8591_probe_callback(struct i2c_client *client, const struct i2c_device_id *id);
static int pcf8591_remove_callback(struct i2c_client *client);
