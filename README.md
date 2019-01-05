# ESP_NIXIE

![Clock](/images/IMG_1246.JPG)

<img src="/images/IMG_3060.jpg" alt="Clock" width="320" height="240"> <img src="/images/IMG_1241.JPG" alt="Clock" width="320" height="240">

<img src="/images/IMG_1242.JPG" alt="Clock" width="320" height="240"> <img src="/images/settings.jpg" alt="Clock" width="185" height="240">

This repository contains the code, and schematics for driving a Nixie tube clock with a NodeMCU 1.0 ESP8266 development board.  STL files are also provided for a 3D printed case.

Building a Nixie Clock is a time honored tradition among makers, and this is yet another DIY Nixie Clock project.  This one is unique in a few respects: 1) it uses an ESP8266 development board as the microcontroller, allowing it to automatically sync its time to NIST over WiFi, and 2) it features a 3D printed frame and hand-solderable PCB.  

When the clock is turned on for the first time, it will create a WiFi access point at ESPNIXIE that can be connected to using a computer or smartphone.  The user can use this access point to specify the WiFi network SSID and password.  This information is stored in EEPROM, so will not be needed the next time the clock is turned on.  It uses the Arduino [Wifi Manager](https://github.com/tzapu/WiFiManager) to create the access point.  See the GitHub link for more information.  

This clock includes a menu-driven OLED screen and rotary encoder, with menu options described below.  All settings are stored in EEPROM so they can be used after a power cycle.
* *Set UTC Offset.*  The user's local time zone is selected by adjusting the time offset from UTC time.
* *Auto DST.* This option enables/disables automatic daylight savings time.  The code includes rules for US Daylight Savings Time (second Sunday in March at 2am to first Sunday in November at 2am).  The code can easily be modified for rules in other countries.
* *12/24 Hours.* This option selects between 12 and 24 hour time formats.
* *Protect Cathode.* Nixie tubes are susceptible to cathode poisoning when material from an active cathode sputters on the inactive cathodes.  This process can be reversed by occasionally cycling through all numbers for short durations.  When enabled, this feature will cycle through all the numbers at a 10 Hz rate for 5 seconds.  Besides protecting the cathodes, it also looks cool.  Time intervals ranging from 15 minutes to two hours can be selected.
* *Auto Shutoff.* When enabled, this feature will automatically turn off the tubes between designated hours (e.g., during the night), helping to preserve Nixie tube life.  The off and on times are selectable through a submenu.
* *Show Zero.* This option allows the user to select whether the left most Nixie tube (tens place for the hour) should be turned off when it is zero, or whether zero should be shown.  
* *Reset Wifi.*  Selecting this will cause the ESP8266 to forget the current WiFi network.  The clock will reboot and set up a new WiFi access point at ESPNIXIE.   

In addition to these settings, holding the rotary encoder button down for more than three seconds will manually toggle the Nixie tubes on and off.  If the Auto Shutoff feature is enabled, the Nixies will stay on or off as selected until the next Auto Shutoff transition occurs, after which the Nixie tubes will turn on and off according to the Auto Shutoff schedule.

**Please PM me if you would like a set of printed circuit boards.** I have enough extras to make a few more ESP Nixie clocks. You will need to purchase the ICs and other components, but those can easily be found on eBay, Amazon, and all the other usual places. I'm not trying to make money off of this project, so can offer the main board and four Nixie tube boards for a small amount (let's say $10). I even have a few extra NodeMCU ESP8266 boards, and can throw one in for $5 more. 

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
Most of these parts can be found on eBay or your favorite online Chinese retailer.  Although no longer manufactured, nixie tubes are still readily available, with the IN-14 selling for about $5-$10 per tube.  I recommend the NCH6100HV power supply, though there are many others out there capable of driving Nixie tubes.  The output voltage should be set to 170 V, which can be adjusted using an onboard potentiometer.  If selecting a different power supply, make sure it is capable of delivering at least 15 mA of continuous current at 170 V.

The Nixie Board PCB and tube PCBs must be manufactured by a PCB supplier.  The least expensive way to do this is to upload the gerber files (included as a .zip file) to one of several Chinese manufacturers such as SchenZhen2U, EasyEDA, or Seeed Studio.  pcbshopper.com allows a comparison between vendors.  Typical prices are $1-$3/board, with a minimum of 5 boards.  Recommended default settings are Layers: 2; PCB Thickness: 1.6mm; Surface Finish: HASL; Copper Weight: 1.  The main board is 100 x 80 mm, and the smaller tube boards are 20.7mm x 28.3mm. (Alternatively, contact me since I may still have a few extras I could send out for $10 + shipping.) 

<img src="/images/IMG_1250.JPG" alt="Clock" width="320" height="240"> <img src="/images/IMG_1207.JPG" alt="Clock" width="320" height="240">

Solder headers, IC sockets, terminal blocks, resistors, voltage regulator, and capacitors to the board.  The 0.33 uF capacitor goes closest to the 2-pin terminal block.  The 20K Ohm resistor is marked as optional.  This will illuminate the decimal point between the hours and minutes.  Personally, I think it looks better without it and I did not use it, but it is there if others wish to use it.  Mount ICs, and NodeMCU to the headers and sockets.  

