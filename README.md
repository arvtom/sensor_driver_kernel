# sensor_driver_kernel
This kernel module is written to learn basics of embedded linux. It uses RPI 4B (Broadcom BCM2835 MCU) to read extermal 8bit ADC PCF8591 using I2C. It was chosen to read only one channel (ch2), to have higher sample rate. Result summary can be found in demo.txt. Driver logs can be observed in pcf8591.txt, driver performance analysis in analysis.txt, plot of ADC values in analysis.png.  
  
There is possibility to adjust driver sample rate and run time length by passing parameters to demo.sh. Clock speed for i2c was increased to 100 kHz by modifying CONFIG_I2C_BCM2708_BAUDRATE value in /boot/config-5.15.0-1027-raspi.  
  
Driver is tested using HW-390 Capacitive soil moisture sensor v2.0 with 3.3V power supply modification. Hand was shortly placed on sensor to simulate different ADC values.

# TODO
* Driver error management  
* Driver comments  
* Project description  
* Analyse effect of sample rate on sample rate jitter, execution time jitter, error of requested sample rate  
* Measure MCU load during driver runtime  

# To enable driver for 10s with 100ms sample period
```bash
sudo bash demo.sh 10 100
```

# Results
![alt text](https://github.com/arvtom/sensor_driver_kernel/blob/main/analysis.png?raw=true)
