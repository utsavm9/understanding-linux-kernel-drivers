// Debug output available at dmesg --follow. Load/Unload using insmod hello.ko and rmmod hello.ko
// Print with extra info from pr_info. More useful than printk
#define pr_fmt(fmt) "%s.ko: %s(): " fmt "\n", KBUILD_MODNAME, __func__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
MODULE_LICENSE("Copyright Utsav");

static int __init hello_init(void) {
    pr_info("Entering");
    pr_info("Loaded from PID: %d, command: %s", current->pid, current->comm);
    return 0;
}

static void __exit hello_exit(void) {
    pr_info("Exiting");
}

module_init(hello_init);
module_exit(hello_exit);
