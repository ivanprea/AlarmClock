// Libraries
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "DHT.h"

// LCD Pins 
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


// Symbols
byte clocksymbol[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b01110,
  0b11111,
  0b00000,
  0b00100,
  0b00000,
};

byte temp[8] = {
  0b10000,
  0b00111,
  0b01000,
  0b01000,
  0b01000,
  0b01000,
  0b00111,
  0b00000,
};

// DS3231 i2c module
RTC_DS3231 rtc;

// DHT22 setup
#define DHTPIN A3 // Pin where the DHT22 is connected
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Buttons pins
const int btAlarmSet = 10; 
const int btSet = 11;
const int btnNightM = 12;
const int btAlarm = 13;
const int buzzer = 9; 
const int isbacklight = 8; 
const int npnBase = 8; 


// LEDs pins
const int led1 = A0;
const int led2 = A1;
const int led3 = A2;


// Variables
int DD, MM, YY, H, M, S, set_state, adjust_state, alarm_state, AH, AM, shake_state;
int shakeTimes = 0;
int i = 0;
int btnCount = 0;
int alarm_set_state;
String sDD;
String sMM;
String sYY;
String sH;
String sM;
String sS;
String aH = "12";
String aM = "00";
String alarm = "     ";
long previousMillis = 0;
long interval = 400; 
float tempC;
double tempK;
int tempReading;
unsigned long starttime;
float temperature;
float humidity;


// Boolean flags
boolean setupScreen = false;
boolean alarmON = false;
boolean turnItOn = false;
boolean backlightON = true;
bool isSettingDateTime = false; 

void setup() {
  initDS3231();
  initLCD();
  initBTNs();
  initLeds();
  initBuzz();
  initDTH22();
  Serial.begin(9600);
  delay(100);
  AH = EEPROM.read(0);
  AM = EEPROM.read(1);
  if (AH > 23) {
    AH = 0;
  }
  if (AM > 59) {
    AM = 0;
  }
}
void initLeds() { 
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  }
void initBTNs() { 
  pinMode(btSet, INPUT_PULLUP);
  pinMode(btnNightM, INPUT_PULLUP);
  pinMode(btAlarm, INPUT_PULLUP);
  pinMode(btAlarmSet, INPUT_PULLUP); 
  }
void initLCD() { 
  lcd.begin(16, 2);
  lcd.createChar(1, clocksymbol);
  lcd.createChar(2, temp);
  digitalWrite(isbacklight, HIGH);
  }
void initDS3231() { 
  rtc.begin();
  }
void initDTH22() { 
  dht.begin(); // Initialize the DHT sensor
  }
void initBuzz() { 
  pinMode(buzzer, OUTPUT);
  }
void loop() {
  readBtns();       // Read buttons
  getTimeDate();    // Read time and date from RTC
  if (!setupScreen) {
    lcdPrint();     // Normally print the current time/date/alarm to the LCD
    if (alarmON) {
      callAlarm();   // and check the alarm if set on
    } else {
      lcd.setCursor(10, 0);
      lcd.write(" ");
    }
  } else {
    timeSetup();  // If button set is pressed then call the time setup function
  }

  // Read and print temperature and humidity values every minute
  unsigned long currentMillis = millis();
  if (currentMillis - starttime >= 1500) { 
    starttime = currentMillis;  // Reset the timer
    measureTemperature();  // Read temperature
    measureHumidity();     // Read humidity
    Serial.begin(9600);  // Initialize serial communication
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.end();  // End serial communication
  }
}


/*************** Functions ****************/
void measureTemperature() {
  temperature = rtc.getTemperature(); // Get temperature from DS3231
  tempC = temperature;
  Serial.begin(9600);  // Initialize serial communication
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");
  Serial.end();  // End serial communication
}

void measureHumidity() {
  humidity = dht.readHumidity(); // Get humidity from DHT22

  if (backlightON) {
    LEDsHum();
  } else {
    // Ensure all LEDs are off when backlight is off
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }

  Serial.begin(9600);  // Initialize serial communication
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.end();  // End serial communication
}

// LEDs Humidity
void LEDsHum() {
  if (humidity < 30) {
     digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led1, LOW);
    delay(200);
    digitalWrite(led1, HIGH);
    delay(200);
    digitalWrite(led1, LOW);
    delay(200);
    digitalWrite(led1, HIGH);
    
  } else if (humidity >= 30 && humidity < 35) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  } else if (humidity >= 35 && humidity < 40) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
  } else if (humidity >= 40 && humidity < 45) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
  } else if (humidity >= 45 && humidity < 50) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
  } else if (humidity >= 50 && humidity < 55) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
  } else if (humidity >= 55 && humidity < 60) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
  } else {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    delay(200);
    digitalWrite(led3, HIGH);
    delay(200);
    digitalWrite(led3, LOW);
    delay(200);
    digitalWrite(led3, HIGH);
    
  }
}

