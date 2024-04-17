#include <TinyWireM.h>
#include <Tiny4kOLED.h>
#include <TinyRTClib.h>
#include <avr/sleep.h>
#include <EEPROM.h>

RTC_DS1307 rtc;

// Define arrays for days of the week and months
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char monthOfTheYear[12][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Pin for the buzzer
#define BUZZER_PIN 1

// Alarm times (hour, minute) for Button 1 and Button 2
int alarmTime1[] = {18,3}; 
int alarmTime2[] = {18,5}; 

// Button pins
#define BUTTON_PIN_1 3
#define BUTTON_PIN_2 4

bool isButtonPressed1 = false;
bool isButtonPressed2 = false;

void setup() {
  // OLED display initialization
  oled.begin(128, 64, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
  oled.clear();
  oled.on();

  // Initialize RTC
  if (!rtc.begin()) {
    oled.setCursor(0, 0);
    oled.print("RTC not found!");
    while (1);
  }

  // If RTC is not running, set it to the current time
  if (!rtc.isrunning()) {
    // Adjust the time using the current date and time
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  // Set button pins as inputs
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
}

void loop() {
  // Fetch current time from RTC
  DateTime now = rtc.now();

  // Update OLED display every second
  oled.clear();
  oled.setFontX2Smooth(FONT8X16P);
  oled.setCursor(0, 0);

  // Print current time
  print_time(now.hour(), now.minute(), now.second());
  oled.setCursor(0, 4);
  oled.setFontX2Smooth(FONT6X8P);

  // Print day, date, and month
  oled.print(daysOfTheWeek[now.dayOfWeek()]);
  oled.print(",");
  oled.print(now.day());
  oled.print(",");
  oled.print(monthOfTheYear[now.month() - 1]);
  oled.print(now.year() % 100);

  // Check button states to set alarm times
  if (digitalRead(BUTTON_PIN_1) == LOW && !isButtonPressed1) {
    // Button 1 pressed, set alarm time and display "Alarm Set"
    isButtonPressed1 = true;
    isButtonPressed2 = false; // Reset button 2 state
    displayAlarmSetMessage(alarmTime1[0], alarmTime1[1]);
    delay(1000); // Display "Alarm Set" for 1 second
  }

  if (digitalRead(BUTTON_PIN_2) == LOW && !isButtonPressed2) {
    // Button 2 pressed, set alarm time and display "Alarm Set"
    isButtonPressed2 = true;
    isButtonPressed1 = false; // Reset button 1 state
    displayAlarmSetMessage(alarmTime2[0], alarmTime2[1]);
    delay(1000); // Display "Alarm Set" for 1 second
  }

  // Check if it's time for any of the alarms
  if ((now.hour() == alarmTime1[0] && now.minute() == alarmTime1[1] && isButtonPressed1) ||
      (now.hour() == alarmTime2[0] && now.minute() == alarmTime2[1] && isButtonPressed2)) {
    trigger_buzzer();
  }
}

void print_time(int hrs, int minu, int sec) {
  // Print hours, minutes, and seconds
  if (hrs < 10) oled.print('0');
  oled.print(hrs);
  oled.print(":");
  if (minu < 10) {
    oled.print('0');
  }
  oled.print(minu);
  oled.print(":");
  if (sec < 10) {
    oled.print('0');
  }
  oled.print(sec);
}
void setAlarm(int hour, int minute) {
  // Set the alarm time and display "Alarm Set" message
  displayAlarmSetMessage(hour, minute);
  delay(1000); // Display "Alarm Set" for 1 second
}
void trigger_buzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
  delay(1000); // Buzzer on for 10 seconds
  digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
}

void displayAlarmSetMessage(int hour, int minute) {
  // Display "Alarm Set" message with the given hour and minute
  oled.clear();
  oled.setFont(FONT8X16P); // Set font size
  oled.setCursor(0, 16); // Adjust cursor position
  oled.print("Alarm Set for "); // Print the message
  if (hour < 10) oled.print('0'); // Print leading zero for hour if necessary
  oled.print(hour);
  oled.print(":");
  if (minute < 10) oled.print('0'); // Print leading zero for minute if necessary
  oled.print(minute);
}
