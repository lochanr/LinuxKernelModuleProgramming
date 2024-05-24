#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LochanR");
MODULE_DESCRIPTION("Creating a folder and a file in sysfs");

static struct kobject *dummy_kobj;

static ssize_t dummy_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer) {
	return sprintf(buf, "%d\n", myvariable);
}

static ssize_t dummy_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count) {
	sscanf(buf, "%du", &myvariable);
  return count;
}

static struct kobj_attribute dummy_attr = __ATTR(dummy, 0660, dummy_show, dummy_store);

static int __init my_init(void){
  int error = 0;
  pr_info("mymodule: initialised\n");
  dummy_kobj = kobject_create_and_add("mymodule", kernel_kobj);
  if (!dummy_kobj)
    return -ENOMEM;
  error = sysfs_create_file(dummy_kobj, &kobj_attribute.attr);
  if (error) {
    pr_info("failed to create the myvariable file in /sys/kernel/mymodule\n");
  }
  return error;
}

static void __exit my_exit(void) {
  pr_info("mymodule: Exit success\n");
  sysfs_remove_file(dummy_kobj, dummy_attribute.attr);
  kobject_put(mymodule);
}

module_init(my_init);
module_exit(my_exit);