// BUTTONS
// Read buttons state
void readBtns() {
  btnsDatetime();
  btnsNightMode();
  if (!isSettingDateTime) {
    btnsAlarm();
  }
  alarmToggle();
}
// Time & Date
void btnsDatetime() {
  set_state = digitalRead(btSet);
  adjust_state = digitalRead(btnNightM);
  alarm_state = digitalRead(btAlarm);
  
  if (set_state == LOW) {
    if (!setupScreen) {
      setupScreen = true;
      isSettingDateTime = true; 
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Set up");
      lcd.setCursor(4, 1);
      lcd.print("DATETIME");
      delay(950);
      lcd.clear();
      btnCount = 1; 
      lcd.clear();
      delay(200);
    } else {
      if (btnCount <= 5) {
        btnCount++;
        delay(300);
        if (btnCount == 6) {
          lcd.clear();
          rtc.adjust(DateTime(YY, MM, DD, H, M, 0));
          lcd.print("   Saving....");
          delay(800);
          lcd.clear();
          setupScreen = false;
          isSettingDateTime = false; 
          btnCount = 0;
        }
      }
    }
  }
}
void btnsAlarm() {
  if (isSettingDateTime) return; 
  
  adjust_state = digitalRead(btnNightM);
  alarm_state = digitalRead(btAlarm);
  alarm_set_state = digitalRead(btAlarmSet);

  
  if (alarm_set_state == LOW) {
    if (!setupScreen) {
      setupScreen = true;
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Set up");
      lcd.setCursor(3, 1);
      lcd.print("ALARM Time");
      delay(950);
      btnCount = 6; 
      lcd.clear();
      delay(200);
    } else {
      lcd.clear();
      DateTime now = rtc.now(); 
      rtc.adjust(DateTime(YY, MM, DD, H, M, now.second())); 
      EEPROM.write(0, AH);  
      EEPROM.write(1, AM);  
      lcd.print("   Saving....");
      delay(800);
      lcd.clear();
      setupScreen = false;
      btnCount = 0;
    }
    delay(500);
  }
}
void btnsNightMode() {
  adjust_state = digitalRead(btnNightM);
  
  if (!setupScreen) {
    if (adjust_state == LOW) {
      if (backlightON) {
        backlightON = false;
        digitalWrite(isbacklight, LOW);
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
      } else {
        backlightON = true;
        digitalWrite(isbacklight, HIGH);
        LEDsHum();
      }
      delay(500);
    }
  }
}
void alarmToggle() {
  alarm_state = digitalRead(btAlarm);
  
  if (!setupScreen) {
      if (alarm_state == LOW) {
       if (alarmON) {
         alarm = "     ";
         alarmON = false;
         }  else {
         alarmON = true;
        }
        delay(500);
      }
     }
    }


// Read time and date from DS3231 RTC
void getTimeDate() {
  if (!setupScreen) {
    DateTime now = rtc.now();
    DD = now.day();
    MM = now.month();
    YY = now.year();
    H = now.hour();
    M = now.minute();
    S = now.second();
  }
  // Make some fixes...
  if (DD < 10) {
    sDD = '0' + String(DD);
  } else {
    sDD = DD;
  }
  if (MM < 10) {
    sMM = '0' + String(MM);
  } else {
    sMM = MM;
  }
  sYY = YY - 2000  ;
  if (H < 10) {
    sH = '0' + String(H);
  } else {
    sH = H;
  }
  if (M < 10) {
    sM = '0' + String(M);
  } else {
    sM = M;
  }
  if (S < 10) {
    sS = '0' + String(S);
  } else {
    sS = S;
  }
  if (AH < 10) {
    aH = '0' + String(AH);
  } else {
    aH = AH;
  }
  if (AM < 10) {
    aM = '0' + String(AM);
  } else {
    aM = AM;
  }
}

// LCD Print values to the display
void lcdPrint() {
  // Convert the temperature to a formatted string with one decimal place
  char tempStr[6]; // A string of 6 characters should be sufficient to format the temperature
  dtostrf(tempC, 4, 1, tempStr); // Convert the float value tempC to a string with one decimal place
  
  String line1 = sH + ":" + sM + ":" + sS + " |";
  lcd.setCursor(0, 0); // First line
  lcd.print(line1);
  if (alarmON) {
    lcd.setCursor(10, 0);
    lcd.write(1);
  }
  String line2 = aH + ":" + aM;
  lcd.setCursor(11, 0);
  lcd.print(line2);

  String line3 = sDD + "-" + sMM + "-" + sYY + " | " + tempStr; // Use the formatted string for the temperature
  lcd.setCursor(0, 1); // Second line
  lcd.print(line3);
  if (setupScreen) {
    lcd.setCursor(13, 1);
    lcd.print("");
  } else {
    lcd.setCursor(15, 1);
    lcd.write(2);
  }
}



