#define pr_fmt(fmt) "%s.ko: %s(): " fmt "\n", KBUILD_MODNAME, __func__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
MODULE_LICENSE("GPL");

/*
❯ lsusb --verbose
    idVendor           0x413c Dell Computer Corp.
    idProduct          0x301a Dell MS116 Optical Mouse
*/

#define EXAMPLE_VENDOR_ID  0x413c
#define EXAMPLE_PRODUCT_ID 0x301a

static struct usb_device_id example_id_table[] = {
    { .driver_info = 42, /* Pick-up any USB Device */ },
    { }
};

MODULE_DEVICE_TABLE(usb, example_id_table);

static int example_probe(struct usb_interface *intf, const struct usb_device_id *id) {
    pr_info("Detected a device idVendor=%d idProduct=%d", id->idVendor, id->idProduct);
    pr_info("Not willing to actually manage this device.");
    return -ENODEV; // -ENODEV signals that this driver is unwilling to manage the device
}

static void example_disconnect(struct usb_interface *intf) {
    pr_info("Maybe device is disconnecting or USB driver is unloading");
}

static struct usb_driver example_driver = {
    .name = "Utsav's Driver",
    .probe = example_probe,
    .disconnect = example_disconnect,
    .id_table = example_id_table,
    .supports_autosuspend = 1,
};

static int __init usb_probe_init(void) {
    pr_info("Registering within USB subsystem");

    int result;
    result = usb_register(&example_driver);
    if (result < 0) {
        pr_err("usb_register() failed with %d", result);
        return -1;
    }

    return 0;
}

static void __exit usb_probe_exit(void) {
    pr_info("Deregistering from USB Subsystem");
    usb_deregister(&example_driver);
}

module_init(usb_probe_init);
module_exit(usb_probe_exit);
