#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/printk.h>
#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/version.h>

MODULE_AUTHOR("LochanR");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Creates a read-only char device that says how many times you have read from the dev file");

#define DEVICE_NAME "chardev"
#define BUF_LEN 80
#define CDEV_NOT_USED  0,
#define CDEV_EXCLUSIVE_OPEN 1,

static char buffer[255];
static int buffer_pointer;
static int major;
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED); /* Is device open? Used to prevent multiple access to device */
static struct class *cls;

static int device_open(struct inode *device, struct file *instance){
  static int count;
  /*atomic_cmpxchg compares the value of already_open with CDEV_NOT_USED. If they are equal, 
  it sets already_open to CDEV_EXCLUSIVE_OPEN and returns the old value else if already open 
  (already_open is not CDEV_NOT_USED), returns -EBUSY, indicating that the device is busy and cannot be opened again.*/
  if(atmoic_cmpxchg(&already_open, CDEVNOT_USED, CDEV_EXCLUSIVE_OPEN))
    return -EBUSY;
  sprintf(msg, "I already told you %d times Hello world!\n", counter++);
  try_module_get(THIS_MODULE);
  return SUCCESS;
}
 
static int device_close(struct inode *device, struct file *instance){
  atomic_set(&already_open, CDEV_NOT_USED);
  module_put(THIS_MODULE);
}

static ssize_t device_read(struct file *File, char __user *user_buffer, size_t count, loff_t * offs){
  int to_copy, not_copied, delta;
  to_copy = min(count, buffer_pointer);
  not_copied = copy_to_user(user_buffer, buffer, to_copy);
  delta = to_copy - not_copied;
  return delta;
}
  
static ssize_t device_write(struct file *File, const char __user *user_buffer, size_t count, loff_t *offs){
  pr_alert("Sorry, this operation is not supported.\n");
  return -EINVAL;
}

static struct file_operations fops = {
  .open = driver_open,
  .read = driver_read,
  .write = driver_write,
  .release = driver_close,
};

static int __init ModuleInit(void){
  major = register_chrdev(0, DEVICE_NAME, &fops);
  if (major < 0) {
    pr_alert("Registering char device failed with %d\n", major);
    return major;
  }
  pr_info("Major number %d was assigned.\n", major);

  cls = class_create(DEVICE_NAME);
  device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
  pr_info("Device created on /dev/%s\n", DEVICE_NAME);
  return 0;
}

static void __exit ModuleExit(void){
  device_destroy(cls, MKDEV(major, 0));
  class_destroy(cls);
  unregister_chrdev(major, DEVICE_NAME);
}

module_init(ModuleInit);
module_exit(ModuleExit);
