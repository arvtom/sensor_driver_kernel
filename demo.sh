#clear kernel logs
sudo dmesg -C

#insert kernel module
sudo insmod pcf8591.ko

#pause for 5s
read -t 5

#remove kernel module
sudo rmmod pcf8591

#print logs
sudo dmesg
