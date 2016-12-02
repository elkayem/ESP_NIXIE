/* 
 *    ESP Nixie Clock 
 *	  Copyright (C) 2016  Larry McGovern
 *	
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License <http://www.gnu.org/licenses/> for more details.
 */
	
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <Wire.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Switch.h"


#define D0 16 // LED_BUILTIN
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0 // 
#define D4 2 //  Blue LED 
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS) 
#define D9 3 // RX0 (Serial console) 
#define D10 1 // TX0 (Serial console) 

const int dataPin  = D5; // SER (pin 14)
const int latchPin = D6; // RCLK (pin 12)
const int clockPin = D7; // SRCLK (pin 11)

const int encoderPinA = D9;
const int encoderPinB = D10;
const int encoderButtonPin = D0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.nist.gov", 0, 7200000); // Update time every two hours

// Set timezone rules.  Offsets set to zero, since they will be loaded from EEPROM
TimeChangeRule myDST = {"DST", Second, Sun, Mar, 2, 0};    
TimeChangeRule mySTD = {"STD", First, Sun, Nov, 2, 0};     
Timezone myTZ(myDST, mySTD);

#define OLED_RESET  LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

Switch encoderButton = Switch(encoderButtonPin, INPUT_PULLUP);
int encoderPos, encoderPosPrev;

enum Menu {
  TOP,
  SETTINGS,
  RESET_WIFI,
  SET_UTC_OFFSET,
  ENABLE_DST
} menu;

// EEPROM addresses
const int EEPROM_addr_UTC_offset = 0; 
const int EEPROM_addr_DST = 1;  

bool enableDST;  // Flag to enable DST

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT); 
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // OLED I2C Address, may need to change for different device,
                                              // Check with I2C_Scanner

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  
  display.setCursor(0,0);
  display.println("Connecting...");
  display.println();
  display.println("Cycle Power after a");
  display.println("few minutes if no");
  display.print("connection.");
  display.display();

    // Setup WiFiManager
  WiFiManager MyWifiManager;
  MyWifiManager.setAPCallback(configModeCallback);
  MyWifiManager.autoConnect("ESPCLOCK");
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Wifi Connected");
  display.display();

  timeClient.begin();

  display.setCursor(0,28);
  display.println("Updating local time");
  display.display();
  while (!timeClient.update()) {
    delay(500);
    display.print(".");
    display.display();
  }

  EEPROM.begin(2);
  // Read Daylight Savings Time setting from EEPROM
  enableDST = EEPROM.read(EEPROM_addr_DST) != 0;
  
  // Read UTC offset from EEPROM
  int utc_offset = (((int)EEPROM.read(EEPROM_addr_UTC_offset)+12) % 24) - 12;
  mySTD.offset = utc_offset * 60;
  myDST.offset = mySTD.offset;
  if (enableDST) {
    myDST.offset += 60;
  }
  myTZ = Timezone(myDST, mySTD);

  menu = TOP;
  updateSelection();
}

time_t prevTime = 0;
bool initial_loop = 1;

void loop() {
  
  updateEncoderPos();
  encoderButton.poll();

  if (encoderButton.pushed()) {
    if (initial_loop == 1) {
      initial_loop = 0;  // Ignore first push
    }
    else {
      updateMenu();
    }
   }

  timeClient.update();
  setTime(myTZ.toLocal(timeClient.getEpochTime()));

  if (now() != prevTime) {
    prevTime = now();
    displayTime();
  }
}

void displayTime(){
   char tod[10], time_str[20], date_str[20];
   const char* am_pm[] = {"AM", "PM"};
   const char* month_names[] = {"Jan", "Feb", "March", "April", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec"};
   unsigned char hourBcd = decToBcd((unsigned char)hourFormat12());

   if ((hourBcd >> 4) == 0) { // If 10's digit is zero, we don't want to display a zero
    hourBcd |= (15 << 4); 
   }
   
   // Write to shift register
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, hourBcd);
   shiftOut(dataPin, clockPin, MSBFIRST, decToBcd((unsigned char)minute()));
   digitalWrite(latchPin, HIGH);
   
   if ((menu == TOP) || (menu == SET_UTC_OFFSET)) {
      formattedTime(tod, hourFormat12(), minute(), second());
      sprintf(time_str, "%s %s", tod, am_pm[isPM()]);
      sprintf(date_str, "%s %d, %d", month_names[month() - 1], day(), year());
      display.fillRect(20,28,120,8,BLACK);
      display.setCursor(20,28);
      display.print(time_str);
      if (enableDST) {
        if (myTZ.utcIsDST(timeClient.getEpochTime())) {
          display.print(" DST");
        }
        else {
          display.print(" STD");
        }
      }
      display.setCursor(20,36);
      display.print(date_str);
      display.display();
   }
}

unsigned char decToBcd(unsigned char val)
{
  return ( ((val/10)*16) + (val%10) );
}

#define colonDigit(digit) digit < 10 ? ":0" : ":"
void formattedTime(char *tod, int hours, int minutes, int seconds)
{
  sprintf(tod, "%d%s%d%s%d", hours, colonDigit(minutes), minutes, colonDigit(seconds), seconds);  // Hours, minutes, seconds
}

