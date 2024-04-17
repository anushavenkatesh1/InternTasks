#include <TinyWireM.h>
#include <Tiny4kOLED.h>
#include <TinyRTClib.h>
#include <avr/sleep.h>
#include <EEPROM.h>

RTC_DS1307 rtc;

// Define arrays for days of the week and months
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char monthOfTheYear[12][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Pin for the buzzer and button
#define BUZZER_PIN 1
#define BUTTON_PIN 3

bool isButtonPressed = false;

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

  // Set button pin as input
  pinMode(BUTTON_PIN, INPUT_PULLUP);
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

  // Check button state to set alarm time
  if (digitalRead(BUTTON_PIN) == LOW && !isButtonPressed) {
    // Button pressed, set alarm time and display "Alarm Set" message
    isButtonPressed = true;
    displayAlarmSetMessage();
    delay(1000); // Display "Alarm Set" for 1 second
  }

  // Check if it's time for the alarm within the interval 18:00 to 19:30
  // {
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

void trigger_buzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
  delay(1000); // Buzzer on for 10 seconds
  digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
}

void displayAlarmSetMessage() {
  // Display "Alarm Set" message
  oled.clear();
  oled.setFont(FONT8X16P); // Set font size
  oled.setCursor(0, 16); // Adjust cursor position
  oled.print("Alarm Set for 18:00 to 19:30"); // Print the message
}
