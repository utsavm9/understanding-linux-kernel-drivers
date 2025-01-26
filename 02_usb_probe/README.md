# Hello World

### References:
* [Linux Device Drivers, Chapter 13: USB Drivers](https://lwn.net/Kernel/LDD3/)
* [Kernel Docs: Writing USB Device Drivers](https://docs.kernel.org/driver-api/usb/writing_usb_driver.html)
* [Kernel Docs: Linux USB Host-Side APIs](https://docs.kernel.org/driver-api/usb/usb.html#c.usb_driver)

---

Output after `sudo insmod usb_probe.ko`:
```
[196542.768014] usb_probe.ko: usb_probe_init(): Registering within USB subsystem
[196542.861438] usb_probe.ko: example_probe(): Detected a device idVendor=0 idProduct=0
[196542.861449] usb_probe.ko: example_probe(): Not willing to actually manage this device.
[196542.954199] usb_probe.ko: example_probe(): Detected a device idVendor=0 idProduct=0
[196542.954207] usb_probe.ko: example_probe(): Not willing to actually manage this device.
[196542.954277] usbcore: registered new interface driver Utsav's Driver
```

Probe function is also called when insterting a USB-C YubiKey.
