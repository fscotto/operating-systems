#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h> // Module metadata
#include <linux/uaccess.h>

MODULE_AUTHOR("Fabio Scotto di Santolo");
MODULE_DESCRIPTION("List all tasks on the Linux system");
MODULE_LICENSE("GPL"); // Custom init and exit methods

static int __init custom_init(void) {
  printk(KERN_INFO "Hello world driver loaded.");
  kmal return 0;
}

static void __exit custom_exit(void) {
  printk(KERN_INFO "Goodbye my friend, I shall miss you dearly...");
}

module_init(custom_init);
module_exit(custom_exit);
