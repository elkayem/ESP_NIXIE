# ESP_NIXIE

![Clock](/images/IMG_3060.jpg)

<img src="/images/IMG_3056.jpg" alt="Clock" width="320" height="240"> <img src="/images/IMG_1241.JPG" alt="Clock" width="320" height="240">

<img src="/images/IMG_1242.JPG" alt="Clock" width="320" height="240"> <img src="/images/IMG_1243.JPG" alt="Clock" width="320" height="240">

This repository contains the code, and schematics for driving a Nixie tube clock with a NodeMCU 1.0 ESP8266 development board.  STL files are also provided for a 3D printed case.

Building a Nixie Clock is a time honored tradition among makers, and this is yet another DIY Nixie Clock project.  This one is unique in a few respects: 1) it uses an ESP8266 development board as the microcontroller, allowing it to automatically sync its time to NIST over WiFi, and 2) it features a 3D printed frame and hand-solderable PCB.  

When the clock is turned on for the first time, it will create a WiFi access point that you will need to connect to using a computer or smartphone.  The user can use this access point to specify the WiFi network SSID and password.  This information is stored in EEPROM, so will not be needed the next time the clock is turned on.  It uses the Arduino [Wifi Manager](https://github.com/tzapu/WiFiManager) to create the access point.  See the GitHub link for more information.  

The OLED screen and rotary encoder are used to specify the time offset from UTC time.  Pressing the encoder button accesses the menu-driven settings.  This offset is stored in EEPROM.  The settings menu is also used to enable auto-daylight savings time adjustment.  It assumes the current US daylight savings time rules.  If the auto adjustment isn't enabled, the UTC time offset will need to be updated anytime DST begins or ends.  

## Parts
* NodeMCU V1.0 (Amica board).  Clone boards often are unofficially labeled as V2
* 4 IN-14 Nixie Tubes
* 1 0.96" I2C 128x64 OLED display
* 1 Rotary Encoder
* 1 12VDC 1A Wall Adapter Power Supply
* 1 NCH6100HV High Voltage DC Power Supply (or equivalent)
* 1 Nixie Board PCB.  I've included the Eagle schematic, board layout, and gerber files in this repository.  
* 2 15-pin female headers
* Lots of male breakaway headers (I haven't counted, buy a pack)
* 6 16-pin DIP IC Sockets
* 4 Nixie Tube PCBs.  Files included in this repository.  This can also easily be created using a strip board.
* 2 74HC595N shift registers
* 4 74141N BCD to Decimal Decoders (or K155ID1 Russian equivalents)
* 1 L7805 5V regulator
* 1 0.33 uF capacitor
* 1 0.1 uF capacitor
* 1 3 Pin 5.08mm Pitch PCB Mount Screw Terminal  
* 1 2 Pin 5.08mm Pitch PCB Mount Screw Terminal 
* 4 15K Ohm resistors
* (optional) 1 20K Ohm resistor
* Various M2 and M3 bolts
* 8 female-female short jumper cables.

## Instructions
Most of these parts can be found on eBay or your favorite online Chinese retailer.  Although no longer manufactured, nixie tubes are still readily available, with the IN-14 selling for about $5-$10 per tube.  I recommend the NCH6100HV power supply, though there are many others out there capabile of driving Nixie tubes.  The output voltage should be set to 170 V, which can be adjusted using an onboard potentiometer.  If selecting a different power supply, make sure it is capable of delivering at least 15 mA of continuous current at 170 V.

The Nixie Board PCB and tube PCBs must be manufactured by a PCB supplier.  The least expensive way to do this is to upload the gerber files (included as a .zip file) to one of several Chinese manufacturers such as SchenZhen2U, EasyEDA, or Seeed Studio.  pcbshopper.com allows a comparison between vendors.  Typical prices are $1-$3/board, with a minimum of 5 boards.  Recommended default settings are Layers: 2; PCB Thickness: 1.6mm; Surface Finish: HASL; Copper Weight: 1.  The main board is 100 x 80 mm, and the smaller tube boards are 20.7mm x 28.3mm. (Alternatively, contact me since I may still have a few extras I could send out for $5 + shipping.) 

<img src="/images/IMG_1250.JPG" alt="Clock" width="320" height="240"> <img src="/images/IMG_1207.JPG" alt="Clock" width="320" height="240">

Solder headers, IC sockets, terminal blocks, resistors, voltage regulator, and capacitors to the board.  The 0.33 uF capacitor goes closest to the 2-pin terminal block.  The 20K Ohm resistor is marked as optional.  This will illuminate the decimal point between the hours and minutes.  Personally, I think it looks better without it and I did not use it, but it is there if others wish to use it.  Mount ICs, and NodeMCU to the headers and sockets.  

<img src="/images/IMG_1249.JPG" alt="Clock" width="320" height="240"> <img src="/images/IMG_1247.JPG" alt="Clock" width="320" height="240">

3D print the parts.  The top cover is printed on its side, and will need supports to prevent the tube holes from sagging.  Four tube socekts are required.  

Mount Nixie tubes through 3D printed tube sockets, and then through the tube PCBs.  You may (optionally) use a stripboard instead of the tube PCB.  I included a picture below of a prototype verson next to the official PCB on the right.  Be careful to make sure the anode wire (the rear wire on the tube with an arrow pointing to it) goes through the PCB hole that connects to the center pin of the 7-pin header.  I should have marked it on the PCB.  Also, the side of the PCB with the traces is the top side.  It will not plug into the board if the tube is soldered to the wrong side.  

<img src="/images/IMG_1248.JPG" alt="Clock" width="320" height="240">

Mount the board to the 3D printed base with M3 bolts, and the OLED and HV power supply with M2 bolts.  Mount the rotary encoder to the back of the case.  Wire the 2-pin terminal block to the 12V wall power supply, and the 3-pin terminal block to the HV power supply using loose wire.  The GND can go to either side of the HV power supply, but make sure the terminal labelled 12V goes to Vin, and terminal labelled 170V goes to HV.  Connect the OLED to the four OLED pins on the board using jumper cables.  The rotary encoder should have three pins on one side and two on the other.  Connect the three pins to the 3 ENC headers on the board, and the two pins to the 2 BTN pins on the board.  If the jumper cables on the encoder are loose, you may wish to solder them on directly.  I did.

Upload the firmware to the NodeMCU board using the Arduino IDE and a micro USB cable.  There are plenty of tutorials on how to configure the Arduino IDE (such as [here](http://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/)) so I won't cover it in this Readme.  Note that a number of libraries are required, most of which can be installed through the Arduino Library Manager.  Specifically, you will need NTPClient, Adafruit GFX, Adafruit SSD1306, EEPROM, ESP8266WiFi, Time, Wifi, WifiManager, and Wire.  In addition, you will need the Timezone library that is not available in the Library Manager, but must be manually installed by adding it to the Arduino libraries folder.  Timezone can be found [here](https://github.com/JChristensen/Timezone).  Finally, you will need a copy of Albert van Dalen's terrific Switch library, found [here](https://github.com/blackketter/Switch).  

Glue the case side panels to the top cover.  I use plastic epoxy.  Close the case and attach on the side corners using (optional) 1/2" #4 or #6 sheet metal screws.

Plug it in and enjoy!  




