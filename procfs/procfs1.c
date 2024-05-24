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

static ssize_t procfile_read(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset){
  

static int __init ModuleInit(void){
}

static void __exit ModuleExit(void){
}

module_init(ModuleInit);
module_exit(ModuleExit);
