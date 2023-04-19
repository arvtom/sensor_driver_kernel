#include <linux/init.h>
#include <linux/module.h>

// Meta information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arvydas Tomkus");
MODULE_DESCRIPTION("Kernel print example");

static int __init my_init(void)
{
    printk("Hello Kernel\n");
    return 0;
}

static void __exit my_deinit(void)
{
    printk("Goodbye Kernel\n");
}

module_init(my_init);
module_exit(my_deinit);
