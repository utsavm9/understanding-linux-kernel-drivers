// Debug output available at dmesg --follow. Load/Unload using insmod hello.ko
// and rmmod hello.ko Print with extra info from pr_info. More useful than
// printk
#define pr_fmt(fmt) "%s.ko: %s(): " fmt "\n", KBUILD_MODNAME, __func__

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
MODULE_LICENSE("Copyright Utsav");

#define MINOR_BASE 3
#define MINOR_COUNT 2

struct chard_dev {
    dev_t dev;
    struct cdev cdev;

    // Assume this is atomic and only store one character
    // written to this char device across all /dev/chard* nodes.
    char data;
};
static struct chard_dev m_chard_dev;

static int chard_open(struct inode *inode, struct file *file) {
    pr_info("Open() called for i_rdev: major: %u, minor: %u",
        MAJOR(inode->i_rdev),
        MINOR(inode->i_rdev));

    // This is preferred instead of using the global variable.
    struct chard_dev *chard_dev = container_of(inode->i_cdev, struct chard_dev, cdev);
    pr_info("container_of() returned chard_dev: 0x%p", chard_dev);
    pr_info("Global m_chard_dev: 0x%p", &m_chard_dev);

    // File also points to the inode
    pr_info("File->f_inode: 0x%p", file->f_inode);
    pr_info("inode: 0x%p", inode);

    // Driver specific data can be stored here.
    file->private_data = chard_dev;
    return 0;
}

static ssize_t chard_read(struct file *file, char __user *user, size_t len, loff_t *offset) {
    pr_info("Read() called for major: %u, minor: %u, len: %zu, offset: %lld",
        MAJOR(file->f_inode->i_rdev),
        MINOR(file->f_inode->i_rdev), len, *offset);

    struct chard_dev *chard_dev = file->private_data;
    if (chard_dev->data == 0) {
        return 0;
    }

    size_t copy_len = len < sizeof(chard_dev->data) ? len : sizeof(chard_dev->data);
    unsigned long uncopied = copy_to_user(user, &chard_dev->data, copy_len);
    pr_info("Data: %c", chard_dev->data);
    chard_dev->data = 0;

    pr_info("Uncopied: %lu", uncopied);
    return copy_len - uncopied;
}

static ssize_t chard_write(struct file *file, const char __user *user, size_t len, loff_t *offset) {
    pr_info("Write() called for major: %u, minor: %u, len: %zu, offset: %lld",
        MAJOR(file->f_inode->i_rdev),
        MINOR(file->f_inode->i_rdev), len, *offset);

    struct chard_dev *chard_dev = file->private_data;
    size_t copy_len = len < sizeof(chard_dev->data) ? len : sizeof(chard_dev->data);
    unsigned long uncopied = copy_from_user(&chard_dev->data, user, copy_len);

    // Returning 0 as default can result in userspace app retrying infinitely.
    pr_info("Uncopied: %lu", uncopied);
    return len;
}

static int chard_flush(struct file *file, fl_owner_t id) {
    pr_info("Flush() called for major: %u, minor: %u",
        MAJOR(file->f_inode->i_rdev),
        MINOR(file->f_inode->i_rdev));
    return 0;
}

static int chard_release(struct inode *inode, struct file *file) {
    pr_info("Release() called for major: %u, minor: %u",
        MAJOR(inode->i_rdev),
        MINOR(inode->i_rdev));
    // When a file structure is shared, like after fork() or dup(),
    // release() won't be invoked until all copies are closed.
    // flush() is invoked when user closes it's copy of the file.
    return 0;
}

static const struct file_operations m_fops = {
    .owner = THIS_MODULE,
    .read = chard_read,
    .write = chard_write,
    .open = chard_open,
    .flush = chard_flush,
    .release = chard_release,
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
