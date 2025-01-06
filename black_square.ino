#include <SD.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

void setup() {
  Serial.begin(250000);
  const int ID = 0x9486;
  tft.begin(ID);
  tft.fillScreen(TFT_GREEN);
  Serial.println("Arduino ready to receive image data...");
  
  for(int i = 0; i < 100; i++)
    for(int j = 0; j < 100; j++){
      tft.drawPixel(i,j,0);
    }
}

void loop() {
}
