// Debug output available at dmesg --follow. Load/Unload using insmod hello.ko
// and rmmod hello.ko Print with extra info from pr_info. More useful than
// printk
#define pr_fmt(fmt) "%s.ko: %s(): " fmt "\n", KBUILD_MODNAME, __func__

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Copyright Utsav");

#define MINOR_BASE 3
#define MINOR_COUNT 2

struct chard_dev {
    dev_t dev;
    struct cdev cdev;
};
static struct chard_dev m_chard_dev;

static int chard_open(struct inode *inode, struct file *file) {
    pr_info("Open called");

    struct chard_dev *chard_dev = container_of(inode->i_cdev, struct chard_dev, cdev);
    // file->private_data = chard_dev;

    pr_info("Open() called for cdev->dev major: %u, minor: %u",
        MAJOR(chard_dev->dev),
        MINOR(chard_dev->dev));
    return 0;
}

static ssize_t chard_read(struct file *read_file, char __user *user, size_t len, loff_t *offset) {
    pr_info("Read called");
    return 0;
}

static const struct file_operations m_fops = {
    .owner = THIS_MODULE,
    .read = chard_read,
    .open = chard_open,
};


static int __init chard_init(void) {
    int result = 0;

    pr_info("Entering");

    // Register device
    // In UNIX, devices traditionally have a unique fixed itentifier.
    // Major identifies the device type (input, sound, usb) and minor identified the device itself.
    // In general, a driver has a major number associated to it and handles for all minors associated to the major.
    const char *name = "chard";
    result = alloc_chrdev_region(&m_chard_dev.dev, MINOR_BASE, MINOR_COUNT, name);
    if (result < 0) {
        pr_err("Could not register char device number, err: %d", result);
        return result;
    }
    pr_info("Registered char device with major: %u, minor: %u to %u", MAJOR(m_chard_dev.dev), MINOR(m_chard_dev.dev),
            MINOR(m_chard_dev.dev) + MINOR_COUNT - 1);

    // Register char device
    cdev_init(&m_chard_dev.cdev, &m_fops);
    m_chard_dev.cdev.owner = THIS_MODULE;

    // With this call, the char device goes live immediately when it returns.
    result = cdev_add(&m_chard_dev.cdev, m_chard_dev.dev, MINOR_COUNT);
    if (result < 0) {
        pr_err("Could not add char device, err: %d", result);
        return result;
    }

    return 0;
}

static void __exit chard_exit(void) {
    unregister_chrdev_region(m_chard_dev.dev, MINOR_COUNT);
    pr_info("Exiting");
}

module_init(chard_init);
module_exit(chard_exit);
