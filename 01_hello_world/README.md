# Hello World

### References:
* [Linux Device Drivers, Chapter 2: Building and Running Modules](https://lwn.net/Kernel/LDD3/)
* [Github Repo with examples](https://github.com/d0u9/Linux-Device-Driver/tree/master/eg_01_hello_world)

---

Compile with just `make` and `hello.ko` kernel object gets built.

Load the driver with `sudo insmod hello.ko`. See this output in `sudo dmesg --follow`:
```
[195555.978512] hello.ko: hello_init(): Entering
[195555.978523] hello.ko: hello_init(): Loaded from PID: 199856, command: insmod
```

Unload the driver with `sudo rmmod hello.ko`.
```
[195562.087897] hello.ko: hello_exit(): Exiting
```

When loaded, module also shows up in:
```
❯ lsmod
Module                  Size  Used by
hello                  12288  0
```
