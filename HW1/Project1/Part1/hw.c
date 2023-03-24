#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");

static int __init my_module_init(void)
{
    printk(KERN_INFO "My module loaded\n");
    printk(KERN_INFO "System uptime is %lu seconds\n", jiffies/HZ);
    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "My module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);