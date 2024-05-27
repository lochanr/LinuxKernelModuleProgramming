#include <linux/module.h>
#include <linux/printk.h>
#include <linux/rwlock.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("Read/Write locks example");

static DEFINE_RWLOCK(myrwlock);                  /* static initalization  -> DEFINE_RWLOCK(my_rwlock); */
rwlock_t my_rwlock;                              /* dynamic initalization -> rwlock_t my_rwlock;; */

static void example_read_lock(void){
  unsigned long flags;
  read_lock_irqsave(&myrwlock, flags);
  pr_info("Read Locked\n");
  read_unlock_irqrestore(&myrwlock, flags);
  pr_info("Read Unlocked\n");
}

static void example_write_lock(void){
  unsigned long flags;
  rwlock_init(&my_rwlock);
  write_lock_irqsave(&my_rwlock, flags);
  pr_info("Write Locked\n");
  write_unlock_irqrestore(&my_rwlock, flags);
  pr_info("Write Unlocked\n");
}
 
static int __init example_rwlock_init(void){
  pr_info("example_rwlock started\n");
  example_read_lock();
  example_write_lock();
  return 0;
}

static void __exit example_rwlock_exit(void){
  pr_info("example_rwlock exit\n");
}

module_init(example_rwlock_init);
module_exit(example_rwlock_exit);
