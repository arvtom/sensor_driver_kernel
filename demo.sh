sudo dmesg -C

echo 'print kernel messages when pcf8591 is not yet inserted'
sudo dmesg

sudo insmod pcf8591.ko

echo 'print kernel messages when pcf8591 is inserted'
sudo dmesg

echo 'print active kernel modules containing keyword hello'
lsmod | grep "pcf8591"

sudo rmmod pcf8591

echo 'print kernel messages when pcf8591 is removed'
sudo dmesg