<img src="/images/IMG_1249.JPG" alt="Clock" width="320" height="240"> <img src="/images/IMG_1247.JPG" alt="Clock" width="320" height="240">

3D print the parts.  The top cover is printed on its side, and will need supports to prevent the tube holes from sagging.  Four tube sockets are required.  

Mount Nixie tubes through 3D printed tube sockets, and then through the tube PCBs.  You may (optionally) use a stripboard instead of the tube PCB.  I included a picture below of a prototype version next to the official PCB on the right.  Be careful to make sure the anode wire (the rear wire on the tube with an arrow pointing to it) goes through the PCB hole that connects to the center pin of the 7-pin header.  I should have marked it on the PCB.  Also, the side of the PCB with the traces is the top side.  It will not plug into the board if the tube is soldered to the wrong side.  

<img src="/images/IMG_1248.JPG" alt="Clock" width="320" height="240">

Mount the board to the 3D printed base with M3 bolts, and the OLED and HV power supply with M2 bolts.  Mount the rotary encoder to the back of the case.  Wire the 2-pin terminal block to the 12V wall power supply, and the 3-pin terminal block to the HV power supply using loose wire.  The GND can go to either side of the HV power supply, but make sure the terminal labeled 12V goes to Vin, and terminal labeled 170V goes to HV.  Connect the OLED to the four OLED pins on the board using jumper cables.  The rotary encoder should have three pins on one side and two on the other.  Connect the three pins to the 3 ENC headers on the board, and the two pins to the 2 BTN pins on the board.  If the jumper cables on the encoder are loose, you may wish to solder them on directly.  I did.

Flash software to ESP8266 using a micro-USB cable.  This can be done one of two ways: a) Use the compiled .bin files included in the repository, or b) compile and flash the firmware using the Arduino IDE.  For upload, I recommend using esptool at https://github.com/espressif/esptool to flash the esp8266.  After installing esptool, the following line is used to flash the ESP8266:

`esptool.py --port COM6 write_flash 0x0000 ESP_NIXIE.ino.nodemcu.bin`
  
where COM6 should be replaced by whatever com port your device is plugged into. (In Windows, this is discoverable using the Device Monitor.) 

Glue the case side panels to the top cover.  I use plastic epoxy.  Close the case and attach on the side corners using (optional) 1/2" #4 or #6 sheet metal screws.

Whe first powering the ESP Nixie clock, it will create a wireless access point called ESPCLOCK with default password: PASSWORD.  Connect to the wireless access point with a computer, and open 192.168.4.1 in a web browser if the configuration page does not automatically open.  This configuration page can be used to set your clock to connect to your home WiFi network.  

The default access point network name and password are hardcoded in lines, which may be changed (and recompiled) as desired:

```
#define AP_NAME "ESPCLOCK"
#define AP_PASSWORD "PASSWORD"
```  

Also note that if you do not see any text at all on the screen, I2C address of the OLED screen may be incorrect.  The code uses 0x3C, which is fairly common.  The I2C_Scanner can determine if your screen has a different address.  The address in the code can be modified and recompiled as necessary.

# Special Instructions for Compiling and Flashing Firmware Using Arduino IDE
1. Configure Arduino IDE
   1. Install Arduino IDE 1.8.7 (or later) from arduino.cc
   2. Open File>Preferences, and enter the following URL into "Additional Board Manager URLs": http://arduino.esp8266.com/stable/package_esp8266com_index.json
   3. Open Tools>Board>Board Manager and install the esp8266 boards.  I am currently using v2.4.2, but later versions should also work.
   4. In the Tools menu, configure Board: NodeMCU 1.0 (ESP-12E Module), CPU Frequency: 80 MHz, Upload Speed: 115200.
2. Install the following libraries.  I have indicated which versions I am currently using (as of 1/5/2019), though in most cases later versions should work: NTPClient (3.1.0), WiFi (1.2.7), WiFiManager (0.14.0), Time (1.5.0), Timezone (1.2.2), Adafruit_GFX (1.3.6), Adafruit_SSD1306 (1.2.9), and Switch (1.2.1).  All of these libraries are added through the Library Manager (Sketch > Include Library > Manage Libraries).   
3. Connect the NodeMCU board to your computer using a micro USB cable, and set Tools>Port to the new port that appears.  Your computer should automatically install the driver, but if it does not, you may need to manually download and install the CP2102 driver from http://www.silabs.com/products/mcu/pages/usbtouartbridgevcpdrivers.aspx.  
4. Press the Upload button to compile the sketch and upload to the NodeMCU.  The most common reason for failing to compile are an selecting the wrong board or not installing all the required libraries.

# Optional Colon Separator Circuit
A colon separator circuit is not presently included in the PCB or 3D printed case, but may be added if desired.  To enable this feature in the code, uncomment the line:

`#define CLOCK_COLON`

The code controls the colon separator using pin D8.  If using a high voltage neon tube (e.g., IN-3 or NE-2H), you will need a high voltage transistor such as the MPSA42, and a 200K series resistor.  The following circuit can be soldered to a postage stamp sized breadboard:

<img src="/images/colon_circuit.JPG" alt="Clock">

As of today, the PCB does not have sufficient separation between the hour and minute tubes to support a colon separator, but I do plan on making this update at some point in the near future.




