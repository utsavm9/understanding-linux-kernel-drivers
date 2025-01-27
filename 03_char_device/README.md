# Hello World

### References:
* [Linux Device Drivers, Chapter 3: Char Devices](https://lwn.net/Kernel/LDD3/)
* [Github Repo with examples](https://github.com/d0u9/Linux-Device-Driver/tree/master/eg_03_scull_basic)
* [Operating Systems 2 Course - Char Device Lab](https://linux-kernel-labs.github.io/refs/heads/master/labs/device_drivers.html)

---

Compile with just `make` and `chard.ko` kernel object gets built.

Load the driver with `sudo insmod chard.ko`. See this output in `sudo dmesg --follow`:
```
[ 7204.668877] chard.ko: chard_init(): Entering
[ 7204.668892] chard.ko: chard_init(): Registered char device with major: 511, minor: 3 to 4
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
254 gpiochip
261 accel
511 chard
```

Hardcode the driver major number in `load_module.sh` and then use it:
```
❯ sudo ./load_module.sh reload
Unloading the module...
Loading the module...
```

Write to the char device like this:
```
❯ sudo echo abcdef > /dev/chard4

❯ sudo echo xyz > /dev/chard4
```
```
[ 8395.003970] chard.ko: chard_open(): Open() called for i_rdev: major: 511, minor: 4
[ 8395.003976] chard.ko: chard_open(): container_of() returned chard_dev: 0x000000000263ee65
[ 8395.003979] chard.ko: chard_open(): Global m_chard_dev: 0x000000000263ee65
[ 8395.003980] chard.ko: chard_open(): File->f_inode: 0x0000000049f55550
[ 8395.003981] chard.ko: chard_open(): inode: 0x0000000049f55550
[ 8395.003993] chard.ko: chard_flush(): Flush() called for major: 511, minor: 4
[ 8395.013998] chard.ko: chard_flush(): Flush() called for major: 511, minor: 4
[ 8395.058602] chard.ko: chard_flush(): Flush() called for major: 511, minor: 4
[ 8395.058620] chard.ko: chard_flush(): Flush() called for major: 511, minor: 4
[ 8395.058695] chard.ko: chard_flush(): Flush() called for major: 511, minor: 4
[ 8395.060944] chard.ko: chard_write(): Write() called for major: 511, minor: 4, len: 4, offset: 0
[ 8395.060962] chard.ko: chard_write(): Uncopied: 0
[ 8395.060969] chard.ko: chard_flush(): Flush() called for major: 511, minor: 4
[ 8395.062065] chard.ko: chard_flush(): Flush() called for major: 511, minor: 4
[ 8395.067270] chard.ko: chard_flush(): Flush() called for major: 511, minor: 4
[ 8395.067291] chard.ko: chard_flush(): Flush() called for major: 511, minor: 4
[ 8395.067322] chard.ko: chard_release(): Release() called for major: 511, minor: 4
```

And read from it like this:
```
❯ cat /dev/chard3
x
```
```
[ 8467.924453] chard.ko: chard_open(): Open() called for i_rdev: major: 511, minor: 3
[ 8467.924459] chard.ko: chard_open(): container_of() returned chard_dev: 0x000000000263ee65
[ 8467.924461] chard.ko: chard_open(): Global m_chard_dev: 0x000000000263ee65
[ 8467.924463] chard.ko: chard_open(): File->f_inode: 0x000000003171180f
[ 8467.924464] chard.ko: chard_open(): inode: 0x000000003171180f
[ 8467.924475] chard.ko: chard_read(): Read() called for major: 511, minor: 3, len: 262144, offset: 0
[ 8467.924480] chard.ko: chard_read(): Data: x
[ 8467.924481] chard.ko: chard_read(): Uncopied: 0
[ 8467.924489] chard.ko: chard_read(): Read() called for major: 511, minor: 3, len: 262144, offset: 0
[ 8467.924500] chard.ko: chard_flush(): Flush() called for major: 511, minor: 3
[ 8467.924502] chard.ko: chard_release(): Release() called for major: 511, minor: 3
```
