/*
 *atomic operations you can be confident that your addition, subtraction or bit
 *flip did actually happen and was not overwritten by some other shenanigans.
*/

#include <linux/atomic.h>
#include <linux/bitops.h>
#include <linux/module.h>
#include <linux/printk.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("Atomic operations example");

static void atomic_bitwise(void){
}

static int __init example_atomic_init(void){
  pr_info("example_atomic started\n");
}

static void __exit example_atomic_exit(void){
  pr_info("example_atomic exit\n");
}

module_init(example_atomic_init);
module_exit(example_atomic_exit);
