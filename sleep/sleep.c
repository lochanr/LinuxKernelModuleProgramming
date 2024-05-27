#include <linux/atomic.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h> 
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <linux/uaccess.h> 
#include <linux/version.h>
#include <linux/wait.h> 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("create a /proc file, and if several processes try to open it at the same time, put all but one to sleep.");

#define MESSAGE_LENGTH 80
static char message[MESSAGE_LENGTH];

static struct proc_dir_entry *our_proc_folder;
#define PROC_ENTRY_FOLDERNAME "sleep_dir"
static struct proc_dir_entry *our_proc_file;
#define PROC_ENTRY_FILENAME "sleep"

static ssize_t module_output(struct file *file, char __user *buf, size_t len, loff_t *offset){
  static int finished = 0;
  int i;
  char output_msg[MESSAGE_LENGTH + 30];
  if (finished) {                                                    /* Return 0 to signify end of file - that we have nothing more to say at this point.*/ 
    finished = 0;
    return 0;
  }
  sprintf(output_msg, "Last input:%s\n", message);
  for (i = 0; i < len && output_msg[i]; i++)
    put_user(output_msg[i], buf + i);
  finished = 1;
  return i;                                                          /* Return the number of bytes "read" */
}

static ssize_t module_input(struct file *file, char __user *buf, size_t length, loff_t *offset){
  int i;
  for (i = 0; i < MESSAGE_LENGTH - 1 && i < length; i++)
    get_user(message[i], buf + i);
  message[i] = '\0';
  return i;
}

static atomic_t already_open = ATOMIC_INIT(0);
static DECLARE_WAIT_QUEUE_HEAD(waitq);

static int module_open(struct inode *inode, struct file *file){
  if ((file->f_flags & O_NONBLOCK) && atomic_read(&already_open))   /* If the file's flags include O_NONBLOCK, it means the process does not want to wait for the file. In this case, if the file is already open, */
    return -EAGAIN;                                                 /* we should fail with -EAGAIN, meaning "you will have to try again", instead of blocking a process which would rather stay awake. */
  try_module_get(THIS_MODULE);                                      /* try_module_get increments the module's usage count to prevent the module from being unloaded while it is in use. */
  while (atomic_cmpxchg(&already_open, 0, 1)) {                     /* If already_open is 0, it is set to 1, and the loop exits. If already_open is already 1, the function enters a loop. */
    int i, is_sig = 0;
    wait_event_interruptible(waitq, !atomic_read(&already_open));   /* wait_event_interruptible puts the process to sleep until already_open becomes 0. This function allows the process to be interrupted by signals.*/

  for (i = 0; i < _NSIG_WORDS && !is_sig; i++)
    is_sig = current->pending.signal.sig[i] & ~current->blocked.sig[i]; /*The function checks if the process was awakened due to a signal(such as Ctrl+C). If a signal is pending, the function returns -EINTR and releases */
  if (is_sig) {                                                         /*the module reference with module_put.*/
    module_put(THIS_MODULE);
    return -EINTR;
  }
}                                                                   

static int module_close(struct inode *inode, struct file *file){
  atomic_set(&already_open, 0);
  wake_up(&waitq);
  module_put(THIS_MODULE);
  return 0;
}

static const struct proc_ops file_ops_4_our_proc_file = {
  .proc_read = module_output,                                       /* "read" from the file */
  .proc_write = module_input,                                       /* "write" to the file */
  .proc_open = module_open,                                         /* called when the /proc file is opened */
  .proc_release = module_close,                                     /* called when it's closed */
  .proc_lseek = noop_llseek,                                        /* return file->f_pos */
};

static int __init sleep_init(void){
  our_proc_folder = proc_mkdir(PROC_ENTRY_FOLDERNAME, NULL);
  if (our_proc_folder == NULL) {
    pr_debug("Error: Could not initialize /proc/%s\n", PROC_ENTRY_FOLDERNAME);
    return -ENOMEM;
    }
  
  our_proc_file = proc_create(PROC_ENTRY_FILENAME, 0644, NULL, &file_ops_4_our_proc_file);
  if (our_proc_file == NULL) {
    pr_debug("Error: Could not initialize /proc/%s/%s\n", PROC_ENTRY_FOLDERNAME, PROC_ENTRY_FILENAME);
    return -ENOMEM;
    }
  
  proc_set_size(our_proc_file, 80);                                /*This line sets the size of the proc file to 80 bytes.*/
  proc_set_user(our_proc_file, GLOBAL_ROOT_UID, GLOBAL_ROOT_GID);  /*This line sets the ownership of the proc file to the root user and root group. */

  pr_info("/proc/%s/%s created\n", PROC_ENTRY_FOLDERNAME, PROC_ENTRY_FILENAME);
  return 0;
}

static void __exit sleep_exit(void){
  remove_proc_entry(PROC_ENTRY_FILENAME, PROC_ENTRY_FOLDERNAME);    /*void proc_remove_entry(const char *name, struct proc_dir_entry *parent);*/
  proc_remove(our_proc_folder);                                    /*void proc_remove(struct proc_dir_entry *de);*/
  pr_debug("/proc/%s removed\n", PROC_ENTRY_FILENAME);
}

module_init(sleep_init);
module_exit(sleep_exit);

