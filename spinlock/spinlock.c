#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/spinlock.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("Spinlock example");

static DEFINE_SPINLOCK(sl_static);                    /*format for static  -> DEFINE_SPINLOCK(my_lock); */ 
static spinlock_t sl_dynamic;                         /*format for dynamic -> spinlock_t my_lock; */

static void example_spinlock_static(void){
  unsigned long flags;
  spin_lock_irqsave(&sl_static, flags);               /*Acquires the spinlock and disables local interrupts to prevent deadlock scenarios where an interrupt handler could try to acquire the same lock, saving the current interrupt state in flags.*/
  pr_info("Locked static spinlock\n");
  spin_unlock_irqrestore(&sl_static, flags);          /*Releases the spinlock and restores the previous interrupt state from flags. */
  pr_info("Unlocked static spinlock\n");
}to prevent deadlock scenarios where an interrupt handler could try to acquire the same lock.

static void example_spinlock_dynamic(void){
  unsigned long flags;
  spin_lock_init(&sl_dynamic);
  spin_lock_irqsave(&sl_dynamic, flags);
  pr_info("Locked dynamic spinlock\n");
  spin_unlock_irqrestore(&sl_static, flags);
  pr_info("Unlocked static spinlock\n");
}

static int __init example_spinlock_init(void){
  pr_info("example spinlock started\n");
  return 0;
}

static void __exit example_spinlock_exit(void){
  pr_info("example spinlock exit\n");
}

module_init(example_spinlock_init);
module_exit(example_spinlock_exit);
