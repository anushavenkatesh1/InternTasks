#include <TinyWireM.h>
#include <Tiny4kOLED.h>
#include <TinyRTClib.h>

RTC_DS1307 rtc;

// Define arrays for days of the week and months
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char monthOfTheYear[12][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Variables to store previous time
int prevHour = -1;
int prevMinute = -1;

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
