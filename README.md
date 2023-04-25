# sensor_driver_kernel
This kernel module is written to learn basics of embedded linux. It uses RPI 4B to read extermal ADC PCF8591 using I2C. Result summary can be found in demo.txt. Driver logs can be observed in pcf8591.txt, driver performance analysis in analysis.txt, plot of ADC values in analysis.png.  
  
There is possibility to adjust driver sample rate and run time length by passing parameters to demo.sh.
  
Driver is tested using HW-390 Capacitive soil moisture sensor v2.0 with 3.3V power supply modification. Hand was shortly placed on sensor to simulate different ADC values.

# TODO
*Driver error management  
*Driver comments  
*Project description  
*Analyse effect of sample rate on sample rate jitter, execution time jitter, error of requested sample rate  
*Measure MCU load during driver runtime  

# To enable driver for 10s with 100ms sample period
```bash
sudo bash demo.sh 10 100
```

# Results
![alt text](https://github.com/arvtom/sensor_driver_kernel/blob/main/analysis.png?raw=true)
