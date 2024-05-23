#include <linux/module.h>
#include <linux/printk.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("Creating  a basic hello_world driver program");

static int __init ModuleInit(void){
  pr_info("Hello World:) \n");
  return 0;
}

static void __exit ModuleExit(void){
  pr_info("Goodbye World:( \n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
