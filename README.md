# ALARM CLOCK w Temp&Hum

This code sets up an Arduino-based alarm clock inspired by an Elegoo project (https://www.elegoo.com/en-fr/blogs/learn/tutorial-arduino-alarm-clock) with additional features. This project has been made possible with the help of ChatGPT.

## Components:

- **Arduino UNO Board**
  
- **RTC DS3231 Module** (also reads temperature)
  
- **LCD Display 1602A**
  
- **DHT22 Humidity Sensor** (for reading humidity only)
  
- **5 Push buttons**
      
- **5x10k resistors** for the buttons
  
- **1xLED**
  
- **1x5k resistors** for the LED (choose the resistor based on the desired LED brightness)
  
- **Buzzer**
  
- **NPN PN2222 Transistor**
  
- **2x5k resistor for pin VO LCD (I tried to put a one 10k resistor but it woudn't work, i don't why)**
  
- **Wires**

  




## Schematic:

![AlarmClockSchematic](https://github.com/ivanprea/AlarmClock/assets/78477048/011ff17d-dbac-4276-9ee1-4159d862a182)



## Complete Wiring Summary:

**LCD Display 1602A:**
- RS to Pin 2
- E to Pin 3
- D4 to Pin 4
- D5 to Pin 5
- D6 to Pin 6
- D7 to Pin 7
- VSS to GND
- VDD to +5V
- V0 to a 2x5k resitors then to GND (in the schematic is a single 10k but with it it does not work, that's why i put 2x5k)
- A (Anode, backlight) to +5V through
- K (Cathode, backlight) to Collector of NPN 2222


**NPN 2222 Transistor:**
- Collector (C): Connect to the cathode (K) of the LCD backlight
- Emitter (E): Connect to GND
- Base (B): Connect to a resistor to Pin 8


**DS3231 RTC Module:**
- SDA to SDA
- SCL to SCL
- VCC to +5V
- GND to GND


**DHT22 Sensor:**
- VCC to +5V
- GND to GND
- Data to Pin A3


**Push Buttons:**
- Button 1 (Set DateTime):
  One pin to pin 11 and the other to GND through a current-limiting 10k resistor
- Button 2 (Night Mode):
  One pin to pin 12 and the other to GND through a current-limiting 10k resistor
- Button 3 (Snooze):
  One pin to pin A1 and the other to GND through a current-limiting 10k resistor
- Button 4 (Alarm):
  One pin to pin 13 and the other to GNDthrough a current-limiting 10k resistor
- Button 5 (Set Alarm):
  One pin to pin 10 and the other to GND through a current-limiting 10k resistor


**LED:**
  Anode to Analog Pin A= and Cathode to GND through a current-limiting 5k resistor


**Buzzer:**
- Positive tol Pin 9
- Negative to GND


## How it works:
The primary function of the display is to provide essential information such as the current time, date, alarm hour, and temperature/humidity readings. The status of the alarm (ON/OFF) is visually indicated by a bell icon located to its left and the LED ON.

Here's how the buttons operate:

Alarm Button (btAlarm): Toggles the alarm function on and off. During datetime setup or alarm/snooze configuration, pressing this button decreases the selected value.

Night Mode Button (btnNightM): Controls the backlight of the LCD display and the state of the LED (which still blinks when the buzzer is active). In setup modes, pressing this button increases the selected value.

Snooze Button (btnSnooze): Initiates the snooze function, turning off the LED. During Datetime or Alarm/Snooze setup, pressing it selects the previous value.

Set Button (btSet): Opens the Datetime setup. Allows the user to progress to the next value. Upon reaching the year value, pressing it again saves and exits. After saving, the clock seconds reset to 00.

Alarm Set (btAlarmSet): Opens the Alarm and Snooze setup mode. Pressing it again directly saves the settings. The clock seconds do not reset to 00 in this mode. To reach the Snooze setup after reaching the minute of the alarm , press the Set Button. 

The LED indicator illuminates when the alarm state is ON, as indicated by the bell icon next to the alarm hour. It blinks when the buzzer is active. Although the LED can be turned OFF using the Night Mode button, it will still blink when the buzzer is active.
When the buzzer is active, a custom message will be displayed on the screen until the alarm is turned off or snoozed.



      
