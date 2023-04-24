# sensor_driver_kernel
This kernel module is written to learn basics of embedded linux. It uses RPI 4B to read extermal ADC PCF8591 using I2C. demo.txt contains summary. Driver logs can be observed in pcf8591.txt, driver performance analysis in analysis.txt, plot of ADC values in analysis.png.  
  
There is possibility to adjust driver sample rate and run time length by passing parameters to demo.sh.
  
Driver is tested using HW-390 Capacitive soil moisture sensor v2.0 with 3.3V power supply modification. Hand was shortly placed on sensor to simulate different ADC values.

# TODO
Error management  
Comments  
Description
Analyse sample rate vs jitter  
Mcu load  

# To enable driver for 10s with 100ms sample period
```bash
sudo bash demo.sh 10 100
```

# Results
![alt text](https://github.com/arvtom/sensor_driver_kernel/blob/main/analysis.png?raw=true)
