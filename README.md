# micro-tftlib
A small C++ library for using with microcontrollers like Arduino or ESPxx to view sensor data on 1.8" displays

#### Documentation

```cpp 
printStateBar(float value, int maxValue)
```
will print a status bar on the right side of the display. `maxValue` equals 100 % of the display height. The bar is calculated relative to the display height.
