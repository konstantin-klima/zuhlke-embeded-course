#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
static int __init hello_init(void){
        pr_alert("Test matf kernel driver!\n");
        return 0;
}



static void __exit hello_exit(void){
    pr_alert("Test driver out\n");
}

module_init(hello_init);
module_exit(hello_exit);


MODULE_DESCRIPTION("MATF module");
MODULE_AUTHOR("Konstantin Klima");
