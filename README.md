# sensor_driver_kernel
This kernel module is written to learn basics of embedded linux. It uses RPI 4B to read extermal ADC PCF8591 using I2C. Driver logs can be observed in demo.txt, driver performance analysis in analysis.txt, ADC values in analysis.png.

# To enable driver for 10s with 100ms sample period
```bash
sudo bash demo.sh 10 100
```

# Results
![alt text](https://github.com/arvtom/sensor_driver_kernel/blob/main/analysis.png?raw=true)
