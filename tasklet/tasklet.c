/* Tasklets are a quick and easy way of scheduling a single function to be run for example,
 * when triggered from an interrupt 
*/

#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/printk.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("Tasklet example");

void task_func(unsigned long data){
  pr_info("Example tasklet starts\n");
  mdelay(5000);
  pr_info("Example tasklet ends\n");
}

DECLARE_TASKLET(my_tasklet,task_func,0)                            /*DECLARE_TASKLET(my_tasklet, my_tasklet_function, 0); */

static int __init tasklet_init(void){
  pr_info("tasklet example init\n");
  tasklet_schedule(&my_tasklet);                                   /*format tasklet_schedule(&my_tasklet); */
  mdelay(200);
  pr_info("Example tasklet init continues...\n");
  return 0;
}

static void __exit tasklet_exit(void){
  pr_info("tasklet example exit\n");
}

module_init(tasklet_init);
module_exit(tasklet_exit);
