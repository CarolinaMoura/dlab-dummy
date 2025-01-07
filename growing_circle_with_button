#include <SD.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

const int leftButtonPin = 2;     // blue
const int rightButtonPin = 3;    // red
const int leftLedPin = 9;
const int rightLedPin = 8;

int leftButtonState = 0;
int rightButtonState = 0;
int rad = 300 / 2;
bool scroll = 0;
int change = -10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
  const int ID = 0x9486;
  tft.begin(ID);
  tft.fillScreen(~TFT_GREEN);
  tft.drawCircle(320 / 2, 480 / 2, rad, 0);
  Serial.println("Arduino ready to receive image data...");
  
  // for(int i = 0; i < 100; i++) {
  //   for(int j = 0; j < 100; j++){
  //     tft.drawPixel(i,j,0);
  //   }
  // }
  
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  leftButtonState = digitalRead(leftButtonPin);
  rightButtonState = digitalRead(rightButtonPin);

  if (scroll == 1) {
    if (rad == 20) {
        change = 10;
    }
    if (rad == 150) {
      change = -10;
    }
    rad += change;
    tft.fillScreen(~TFT_GREEN);
    tft.drawCircle(320 / 2, 480 / 2, rad, 0);
    delay(100);
  }

  if (leftButtonState == HIGH) {
    digitalWrite(leftLedPin, HIGH);
    scroll = 1;
  } else {
    digitalWrite(leftLedPin, LOW);
  }

  if (rightButtonState == HIGH) {
    digitalWrite(rightLedPin, HIGH);
    scroll = 0;
  } else {
    digitalWrite(rightLedPin, LOW);
  }
  
}
