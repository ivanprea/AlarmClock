# ALARM CLOCK w Temp&Hum

This code sets up an Arduino-based alarm clock inspired by an Elegoo project (https://www.elegoo.com/en-fr/blogs/learn/tutorial-arduino-alarm-clock) with additional features. This project has been made possible with the help of ChatGPT.

## Components:

- **Arduino UNO Board**
  
- **RTC DS3231 Module** (also reads temperature)
  
- **LCD Display 1602A**
  
- **DHT22 Humidity Sensor** (for reading humidity only)
  
- **4 Push buttons**:
    - **Set Button** (btSet): Opens time/date and alarm setup. Allows the user to navigate between hour/minute/etc. Press again to save and exit setup mode after setting the alarm.
    - **Night Mode Button** (btnNightM): Toggles the backlight of the LCD display and LEDs (used to indicate humidity levels).
    - **Alarm Button** (btAlarm): Toggles the alarm function on and off.
    - **Alarm Set/Save Button** (btAlarmSet): Directly accesses the alarm setup mode. When in time/date or alarm setup if pressed on anyvalue it saves directly.
      
- **4x10k resistors** for the buttons
  
- **3xLEDs**
  
- **3x5k resistors** for the LEDs (choose the resistor based on the desired LED brightness)
  
- **Buzzer**
  
- **NPN PN2222 Transistor**
  
- **Potentiometer**
  
- **Wires**

  

## Himidity leds logic:

**!IMPORTANT!** 

The design to display the humidity percentage via LEDs is overcomplicated. Aesthetic preference was chosen over readability. The initial idea was to use three LEDs (Yellow 30%-40%, Green 40%-50%, and Red 50%-60%), but to maintain a minimal design with matching blue LEDs for the LCD, an alternative solution was implemented. Although this design indicates a more accurate range of humidity percentages, it may not be as intuitive. If you replicate this project, you are not obligated to use this logic for displaying humidity.

![HumidityLedsLogicSmall](https://github.com/ivanprea/AlarmClock/assets/78477048/b64416f5-2de8-4b26-a354-be3a1e812d87)



## Schematic:

![AlarmClockSchematic](https://github.com/ivanprea/AlarmClock/assets/78477048/11613fd0-6c07-4829-9e22-0358411c37f3)


## Bugs:
*Feel free to make any adjustment to the code to improve it*

- When entering the Time/Date setup, after setting the Year, if the Set Button is clicked again, it mistakenly goes to the Alarm setup before saving. This should be avoided. The Set Button should only allow the user to set Time/Date. The Alarm Set/Save Button should only allow the user to set and save the alarm.

- When entering the setup mode and exiting, whether or not any modifications are made, the clock's seconds automatically reset to zero.

- Buzzer will ring only for one minute.
