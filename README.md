# GPS-Tracker

#

The GPS-Tracker is an extremely simple GPS device that records its current position to a removable micro SD card, where it can then be dumped into a GPS coordinate visualiser.
#

## Schematic
![gpstrackerschematic](https://i.imgur.com/oEBkrGV.jpg)
## Bill of Materials
- J: [ESP32-Pico-D4-Module](https://www.amazon.com/RCmall-ESP32-PICO-KIT-Development-ESP32-PICO-D4-Module/dp/B09WDQYDJD?th=1)
- U: [NEO-6M GPS module](https://www.amazon.com/Microcontroller-Compatible-Sensitivity-Navigation-Positioning/dp/B07P8YMVNT?th=1)
- S: [SparkFun microSD Transflash Breakout](https://www.sparkfun.com/products/544)

## Tips For Construction
- When flashing data onto the ESP32-Pico-D4-Module, make sure that the RX0 and TX0 are not connected to anything.

## Wiring
Esp32 Board --- Exterenal Device
### NEO-6M:
- TX0 --- RXD
- RX0 --- TXD
- 5v --- VCC
- GND --- GND
### SparkFun microSD:
- 19 --- DO
- 23 --- DI
- 18 --- SCK
- 5 --- CS
- 3V3 --- VCC
- GND --- GND


