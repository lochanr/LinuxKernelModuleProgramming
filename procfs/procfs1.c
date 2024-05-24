#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("procfs example in LKM");

static struct proc_dir_entry *proc_folder;
static struct proc_dir_entry *proc_file;

static ssize_t read_proc(struct file *File, char __user *user_buffer, size_t count, loff_t *offs){
  char text[] = "Hello from a procfs file!\n";
	int to_copy, not_copied, delta;
	to_copy = min(count, sizeof(text));
	not_copied = copy_to_user(user_buffer, text, to_copy);
	delta = to_copy - not_copied;
	return delta;
}

static ssize_t write_proc(struct file *File, const char __user *user_buffer, size_t count, loff_t *offs){
  int to_copy, not_copied, delta;
  char text [255];
  memset(text, 0, sizeof(text));
	to_copy = min(count, sizeof(text));
	not_copied = copy_to_user(text, user_buffer, to_copy);
	delta = to_copy - not_copied;
	return delta;
}

static int open_proc(struct inode *inode, struct file *File){
  try_module_get(THIS_MODULE);
  return 0;
}

static int close_proc(struct inode *inode, struct file *File){
  module_put(THIS_MODULE);
  return 0;
}

static struct proc_ops fops = {
  .proc_read = read_proc,
  .proc_write = write_proc,
  .proc_open = open_proc,
  .proc_close = close_proc
};

static int __init ModuleInit(void){
  proc_folder = proc_mkdir("example", NULL);
  if(proc_folder == NULL) {
		printk("Error creating /proc/example\n");
		return -ENOMEM;
	}
  proc_file = proc_create("dummy", 0666, proc_folder, &fops);
  if(proc_folder == NULL) {
		printk("Error creating /proc/example/dummy\n");
		return -ENOMEM;
	}
  printk("procfs_test - Created /proc/hello/dummy\n");
	return 0;
}

static void __exit ModuleExit(void){
  printk("Removing /proc/example/dummy\n");
	proc_remove(proc_file);
	proc_remove(proc_folder);
}

module_init(ModuleInit);
module_exit(ModuleExit);
