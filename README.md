# sensor_driver_kernel
Kernel module, which uses RPI 4B to read external ADC PCF8591 using I2C. At the moment only hello_kernel is printed to dmesg.

# usage
make all - builds .ko kernel object file from .c source file according to Makefile.
dmesg -C - clears buffered messages.
insmod hello_kernel.ko - inserts kernel module into kernel, which then prints into dmesg.
lsmod | grep "hello" - lists active kernel modules containing "hello".
rmmod hello_kernel - removes kernel module from kernel.
