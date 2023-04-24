#clear kernel logs
sudo dmesg -C > demo.txt

#insert kernel module
sudo insmod pcf8591.ko >> demo.txt

#pause bash script (seconds)
read -t $1 >> demo.txt

#remove kernel module
sudo rmmod pcf8591 >> demo.txt

#print logs
sudo dmesg >> demo.txt

#call python to analyse driver performance
python3 analysis.py >> demo.txt

#print demo result
cat demo.txt
