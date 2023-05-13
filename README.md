# GPS-Tracker

#

The GPS-Tracker is an extremely simple GPS device that records its current position to a removable micro SD card, where it can then be dumped into a GPS coordinate visualiser.
#
![IMG_20230114_194207_hdr](https://github.com/skinnyrad/GPS-Tracker/assets/124194193/3aa2e711-0c32-4537-a465-d7b46b087908)

## Schematic
![GPS2SCHEME](https://github.com/skinnyrad/GPS-Tracker/assets/124194193/9eda6906-71f8-44f7-852c-c0fd8385bc26)
## Bill of Materials
- E: [ESP32-Pico-D4-Module](https://www.amazon.com/RCmall-ESP32-PICO-KIT-Development-ESP32-PICO-D4-Module/dp/B09WDQYDJD?th=1)
- G: [NEO-6M GPS module](https://www.amazon.com/Microcontroller-Compatible-Sensitivity-Navigation-Positioning/dp/B07P8YMVNT?th=1)
- S: [SparkFun microSD Transflash Breakout](https://www.sparkfun.com/products/544)

## Wiring
### ESP32 --- NEO-6M:
- TX0 --- RXD
- RX0 --- TXD
- 5v --- VCC
- GND --- GND
### ESP32 --- SparkFun microSD:
- 19 --- DO
- 23 --- DI
- 18 --- SCK
- 5 --- CS
- 3V3 --- VCC
- GND --- GND

## Libraries
Either search with the Library Manager in Arduino IDE or click the links.
- [LittleFS_esp32 by lorol](https://github.com/lorol/LITTLEFS)
- [TinyGPSPlus-ESP32 by Mikal Hart](https://github.com/Tinyu-Zhao/TinyGPSPlus-ESP32)

## Additional boards manager URLs
Copy this URL to the corresponding section in preferences. 
- https://dl.espressif.com/dl/package_esp32_index.json
#
(Make sure to also install this with the Board Manager; The board used by this project is named "ESP32 PICO-D4")

## Useful Information

## Tips For Construction
- When flashing data onto the ESP32-Pico-D4-Module, make sure that the RX0 and TX0 are not connected to anything.

### How To Use
After the device is fully constructed, programmed and powered, the GPS will immediately begin recording the location data to the SD card. 
### Mapping
[Online GPS Visualizer](https://www.gpsvisualizer.com/)
 - Choose the positional data file with the "Browse" button, and press "Map it"
