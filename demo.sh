#clear kernel logs
sudo dmesg -C

#insert kernel module
sudo insmod pcf8591.ko

#pause bash script (seconds)
read -t 1

#remove kernel module
sudo rmmod pcf8591

#print logs
sudo dmesg