// TIME & DATE SETUP
void timeSetup() {
  int up_state = adjust_state;
  int down_state = alarm_state;
  if (btnCount <= 5) {
    if (btnCount == 1) { // Set Hour
      lcd.setCursor(4, 0);      lcd.print(">");
      if (up_state == LOW) { // Up button +
        if (H < 23) {
          H++;
        } else {
          H = 0;
        }
        delay(350);
      }
      if (down_state == LOW) { // Down button -
        if (H > 0) {
          H--;
        } else {
          H = 23;
        }
        delay(350);
      }
    } else if (btnCount == 2) { // Set Minutes
      lcd.setCursor(4, 0);
      lcd.print(" ");
      lcd.setCursor(9, 0);
      lcd.print(">");
      if (up_state == LOW) {
        if (M < 59) {
          M++;
        } else {
          M = 0;
        }
        delay(350);
      }
      if (down_state == LOW) {
        if (M > 0) {
          M--;
        } else {
          M = 59;
        }
        delay(350);
      }
    } else if (btnCount == 3) { // Set Day
      lcd.setCursor(9, 0);
      lcd.print(" ");
      lcd.setCursor(1, 1);
      lcd.print(">");
      
      if (up_state == LOW) {
        if (DD < 31) {
          DD++;
        } else {
          DD = 1;
        }
        delay(350);
      }
      if (down_state == LOW) {
        if (DD > 1) {
          DD--;
        } else {
          DD = 31;
        }
        delay(350);
      }
    } else if (btnCount == 4) { // Set Month
      lcd.setCursor(1, 1);
      lcd.print(" ");
      lcd.setCursor(6, 1);
      lcd.print(">");
      
      if (up_state == LOW) {
        if (MM < 12) {
          MM++;
        } else {
          MM = 1;
        }
        delay(350);
      }
      if (down_state == LOW) {
        if (MM > 1) {
          MM--;
        } else {
          MM = 12;
        }
        delay(350);
      }
    } else if (btnCount == 5) { // Set Year
      lcd.setCursor(6, 1);
      lcd.print(" ");
      lcd.setCursor(11, 1);
      lcd.print(">");
      
      if (up_state == LOW) {
        if (YY < 2999) {
          YY++;
        } else {
          YY = 2000;
        }
        delay(350);
      }
      if (down_state == LOW) {
        if (YY > 2018) {
          YY--;
        } else {
          YY = 2999;
        }
        delay(350);
      }
    }
    lcd.setCursor(5, 0);
    lcd.print(sH);
    lcd.setCursor(8, 0);
    lcd.print(":");
    lcd.setCursor(10, 0);
    lcd.print(sM);
    lcd.setCursor(2, 1);
    lcd.print(sDD);
    lcd.setCursor(4, 1);
    lcd.print(" -");
    lcd.setCursor(7, 1);
    lcd.print(sMM);
    lcd.setCursor(9, 1);
    lcd.print(" -");
    lcd.setCursor(12, 1);
    lcd.print(sYY);
  } else {
    setAlarmTime();
  }
}


// ALARM SETUP
void setAlarmTime() {
  int up_state = adjust_state;
  int down_state = alarm_state;

  
  lcd.setCursor(0, 0);
  lcd.write(1); //Clocksymbol

 
  lcd.setCursor(1, 0);
  lcd.print("Set your Alarm:");

  lcd.setCursor(15, 0);
  lcd.write(1); //Clocksymbol
  String line2;

  if (btnCount == 6) { // Set alarm Hour
    if (up_state == LOW) {
      if (AH < 23) {
        AH++;
      } else {
        AH = 0;
      }
      delay(350);
    }
    if (down_state == LOW) {
      if (AH > 0) {
        AH--;
      } else {
        AH = 23;
      }
      delay(350);
    }
    line2 = "    >" + aH + " : " + aM + "    ";
  } else if (btnCount == 7) { // Set alarm Minutes
    if (up_state == LOW) {
      if (AM < 59) {
        AM++;
      } else {
        AM = 0;
      }
      delay(350);
    }
    if (down_state == LOW) {
      if (AM > 0) {
        AM--;
      } else {
        AM = 59;
      }
      delay(350);
    }
    line2 = "     " + aH + " :" + ">" + aM + " ";
  }

  lcd.setCursor(0, 1);
  lcd.print(line2);

  if (set_state == LOW && btnCount == 6) {
    btnCount = 7; // Move to setting minutes after setting hours
    delay(200);
  }
}


// ALARM TOGGLE
void callAlarm() {
  static unsigned long lastMillis = 0;
  static bool beepState = false;

  if (aH == sH && aM == sM) {
    turnItOn = true;
  }

  if (turnItOn) {
    unsigned long currentMillis = millis();

    // Check if it's time to toggle the beep state
    if (beepState && (currentMillis - lastMillis >= 200)) { // Beep duration in ms
      noTone(buzzer);
      beepState = false;
      lastMillis = currentMillis;
    } else if (!beepState && (currentMillis - lastMillis >= 200)) { // Pause duration in ms
      tone(buzzer, 1000); // Emit beep sound
      beepState = true;
      lastMillis = currentMillis;
    }
  }

  // Turn off the buzzer and reset states when the alarm button is pressed
  if (digitalRead(btAlarm) == LOW) {
    noTone(buzzer);
    turnItOn = false;
    beepState = false;
    lastMillis = 0;
  }
}



