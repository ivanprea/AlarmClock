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

![HumidityLedsLogic](https://github.com/ivanprea/AlarmClock/assets/78477048/8658bab2-1e98-49b6-9ad3-2e585d580f90)


## Schematic:

![AlarmClockSchematic](https://github.com/ivanprea/AlarmClock/assets/78477048/d377a258-9a2e-4ab2-8b17-9ed5b26733d9)


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
- V0 to a potentiometer (middle pin) for contrast adjustment (other two pins to +5V and GND)
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
- Button 3 (Alarm):
  One pin to pin 13 and the other to GNDthrough a current-limiting 10k resistor
- Button 4 (Set Alarm):
  One pin to pin 10 and the other to GND through a current-limiting 10k resistor


**LEDs:**
- LED1:
  Anode to Analog Pin A0 and Cathode to GND through a current-limiting 5k resistor
- LED2:
  Anode to Analog Pin A1 and Cathode to GND through a current-limiting 5k resistor
- LED3:
  Anode to Analog Pin A2 and Cathode to GND through a current-limiting 5k resistor


**Buzzer:**
- Positive tol Pin 9
- Negative to GND


