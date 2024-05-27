#include <linux/completion.h>
#include <linux/err.h> 
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("Completions example");

static struct completion crank_comp;
static struct completion flywheel_comp;

int machine_crank_thread(void *arg){
  pr_info("Turn the crank\n");
  complete_all(&crank_comp);                      /*The crank thread uses the complete_all() function to update the completion, which lets the flywheel thread continue. So even though flywheel_thread  */
  kthread_complete_and_exit(&crank_comp, 0);      /*is started first you should notice when you load this module and run dmesg, that turning the crank always happens first because the flywheel thread waits for the crank thread to complete. */
}

int machine_flywheel_spinup_thread(void *arg){
  wait_for_completion(&crank_comp);
  pr_info("Flywheel spins up\n");
  complete_all(&flywheel_comp);
  kthread_complete_and_exit(&flywheel_comp, 0);
}

static int __init completions_init(void){
  pr_info("completions example\n");
  struct task_struct *crank_thread;
  struct task_struct *flywheel_thread;
  init_completion(&crank_comp);
  init_completion(&flywheel_comp);
  crank_thread = kthread_create(machine_crank_thread, NULL, "KThread Crank");
  if (IS_ERR(crank_thread))
    goto ERROR_THREAD_1;
  flywheel_thread = kthread_create(machine_flywheel_spinup_thread, NULL, "KThread Flywheel");
  if (IS_ERR(flywheel_thread))
    goto ERROR_THREAD_2;
  wake_up_process(flywheel_thread);
  wake_up_process(crank_thread);
  return 0;
ERROR_THREAD_2:
  kthread_stop(crank_thread);
ERROR_THREAD_1:
  return -1;
}

static void __exit completions_exit(void){
  wait_for_completion(&crank_comp);
  wait_for_completion(&flywheel_comp);
  pr_info("completions exit\n");
}



module_init(completions_init);
module_exit(completions_exit);

