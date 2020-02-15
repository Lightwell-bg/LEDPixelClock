# LED Pixel Clock (Clock, temperature, humidity, atmospheric pressure, and remote monitoring)
Multifunctional clock based on ESP8266 and WS2811 addressable LEDs for Home Automation (IoT)

------------
## What is a LED Pixel Clock?
The LED Pixel Clock is a four digit clock made up of 30 WS2811 addressable LEDs (8 mm) arranged into a 7-segment display. It based on ESP8266 microcontroller and real-time clock DS3231. BME280 sensor provides a precise measurement of indoor temperature, humidity and barometric pressure. An Internet connection via Wi-Fi module helps to synchronize time with NTP servers. Data from sensors can be transmitted via MQTT protocol or displayed on popular monitoring service thingspeak.com. Clock adjustment and manipulation is performed via WEB-interface.

# Features include:
- Always precise time because of synchronization with NTP time servers
- Indoor temperature, humidity and barometric pressure measurement
- Sending temperature, humidity and barometric pressure data to MQTT and Thingspeak.com server
- No buttons – control the clock via WEB-interface from PC, tablet or smartphone
- High display brightness
- Seven colors to display information
- Brightness adjustment
- Two alarm clocks
- Firmware update via Wi-Fi
- Information display on 3 languages: English, Russian, Bulgarian
- An option to install different alternative Open Source firmware
- Powered via USB
- Hackable: Open source hardware and firmware, Displays information read via UART

## Required Parts:
- Wemos D1 Mini: http://got.by/40d3u7
- RGB LED with WS2811 8mm http://ali.pub/4h4ehf
- BME280 Digital Sensor (Temperature, Humidity, Barometric Pressure) 5V: http://got.by/40d52x
- TTP223B digital touch capacitive sensor: http://got.by/40d6kz
- DS3231 AT24C32 IIC Module Precision Clock http://got.by/42k10g
- Active Buzzer http://got.by/42k1sg

Note: Using the links provided here help to support these types of projects. Thank you for the support.

**You can purchase a PCB with soldered elements and connectors for connecting the indicator and sensors, or just a PCB and a set of elements for installation**
https://diy.led-lightwell.eu/

## Schematics / Assembly
| Component  | Pins  |  Wemos D1 Mini |
| ------------ | ------------ | ------------ |
| **RGB LED with WS2811**  |  Din -->|D5 (GPIO14)|
||+5V -->||
||GND --> |   |
|**DS3231**|SCL -->|D1 (GPIO5)|
||SDA -->|D2 (GPIO4)|
||+3V||
||GND||
|**BME280**|SCL -->|D1 (GPIO5)|
||SDA -->|D2 (GPIO4)|
||+5V||
||GND||
|**Buzzer (active)**|+ -->|D8 (GPIO15)|
||GND||
|**TTP223B digital touch capacitive  sensor**|Dout -->|D6 (GPIO12)|
||+5V||
||GND||


## Power Supply
You must use a power supply 5V with a capacity of 2 A or more.

## Loading Supporting Library Files in Arduino
The following packages and libraries are used (download and install):
- ArduinoJson by Benoit Blanchon (version 6.13.0)
- TimeLib by Paul Stoffregen (version 1.5)
- PubSubClient by Nick O'Leary (version 2.7)
- RTClib by Adafruit  (version 1.3.3)
- Adafruit Unified Sensor by Adafruit (version 1.0.3) (for DHT sensor only)
- DHT sensor library by Adafruit (version 1.3.8) (for DHT sensor only)
- BlueDot BME280 Library by BlueDot (version 1.0.9)
- NetCrtESP (version 1.0) https://github.com/Lightwell-bg/NetCrtESP
- ESPTimeFunc (version 1.0) https://github.com/Lightwell-bg/ESPTimeFunc

Note: There could be some Issues if using different versions of the libraries.

## Initial Configuration
Starting with LW-Clock editing the Config.h file is optional. SSID and password for Wi-Fi, all API Keys are managed in the Web Interface. It is not required to edit the Config.h file before loading and running the code.
This project requires SPIFFS for saving and reading configuration settings
Use an external power supply with a capacity of 2 A or more!
**NOTE:** The settings in the Config.h are the default settings for the first loading. After loading you can change the settings via the Web Interface. If you want to change settings again in the Config.h, you will need to erase the file system on the Wemos or use the “Reset Settings” option in the Web Interface. During the first time run, Wemos D1 mini will try to connect to your Wi-Fi and fail (because it doesn't have any previously saved Wi-Fi credentials). This will cause it to start up an access point, which serves up a captive configuration portal. You can connect to this access point to select and enter credentials for your network. Save configuration and restart device. (Wemos D1 mini will restart and connect to your Wi-Fi network)

## Web Interface
All clock management is carried out using a web browser. On first power-up, LW-Clock will serve a hotspot called LW-Clock. For clock adjustment and manipulation open a webpage with the 192.168.4.1 address. Within this hotspot you can manually override the time and date, set text messages for display, set timecodes of messages output and update software.

For Wi-Fi network connection please input its SSID and password. After successful connection LW-Clock will begin to receive and transmit information over the Internet. NTP servers are used to get accurate time. To transmit temperature, humidity and barometric pressure data you can use the MQTT server https://www.cloudmqtt.com/ and the server https://thingspeak.com. You can also transmit information for display on the clock in the form of a creeping line through the MQTT server.

![](https://github.com/Lightwell-bg/LEDPixelClock/blob/master/3DPrint/LPC-011.jpg)
![](https://github.com/Lightwell-bg/LEDPixelClock/blob/master/3DPrint/LPC-012.jpg)
![](https://github.com/Lightwell-bg/LEDPixelClock/blob/master/3DPrint/LPC-02.jpg)
![](https://github.com/Lightwell-bg/LEDPixelClock/blob/master/3DPrint/LPC-03.jpg)
![](https://github.com/Lightwell-bg/LEDPixelClock/blob/master/3DPrint/LPC-04.jpg)
![](https://github.com/Lightwell-bg/LEDPixelClock/blob/master/3DPrint/LPC-05.jpg)
![](https://github.com/Lightwell-bg/LEDPixelClock/blob/master/3DPrint/LPC-06.jpg)
![](https://github.com/Lightwell-bg/LEDPixelClock/blob/master/3DPrint/LPC-07.jpg)