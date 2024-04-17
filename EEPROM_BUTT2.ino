#include <TinyWireM.h>
#include <Tiny4kOLED.h>
#include <TinyRTClib.h>
#include <avr/sleep.h>

RTC_DS1307 rtc;

// Define arrays for days of the week and months
const char* daysOfTheWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char* monthOfTheYear[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Pin for the buzzer and buttons
#define BUZZER_PIN 1
#define BUTTON_PIN 3
#define DISPLAY_BUTTON_PIN 4

bool isButtonPressed = false;
bool isDisplayButtonPressed = false;
DateTime buttonPressTime;

void setup() {
  oled.begin(128, 64, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
  oled.clear();
  oled.on();

  if (!rtc.begin()) {
    oled.setCursor(0, 0);
    oled.print("RTC not found!");
    while (1);
  }

  if (!rtc.isrunning()) {
    char date[] = __DATE__;
    char time[] = __TIME__;
    rtc.adjust(DateTime(date, time));
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(DISPLAY_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  DateTime now = rtc.now();
  oled.clear();
  oled.setFontX2Smooth(FONT8X16P);
  oled.setCursor(0, 0);
  print_time(now.hour(), now.minute(), now.second());
  oled.setCursor(0, 4);
  oled.setFontX2Smooth(FONT6X8P);
  oled.print(daysOfTheWeek[now.dayOfWeek()]);
  oled.print(",");
  oled.print(now.day());
  oled.print(",");
  oled.print(monthOfTheYear[now.month() - 1]);
  oled.print(now.year() % 100);

  if (digitalRead(BUTTON_PIN) == LOW && !isButtonPressed) {
    isButtonPressed = true;
    buttonPressTime = now;
    displayAlarmSetMessage();
    delay(1000);
  }

  if (((now.hour() == 18 && now.minute() >= 0) || (now.hour() == 19 && now.minute() <= 30)) && isButtonPressed) {
    trigger_buzzer();
  }

  if (digitalRead(DISPLAY_BUTTON_PIN) == LOW && !isDisplayButtonPressed) {
    isDisplayButtonPressed = true;
    displayButton1PressTime(buttonPressTime);
    delay(1000);
  }
}

void print_time(int hrs, int minu, int sec) {
  if (hrs < 10) oled.print('0');
  oled.print(hrs);
  oled.print(":");
  if (minu < 10) oled.print('0');
  oled.print(minu);
  oled.print(":");
  if (sec < 10) oled.print('0');
  oled.print(sec);
}

void trigger_buzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}

void displayAlarmSetMessage() {
  oled.clear();
  oled.setFont(FONT8X16P);
  oled.setCursor(0, 16);
  oled.print("Alarm Set for 18:00 to 19:30");
}

void displayButton1PressTime(DateTime buttonPressTime) {
  oled.clear();
  oled.setFont(FONT8X16P);
  oled.setCursor(0, 16);
  oled.print("Button 1 Press Time:");
  oled.setCursor(0, 32);
  oled.print(daysOfTheWeek[buttonPressTime.dayOfWeek()]);
  oled.print(", ");
  oled.print(buttonPressTime.day());
  oled.print(" ");
  oled.print(monthOfTheYear[buttonPressTime.month() - 1]);
  oled.print(" ");
  oled.print(buttonPressTime.year());
  oled.setCursor(0, 48);
  oled.print(buttonPressTime.hour());
  oled.print(":");
  if (buttonPressTime.minute() < 10) oled.print('0');
  oled.print(buttonPressTime.minute());
}
