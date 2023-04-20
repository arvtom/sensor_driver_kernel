make all
sudo dmesg -C

echo 'print kernel messages when hello_kernel is not yet inserted'
sudo dmesg

sudo insmod hello_kernel.ko

echo 'print kernel messages when hello_kernel is inserted'
sudo dmesg

echo 'print active kernel modules containing keyword hello'
lsmod | grep "hello"

sudo rmmod hello_kernel

echo 'print kernel messages when hello_kernel is removed'
sudo dmesg
