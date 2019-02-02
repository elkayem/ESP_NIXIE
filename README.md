# ESP_NIXIE

![Clock](/images/clock.gif)

<img src="/images/20190120-0001.jpg" alt="Clock" width="400"> <img src="/images/20190120-0002.jpg" alt="Clock" width="400">

<img src="/images/IMG_3060.jpg" alt="Clock" width="400"> <img src="/images/IMG_1246.JPG" alt="Clock" width="400"> 

<img src="/images/IMG_1241.JPG" alt="Clock" width="320"> <img src="/images/IMG_1242.JPG" alt="Clock" width="320"> <img src="/images/settings.jpg" alt="Clock" width="185">

Building a Nixie Clock is a time honored tradition among makers, and this is yet another DIY Nixie Clock project.  This one is unique in a few respects: 1) it uses an ESP8266 development board as the microcontroller, allowing it to automatically sync its time to NIST over WiFi, and 2) it features a 3D printed frame and hand-solderable PCB. 

This repository contains the code, and schematics for driving a Nixie tube clock with a NodeMCU 1.0 ESP8266 development board.  STL files are also provided for a 3D printed case.  Two variants of the PCB and 3D printed cases are available, a PCB and case without a colon separator circuit, and a PCB and case with a colon separator circuit.  The code is the same, with a single #define statement that can be commented to compile the option without the separator circuit.   

