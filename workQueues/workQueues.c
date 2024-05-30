/* To add a task to the scheduler we can use a workqueue. The kernel then uses
 *the Completely Fair Scheduler (CFS) to execute work within the queue.
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("Workqueue example");

static struct workqueue_struct *queue = NULL;                                    /* A pointer to a workqueue structure that will be used to queue work items. */
static struct work_struct work;                                                  /* A work_struct object representing a work item to be processed by the workqueue. */

static void work_handler(struct work_struct *data){                              /* The function that will be executed by the workqueue. It simply logs a message indicating that the work handler function is running. */
  pr_info("work handler function.\n");
}

static int __init sched_init(void){
  queue = alloc_workqueue("HELLOWORLD", WQ_UNBOUND, 1);                          /*Allocates a workqueue named "HELLOWORLD". WQ_UNBOUND creates a workqueue that is not bound to any particular CPU, and 1 indicates */
                                                                                 /* the maximum number of active work items */
  INIT_WORK(&work, work_handler);                                                /*  Initializes the work_struct object work with the function work_handler. */
  queue_work(queue, &work);                                                      /* Queues the work item work to the workqueue queue.*/
  return 0;
}                                                                                

static void __exit sched_exit(void){
  destroy_workqueue(queue);                                                      /*Destroys the workqueue, ensuring that all work items in the queue are completed or canceled. */
}

module_init(sched_init);
module_exit(sched_exit);
