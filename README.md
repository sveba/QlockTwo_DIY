# QlockTwo DIY
*Project by Jonas Lauener and Frederic Letsch*

As we could not afford the hefty price tag of the original QlockTwo Classic, our aim was to build a perfect copy of it. Over the course of two years we built five units for us and our parents. In the spirit of the open source community we would like to enable everyone to build their own perfect QlockTwo.

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.

<img src="https://raw.githubusercontent.com/jolau/QlockTwo_DIY/master/docs/images/P1080219.jpg" width="30%"></img> <img src="https://raw.githubusercontent.com/jolau/QlockTwo_DIY/master/docs/images/P1080262.jpg" width="30%"></img> <img src="https://raw.githubusercontent.com/jolau/QlockTwo_DIY/master/docs/images/P1080293.jpg" width="30%"></img> <img src="https://raw.githubusercontent.com/jolau/QlockTwo_DIY/master/docs/images/P1080243.jpg" width="30%"></img> <img src="https://raw.githubusercontent.com/jolau/QlockTwo_DIY/master/docs/images/P1080252.jpg" width="30%"></img> <img src="https://raw.githubusercontent.com/jolau/QlockTwo_DIY/master/docs/images/P1080278.jpg" width="30%"></img> 

## Front Panel
The front panel is a complete reproduction of the original QlockTwo panel in terms of proportions, font and words. Due to copyright issues we only provide an Ilustrater template here. It should be easy tough to replace the letters to get the version you want. As raw material for the front plate we used an acrylic glass with a thin black color film on the back side, where we engraved the letters with a laser cutter. (TG4-400 https://www.gravurmaterialien.ch/gravurmaterial/acrylglas/troglass-reverse.html)
<p align="center">
  <img width="400" height="400" src="https://raw.githubusercontent.com/jolau/QlockTwo_DIY/master/front/Qlocktwo_blank-1.png">
</p>

## Baseplate
The dimensions of the clock were derived from the distance betweend to LEDs on a strip (33.3 mm) such that the proportions of our front panel matches the original ones. Since we had acces to a CNC milling machine, we designed a unibody to hold our LEDs and the front panel. If you don't have access to a milling machine, you may slice the CAD file in layers to use a laser cutter and some glue instead.
To avoid the LEDs shining trough to other letter we build a pattern using a laser cutter to shield the LEDs. The front panel is held in place by magnets in the baseplate and metal discs glued to the front panel. This enabels a seamless and easy mount of the front panel.
![cad_overview](https://raw.githubusercontent.com/jolau/QlockTwo_DIY/master/CAD%20files/001_000002_00_A_Qlocktwo_Assembly_VersionSchale.png)

## Electronics
### Used parts
- RGBW LED strip
- ESP8266 (WEMOS D1)
- DS3231 (RTC Clock)
- 4 push buttons ([Multimec 5GTH935](https://www.digikey.ch/product-detail/en/mec-switches/5GTH935/679-2612-ND/2411559)) with white caps ([1DS06](https://www.digikey.ch/product-detail/en/mec-switches/1DS06/679-2520-ND/2411577))
- FET for voltage level shifter
- 5528 Light Dependent Resistor
- 5V power adapter with textile cable and [DC power plug](https://www.te.com/usa-en/product-1-2129334-1.html)/[jack](https://www.te.com/usa-en/product-2129567-1.html)

### Schematics
The schematics are fairly simple. We realised it on a stripboard.
![schematics](https://raw.githubusercontent.com/jolau/QlockTwo_DIY/master/schematics/QlockTwo_Wemos_schem.png) 
Only the voltage level shifter is to be remarked. As the LED strip are on a 5V level and the ESP8266 has 3V3, a voltage level shifter is implemented to shift the signal from 3V3 to 5V.

## Software
The ESP8266 is programed with the Arduino platform by PlatformIO. Many thanks to all the incredibles Arduino libraries.
### Used libraries
- [NeoPixelBus](https://github.com/Makuna/NeoPixelBus): Control the RGBW LED strip.
- [WifiManager](https://github.com/tzapu/WiFiManager): Handle Wifi connection and enter AP mode to enter Wifi credentials and enable/disable time of QlockTwo.
- [RTC](https://github.com/Makuna/Rtc): Handle the DS3231 RTC module.
- [AceButton](https://github.com/bxparks/AceButton): Handle the four push buttons.
- [ArduinoJson](https://arduinojson.org/): Used to serialize the configuration object for saving it on SPIFF (file system of ESP8266). 
- [Time](https://github.com/PaulStoffregen/Time) and [Timezone](https://github.com/JChristensen/Timezone): Convert the UTC time to local time (with timezone and daylight saving adjustement).

###  Code architecture
The code is structured in independant modules. These are then wired together and used in the main.cpp.
- **AmbientLightModule**: Adapts the light intensity based on readings from a light sensor
- **ClockModule**: ClockModule is handling the RTC and keeps it updated over NTP
- **ConfigModule**: Save and Load Config to/from SPIFF of the ESP
- **LedControlModule**:	Control the LEDs resp show the words and minute dots
- **LedWord**: LedWord representing on word (only horizontal) on the matrix, defined by first pixel and word length
- **SimpleTime**: A simple representation of time with hour and minutes
- **WifiModule**: Handling connection to Wifi and setting up Wifi credentials by creating an Access Point 

All documentation of the code can be found here: [Doxygen](https://jolau.github.io/QlockTwo_DIY/html/index.html)
