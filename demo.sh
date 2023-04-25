#init text files
echo "bash script result" > demo.txt
echo " " >> demo.txt

echo "pcf8591 driver result" > pcf8591.txt
echo " " >> pcf8591.txt

echo "python analysis result" > analysis.txt
echo " " >> analysis.txt

#clear kernel logs
sudo dmesg -C >> demo.txt

max_i2c_clock=$(xxd /sys/class/i2c-adapter/i2c-1/of_node/clock-frequency | awk -F' ' '{print $2$3}')
printf "max_i2c_clock= %d Hz\n" $((16#$max_i2c_clock)) >> demo.txt

#insert kernel module
sudo insmod pcf8591_module.ko sample_period_ms=$2 >> demo.txt

#pause bash script (seconds)
read -t $1 >> demo.txt

#remove kernel module
sudo rmmod pcf8591_module >> demo.txt

#save driver logs
sudo dmesg >> pcf8591.txt

#call python to analyse driver performance
python3 analysis.py >> analysis.txt

#append new line to log files
echo " " >> demo.txt
echo " " >> pcf8591.txt
echo " " >> analysis.txt

#print driver result
cat pcf8591.txt >> demo.txt

#print analysis result
cat analysis.txt >> demo.txt

#print bash script result
cat demo.txt
