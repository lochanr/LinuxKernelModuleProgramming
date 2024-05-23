#include <linux/init.h>
#include <linux/kernel.h> 
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("USING PARAMETERS IN LKM");

static short int myint = 1;
static char *mystring = "lion";
static int myarray[2] = { 420, 420 };
static int myarrayargc = 0;

module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myint, "A integer");
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");
module_param_array(myarray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintarray, "An array of integers");


static int __init ModuleInit(void){
  pr_info("Hello World :) \n");
  pr_info("myint is an integer: %d\n", myint);
  pr_info("mystring is a string: %s\n", mystring);
  for (i = 0; i < ARRAY_SIZE(myarray); i++)
    pr_info("myarray[%d] = %d\n", i, myarray[i]);
  pr_info("got %d arguments for myintarray.\n", arr_argc);
  return 0;
}

static void __exit ModuleExit(void){
  pr_info("Goodbye World :( \n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
