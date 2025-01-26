# Hello World

### References:
* [Linux Device Drivers, Chapter 3: Char Devices](https://lwn.net/Kernel/LDD3/)
* [Github Repo with examples](https://github.com/d0u9/Linux-Device-Driver/tree/master/eg_03_scull_basic)
* [Operating Systems 2 Course - Char Device Lab](https://linux-kernel-labs.github.io/refs/heads/master/labs/device_drivers.html)

---

Compile with just `make` and `chard.ko` kernel object gets built.

Load the driver with `sudo insmod chard.ko`. See this output in `sudo dmesg --follow`:
```
[1125619.761906] chard.ko: chard_init(): Entering
[1125619.761917] chard.ko: chard_init(): Registered char device with major:506, minor: 3 to 5
```

The driver has now been registered as a char device:
```
❯ cat /proc/devices
Character devices:
  1 mem
  4 /dev/vc/0
  4 tty
  4 ttyS
  5 /dev/tty
  5 /dev/console
  5 /dev/ptmx
  7 vcs
 10 misc
...
261 accel
506 chard
507 nvidia-uvm
```