When the clock is turned on for the first time, it will create a WiFi access point at ESPNIXIE that can be connected to using a computer or smartphone.  The user can use this access point to specify the WiFi network SSID and password.  This information is stored in EEPROM, so will not be needed the next time the clock is turned on.  It uses the Arduino [Wifi Manager](https://github.com/tzapu/WiFiManager) to create the access point.  

This clock includes a menu-driven OLED screen and rotary encoder, with menu options described below.  All settings are stored in EEPROM so they can be used after a power cycle.
* *Set UTC Offset.*  The user's local time zone is selected by adjusting the time offset from UTC time.
* *Auto DST.* This option enables/disables automatic daylight savings time.  The code includes rules for US Daylight Savings Time (second Sunday in March at 2am to first Sunday in November at 2am).  The code can easily be modified for rules in other countries.
* *12/24 Hours.* This option selects between 12 and 24 hour time formats.
* *Blink Colon.* This option is available if using a colon separator.  If enabled, it will blink the colon once per second, otherwise it will keep the colon on steady.  This option is not available if #define CLOCK_COLON is commented in the code.  
* *Protect Cathode.* Nixie tubes are susceptible to cathode poisoning when material from an active cathode sputters on the inactive cathodes.  This process can be reversed by occasionally cycling through all numbers for short durations.  When enabled, this feature will cycle through all the numbers at a 10 Hz rate for 5 seconds.  Besides protecting the cathodes, it also looks cool.  Time intervals ranging from 15 minutes to two hours can be selected.
* *Auto Shutoff.* When enabled, this feature will automatically turn off the tubes between designated hours (e.g., during the night), helping to preserve Nixie tube life.  The off and on times are selectable through a submenu.
* *Screensaver.* When enabled, a screensaver will activate on the OLED if the menu has been inactive for more than 60 seconds.  OLEDs are subject to screen burn-in, and this will help alleviate that issue.
* *Show Zero.* This option allows the user to select whether the left most Nixie tube (tens place for the hour) should be turned off when it is zero, or whether zero should be shown.  
* *Reset Wifi.*  Selecting this will cause the ESP8266 to forget the current WiFi network.  The clock will reboot and set up a new WiFi access point at ESPNIXIE.   

In addition to these settings, holding the rotary encoder button down for more than three seconds will manually toggle the Nixie tubes on and off.  If the Auto Shutoff feature is enabled, the Nixies will stay on or off as selected until the next Auto Shutoff transition occurs, after which the Nixie tubes will turn on and off according to the Auto Shutoff schedule.

**Please PM me if you would like a set of printed circuit boards.** I have enough extras to make a few more ESP Nixie clocks.  

## Parts
* NodeMCU V1.0 (Amica board).  Clone boards often are unofficially labeled as V2
* 4 IN-14 Nixie Tubes
* 1 0.96" I2C 128x64 OLED display
* 1 Rotary Encoder
* 1 12VDC 1A Wall Adapter Power Supply
* 1 NCH6100HV High Voltage DC Power Supply (or equivalent)
* 1 Nixie Board PCB (options with or without colon separator circuit).  I've included the Eagle schematic, board layout, and gerber files in this repository for both options.  
* 2 15-pin female headers
* Lots of male breakaway headers (I haven't counted, buy a pack)
* 6 16-pin DIP IC Sockets
* 4 Nixie Tube PCBs.  Files included in this repository.  This can also easily be created using a strip board.
* 2 74HC595N shift registers
* 4 74141N BCD to Decimal Decoders (or K155ID1 Russian equivalents)
* 1 L7805 5V regulator (an additional heat sink is recommended)
* 1 0.33 uF capacitor
* 1 0.1 uF capacitor
* 1 3 Pin 5.08mm Pitch PCB Mount Screw Terminal  
* 1 2 Pin 5.08mm Pitch PCB Mount Screw Terminal 
* 4 15K Ohm resistors
* 1 20K Ohm resistor (used to illuminate decimal point if not using colon separator circuit.  However, I no longer recommend using this resistor for ascetic reasons.)
* Various M2 and M3 bolts
* 8 female-female short jumper cables.
* 1 MPSA42 Transistor (if using colon separator PCB)
* 2 200K Ohm resistors (if using colon separator circuit)
* 1 33K Ohm resistor (if using colon separator circuit)
* 1 IN-3 tube or 2 NE-2H tubes (if using colon separator circuit)

## Instructions
Most of these parts can be found on eBay or your favorite online Chinese retailer.  Although no longer manufactured, nixie tubes are still readily available, with the IN-14 selling for about $5-$10 per tube.  I recommend the NCH6100HV power supply, though there are many others out there capable of driving Nixie tubes.  The output voltage should be set to 170 V, which can be adjusted using an onboard potentiometer.  If selecting a different power supply, make sure it is capable of delivering at least 15 mA of continuous current at 170 V.

The Nixie Board PCB and tube PCBs must be manufactured by a PCB supplier.  The least expensive way to do this is to upload the gerber files (included as a .zip file) to one of several Chinese manufacturers such as JLCPCB, SchenZhen2U, or Seeed Studio.  pcbshopper.com allows a comparison between vendors.  Typical prices are $1-$3/board, with a minimum of 5 boards.  Recommended default settings are Layers: 2; PCB Thickness: 1.6mm; Surface Finish: HASL; Copper Weight: 1.  The main board is 100 x 80 mm, and the smaller tube boards are 20.7mm x 28.3mm.  

<img src="/images/IMG_3482.JPG" alt="Clock" width="320"> <img src="/images/IMG_1250.JPG" alt="Clock" width="320">

<img src="/images/IMG_3473.JPG" alt="Clock" width="320" height="240"> <img src="/images/IMG_1207.JPG" alt="Clock" width="320" height="240">

Solder headers, IC sockets, terminal blocks, resistors, voltage regulator, and capacitors to the board.  The 0.33 uF capacitor goes closest to the 2-pin terminal block.  The 20K Ohm resistor is marked as optional.  This will illuminate the decimal point between the hours and minutes.  Personally, I think it looks better without it and I did not use it, but it is there if others wish to use it.  Mount ICs, and NodeMCU to the headers and sockets.  

<img src="/images/IMG_1249.JPG" alt="Clock" width="320" height="240"> <img src="/images/IMG_1247.JPG" alt="Clock" width="320" height="240">

3D print the parts.  The top cover is printed on its side, and will need supports to prevent the tube holes from sagging.  Four tube sockets are required.  

Mount Nixie tubes through 3D printed tube sockets, and then through the tube PCBs.  You may (optionally) use a stripboard instead of the tube PCB.  I included a picture below of a prototype version next to the official PCB on the right.  Be careful to make sure the anode wire (the rear wire on the tube with an arrow pointing to it) goes through the PCB hole that connects to the center pin of the 7-pin header.  I should have marked it on the PCB.  Also, the side of the PCB with the traces is the top side.  It will not plug into the board if the tube is soldered to the wrong side.  

<img src="/images/IMG_1248.JPG" alt="Clock" width="320" height="240">

Mount the board to the 3D printed base with M3 bolts, and the OLED and HV power supply with M2 bolts.  Mount the rotary encoder to the back of the case.  Wire the 2-pin terminal block to the 12V wall power supply, and the 3-pin terminal block to the HV power supply using loose wire.  The GND can go to either side of the HV power supply, but make sure the terminal labeled 12V goes to Vin, and terminal labeled 170V goes to HV.  Connect the OLED to the four OLED pins on the board using jumper cables.  The rotary encoder should have three pins on one side and two on the other.  Connect the three pins to the 3 ENC headers on the board, and the two pins to the 2 BTN pins on the board.  If the jumper cables on the encoder are loose, you may wish to solder them on directly.  I did.

Flash software to ESP8266 using a micro-USB cable.  This can be done one of two ways: a) Use the compiled .bin files included in the repository, or b) compile and flash the firmware using the Arduino IDE.  For upload, I recommend using esptool at https://github.com/espressif/esptool to flash the esp8266.  After installing esptool, the following line is used to flash the ESP8266:

`esptool.py --port COM6 write_flash 0x0000 ESP_NIXIE_with_colon_separator.bin`
  
where COM6 should be replaced by whatever com port your device is plugged into. (In Windows, this is discoverable using the Device Monitor.) 

Glue the case side panels to the top cover.  I use plastic epoxy.  Close the case and attach on the side corners using (optional) 1/2" #4 or #6 sheet metal screws.

When first powering the ESP Nixie clock, it will create a wireless access point called ESPCLOCK with default password: PASSWORD.  Connect to the wireless access point with a computer, and open 192.168.4.1 in a web browser if the configuration page does not automatically open.  This configuration page can be used to set your clock to connect to your home WiFi network.  

The default access point network name and password are hardcoded in lines, which may be changed (and recompiled) as desired:

```
#define AP_NAME "ESPCLOCK"
#define AP_PASSWORD "PASSWORD"
```  

To compile the code without the colon separator PCB, comment out the following line.  This removes the option to enable/disable the circuit from blinking in the menu.

`#define CLOCK_COLON \\ Comment out if not using colon separator PCB`

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




