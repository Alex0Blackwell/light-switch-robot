#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


Servo servo;
int servoSpeed = 2;
int degreeOfRotation = 90;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup(void) {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    while(true);
  }

  servo.attach(9);  // use d-9

  toggleLight();
}


void loop(void) {
  countDown();
  toggleLight();
}


/**
 * Start a timer to trigger once a day at the same time.
 */
void countDown(void) {
  int hours = 24;
  int mins, secs;
  mins = secs = 0;

  while(hours > 0 || mins > 0 || secs > 0) {
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.setTextColor(WHITE);
    display.clearDisplay();

    display.print(hours);
    display.print(" hour");
    if(hours > 1)
      display.print("s");
    display.println();

    display.print(mins);
    display.print(" min");
    if(mins > 1)
      display.print("s");
    display.println();

    display.print(secs);
    display.print(" sec");
    if(secs > 1)
      display.print("s");
    display.println();

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

    delay(1000);
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
