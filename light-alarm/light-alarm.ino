/**
 * Alex Blackwell
 * November 1, 2020
 * Light Switch Robot
 *
 * This project is based on going to sleep at 12am and waking up at 8am.
 * The light will turn on at 8am and the display will let you know to go asleep at 12am.
 * The dialogue considers 4:59am as late and 5am as early (I've had some late nights).
 */


#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels


Servo servo;
int servoSpeed = 2;
int degreeOfRotation = 90;
int offset = 8.5;  // hour offset, plug in at 11pm and wake up at 8am

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup(void) {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    while(true);
  }

  servo.attach(9);  // use d-9 for servo signal

  toggleLight();

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.println("Waiting\nfor\n8am....");
  display.display();

  delay(offset*60*60*1000);  // launch at 6:30pm, activate at 8am
}


void loop(void) {
  countDown();
  toggleLight();
  // and that's all there is to it!
}


/**
 * Start a timer to trigger once a day at the same time.
 * Give the timer a dialogue with a wake up time of 8am
 * and a bed time of 12am.
 */
void countDown(void) {
  int hours = 24;
  int mins, secs;
  mins = secs = 0;

  while(hours > 0 || mins > 0 || secs > 0) {
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.clearDisplay();

    if(hours <= 8) {
      // let bed time be 12am
      display.setTextSize(2);
      display.println("It is:");
      if(hours == 8) {
        display.print(mins);
        display.print(" mins\nuntil your\nbedtime");
      }
      else if(hours > 3) {
        // let 5am be early and 4:59am be late
        // until an hour past, display in minutes
        if(hours == 7) {
          display.print(mins);
          display.print(" mins\npast\nbedtime");
        } else {
          // rounds down
          display.print(7-hours);
          if(hours == 6)  // one hour past
            display.print(" hour\npast your\nbedtime");
          else
            display.print(" hours\npast your\nbedtime");
        }
      }
      else if(hours >= 1) {
        display.print("Waking up in ");
        display.print(hours);
        display.print(" hour");
        if(hours > 1)
          display.print("s");
      } else {
        display.print("Waking up in ");
        display.print(mins);
        display.print("\nminute");
        if(mins != 1)
          display.print("s");
      }
    } else {
      display.print(hours-8);
      display.print(" hour");
      if(hours-8 > 1)
        display.print("s");
      display.println();

      display.print(mins);
      display.print(" minute");
      if(mins > 1)
        display.print("s");
      display.println();

      display.print(secs);
      display.print(" second");
      if(secs > 1)
        display.print("s");
      display.println();
    }

    display.display();

    if(--secs < 0) {
      if(mins == 0 && hours == 0)
        break;
      secs = 59;
      if(--mins < 0) {
        mins = 0;
        if(--hours < 0)
          hours = 0;
        else
          mins = 59;
      }
    }

    delay(945);  // in practice, calculations take some time
  }
}


/**
 *  Move the servo to toggle the light switch.
 */
void toggleLight(void) {

  int i;
  // rotate x degrees
  for(i = 0; i <= degreeOfRotation; ++i) {
    servo.write(i);
    delay(servoSpeed);
  }
  // rotate back to origin
  for(i = degreeOfRotation; i >= 0; --i) {
    servo.write(i);
    delay(servoSpeed);
  }

  return;
}
