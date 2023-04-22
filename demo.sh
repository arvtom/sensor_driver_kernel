sudo dmesg -C

echo 'print kernel messages when hello_kernel is not yet inserted'
sudo dmesg

sudo insmod pcf8591.ko

echo 'print kernel messages when hello_kernel is inserted'
sudo dmesg

echo 'print active kernel modules containing keyword hello'
lsmod | grep "pcf8591"

sudo rmmod pcf8591

echo 'print kernel messages when hello_kernel is removed'
sudo dmesg
