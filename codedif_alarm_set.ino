#include <TinyWireM.h>
#include <Tiny4kOLED.h>
#include <TinyRTClib.h>
#include <avr/sleep.h>

RTC_DS1307 rtc;

// Define arrays for days of the week and months
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char monthOfTheYear[12][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Variables to store previous time
int prevHour = -1;
int prevMinute = -1;

// Pin for the buzzer
#define BUZZER_PIN 1

// Alarm times (hour, minute)
int alarmTime1[] = {15, 6}; // Default alarm time for button 1
int alarmTime2[] = {15, 8};  // Default alarm time for button 2

// Button pins
#define BUTTON_PIN_1 3
#define BUTTON_PIN_2 4

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

  // Check if it's time for any of the alarms
  if (now.hour() == alarmTime1[0] && now.minute() == alarmTime1[1]) {
    trigger_buzzer();
  }

  if (now.hour() == alarmTime2[0] && now.minute() == alarmTime2[1]) {
    trigger_buzzer();
  }

  // Check button states to set alarm times
  if (digitalRead(BUTTON_PIN_1) == LOW) {
    // Button 1 pressed, set alarm time
    setAlarmTime(1);
  }

  if (digitalRead(BUTTON_PIN_2) == LOW) {
    // Button 2 pressed, set alarm time
    setAlarmTime(2);
  }

  delay(1000); // Update once per second
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

void trigger_buzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
  delay(10000); // Buzzer on for 10 seconds
  digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
}

void setAlarmTime(int buttonNumber) {
  // Fetch current time from RTC
  DateTime now = rtc.now();

  // Check which button is pressed and set corresponding alarm time
  if (buttonNumber == 1) {
    alarmTime1[0] = now.hour();
    alarmTime1[1] = now.minute();
  } else if (buttonNumber == 2) {
    alarmTime2[0] = now.hour();
    alarmTime2[1] = now.minute();
  }
}
