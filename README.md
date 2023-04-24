# sensor_driver_kernel
Kernel module, which uses RPI 4B to read external ADC PCF8591 using I2C. Driver logs can be observed in demo.txt, driver performance analysis in analysis.txt, ADC values in analysis.png.

# enable driver for 10s with 100ms sample period
sudo bash demo.sh 10 100

![alt text](https://github.com/arvtom/sensor_driver_kernel/blob/main/demo.png?raw=true)
