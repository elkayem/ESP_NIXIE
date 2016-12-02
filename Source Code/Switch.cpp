/*
Switch.cpp
Copyright (C) 2012  Albert van Dalen http://www.avdweb.nl
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .
 
Version 20-4-2013
_debouncePeriod=50
Version 22-5-2013
Added longPress, doubleClick
Version 1-12-2015
added process(input)
Version 15-1-2016
added deglitching
 
..........................................DEGLITCHING..............................
                                           
                        ________________   _
               on      |                | | |    _                        
                       |                | | |   | |                                      
                       |                |_| |___| |__                                                           
 analog        off_____|_____________________________|____________________________  
                   
                        ________________   _     _
 input            _____|                |_| |___| |_______________________________               
            
 poll            ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^   
 
 equal           0 1 1 0 1 1 1 1 1 1 1 1 0 0 0 1 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
 
 deglitchPeriod          <--------><--   <--     <-  <--------><--------><--------
                                    ___________________________
 deglitched       _________________|                           |__________________
 
 deglitchTime            ^         ^     ^       ^   ^         ^        ^
 
 ..........................................DEBOUNCING.............................
 
 debouncePeriod                    <-------------------------------->    
                                    _________________________________
 debounced        _________________|                                 |____________  
                                    _                                 _
 _switched        _________________| |_______________________________| |__________       
                                                     
 switchedTime                      ^                                 ^ 
  
 
**********************************************************************************
........................................DOUBLE CLICK..............................
                                         
                           __________         ______                           
 debounced        ________|          |_______|      |_____________________________  
 
 poll            ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^          
                           _                  _
 pushed          _________| |________________| |__________________________________       
                                                     
 pushedTime               ^                  ^ 
 
 doubleClickPeriod         <------------------------------------->                     
                                              _
 _doubleClick     ___________________________| |__________________________________
 
                             
........................................LONG PRESS................................
                                          
                           ___________________________                                     
 debounced        ________|                           |___________________________        
 
 poll            ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^             
         
 longPressPeriod            <--------------->         
                            _                           _
 _switched        _________| |_________________________| |________________________       
                                              __________
 longPressDisable ___________________________|          |_________________________                                  
                                              _
 _longPress       ___________________________| |__________________________________        
  
*/
 
//#include "Arduino.h"
#include "Switch.h"
               
Switch::Switch(unsigned char _pin, unsigned char PinMode, bool polarity, int debouncePeriod, int longPressPeriod, int doubleClickPeriod, int deglitchPeriod):
pin(_pin), polarity(polarity), deglitchPeriod(deglitchPeriod), debouncePeriod(debouncePeriod), longPressPeriod(longPressPeriod), doubleClickPeriod(doubleClickPeriod)
{ pinMode(pin, PinMode);
  switchedTime = millis();
  debounced = digitalRead(pin);
}
  
bool Switch::poll()
{ input = digitalRead(pin);
  return process();
}
 
bool Switch::process()
{ deglitch();
  debounce();
  calcDoubleClick();
  calcLongPress();
  return _switched;
}
 
void inline Switch::deglitch()
{ ms = millis();
  if(input == lastInput) equal = 1;
  else
  { equal = 0;
    deglitchTime = ms;
  }
  if(equal & ((ms - deglitchTime) > deglitchPeriod)) // max 50ms, disable deglitch: 0ms
  { deglitched = input;
    deglitchTime = ms;
  }
  lastInput = input; 
}
 
void inline Switch::debounce()
{ ms = millis();
  _switched = 0;
  if((deglitched != debounced) & ((ms - switchedTime) >= debouncePeriod))
  { switchedTime = ms;
    debounced = deglitched;
    _switched = 1;
    longPressDisable = false;
  }
}
 
void inline Switch::calcDoubleClick()
{ _doubleClick = false;
  if(pushed())
  { _doubleClick = (ms - pushedTime) < doubleClickPeriod; // pushedTime of previous push
    pushedTime = ms;
  }
}
 
void inline Switch::calcLongPress()
{ _longPress = false;
  if(!longPressDisable)
  { _longPress = on() && ((ms - pushedTime) > longPressPeriod); // true just one time between polls
    longPressDisable = _longPress; // will be reset at next switch  
  } 
}
 
bool Switch::switched()
{ return _switched;
}
 
bool Switch::on()
{ return !(debounced^polarity);
}
 
bool Switch::pushed()
{ return _switched && !(debounced^polarity);
}
 
bool Switch::released()
{ return _switched && (debounced^polarity);
}
 
bool Switch::longPress()
{ return _longPress;
}
 
bool Switch::doubleClick()
{ return _doubleClick;
}