void updateEncoderPos() {
    static int encoderA, encoderB, encoderA_prev;   

    encoderA = digitalRead(encoderPinA); 
    encoderB = digitalRead(encoderPinB);
 
    if((!encoderA) && (encoderA_prev)){ // A has gone from high to low 
      encoderPosPrev = encoderPos;
      encoderB ? encoderPos++ : encoderPos--;  
      if (menu != TOP) {
        updateSelection();
      }    
    }
    encoderA_prev = encoderA;     
}

void updateMenu() {  // Called whenever button is pushed

  switch (menu) {
    case TOP:
      menu = SETTINGS;
      break;
    case SETTINGS:
      switch (mod(encoderPos,4)) {
        case 0: // Reset Wifi
          menu = RESET_WIFI;
          break;
        case 1: // Timezone Offset
          menu = SET_UTC_OFFSET;
          break;
        case 2: // Enable DST
          menu = ENABLE_DST;
          break;
        case 3: // Return
          menu = TOP;
          break;
      }
      break;
    case RESET_WIFI:
      if (mod(encoderPos, 2) == 1){  // Selection = YES
        resetWiFi();
        menu = TOP; 
      }
      else {  // Selection = NO
        menu = SETTINGS;
      }
      break;
    case SET_UTC_OFFSET:
      EEPROM.write(EEPROM_addr_UTC_offset, (unsigned char)(mod(mySTD.offset/60,24))); 
      EEPROM.commit();
      menu = TOP;
      break;
    case ENABLE_DST:
      if (mod(encoderPos, 2) == 1){  // Selection = YES
        enableDST = true;
      }
      else {  // Selection = NO
        enableDST = false;
      }
      EEPROM.write(EEPROM_addr_DST, (unsigned char)enableDST);
      EEPROM.commit();
      myDST.offset = mySTD.offset;
      if (enableDST) {
        myDST.offset += 60;
      }
      myTZ = Timezone(myDST, mySTD);   
      menu = TOP; 
      break;
  }
  encoderPos = 0;  // Reset encoder position
  encoderPosPrev = 0;
  updateSelection(); // Refresh screen
}

void updateSelection() { // Called whenever encoder is turned
  int UTC_STD_Offset, dispOffset;
  
  display.clearDisplay();
  switch (menu) {
    case TOP:
      display.setTextColor(WHITE,BLACK);
      display.setCursor(0,0);
      display.print("Wifi Connected");
      display.setCursor(0,56);
      display.print("Click for settings");
      break;
    case SETTINGS:
      display.setCursor(0,0); 
      display.setTextColor(WHITE,BLACK);
      display.print("SETTINGS");
      display.setCursor(0,16);
      setHighlight(0,4);
      display.println("Reset Wifi Connection");
      setHighlight(1,4);
      display.println("Set Timezone Offset  ");
      setHighlight(2,4);
      display.println("Enable Auto DST      ");
      setHighlight(3,4);
      display.println("Return               ");
      break;
    case RESET_WIFI:
      display.setCursor(0,0); 
      display.setTextColor(WHITE,BLACK);
      display.print("RESET WIFI?");
      display.setCursor(0,16);
      setHighlight(0,2);
      display.println("No                   ");
      setHighlight(1,2);
      display.println("Yes                  ");
      break;  
    case SET_UTC_OFFSET:
      UTC_STD_Offset = mySTD.offset/60;
      if (encoderPos > encoderPosPrev) {
        UTC_STD_Offset = ((UTC_STD_Offset + 12 + 1) % 24) - 12;
      } else if (encoderPos < encoderPosPrev) {
        UTC_STD_Offset = ((UTC_STD_Offset + 12 - 1) % 24) - 12;
      }
      mySTD.offset = UTC_STD_Offset * 60;
      myDST.offset = mySTD.offset;
      if (enableDST) {
        myDST.offset += 60;
      }
      myTZ = Timezone(myDST, mySTD);
      
      display.setCursor(0,0);
      display.setTextColor(WHITE,BLACK);
      display.println("SET TIMEZONE OFFSET");
      display.println();
      display.print("    UTC ");
      display.print(UTC_STD_Offset >= 0 ? "+ " : "- ");
      dispOffset = UTC_STD_Offset;
      if (enableDST) {
        if (myTZ.utcIsDST(timeClient.getEpochTime())) {
          dispOffset += 1;  // Include DST in UTC offset
        }
      }
      display.print(abs(dispOffset));
      display.print(" hours");
      displayTime();

      display.setCursor(0,48);
      display.println("Press knob to");
      display.print("confirm offset");
      break;
    case ENABLE_DST:
      display.setCursor(0,0); 
      display.setTextColor(WHITE,BLACK);
      if (enableDST) {
        display.print("Auto DST Enabled");
      }
      else {
        display.print("Auto DST Disabled");
      }
      display.setCursor(0,16);
      setHighlight(0,2);
      display.println("Disable Auto DST     ");
      setHighlight(1,2);
      display.println("Enable Auto DST      ");
      break;
  }
  display.display(); 
}

void resetWiFi(){
  WiFiManager MyWifiManager;
  MyWifiManager.resetSettings();
  ESP.restart();
}

void setHighlight(int menuItem, int numMenuItems) {
  if (mod(encoderPos, numMenuItems) == menuItem) {
    display.setTextColor(BLACK,WHITE);
  }
  else {
    display.setTextColor(WHITE,BLACK);
  }
}

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

void configModeCallback (WiFiManager *myWiFiManager) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("To configure Wifi,  ");
  display.println("connect to Wifi ");
  display.println("network ESPCLOCK and");
  display.println("open 192.168.4.1");
  display.println("in web browser");
  display.display(); 
}



