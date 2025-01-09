#include <SD.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include <Fonts/FreeSans12pt7b.h>  // Include the desired font header

#define WHITE 0xFFFF
#define BLACK 0x0
#define RED 0xF800

#define CHAR_SZ 6

MCUFRIEND_kbv tft;

int SCREEN_WIDTH = 320;
int SCREEN_HEIGHT = 480;

const int leftButtonPin = 2;     // blue
const int rightButtonPin = 3;    // red
const int leftLedPin = 9;
const int rightLedPin = 8;

int leftButtonState = 0;
int rightButtonState = 0;
bool isRedSquareOn = false;
int counter = 0;

int imageWidth = 320; 
int imageHeight = 320;

// Image filenames on the SD card
const char* imageNames[] = {
  "hambre",
  "agua",
  "dolor"

};

File imageFile;  // File object for the image
const int numImages = sizeof(imageNames) / sizeof(imageNames[0]); // Number of images

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
  const int ID = 0x9486;
  tft.begin(ID);
  tft.fillScreen(TFT_WHITE);

  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);

  // Initialize SD card
  if (!SD.begin(53)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  tft.setRotation(2);
  // int tmp = SCREEN_WIDTH;
  // SCREEN_WIDTH = SCREEN_HEIGHT;
  // SCREEN_HEIGHT = tmp;
  // displayImage("kris_2x3");
  
  // Display image on TFT
  // displayImage("/rgb_cat");  // Assuming your image is saved as "image.rgb565"
  // listFiles(SD.open("/"), 0);
  displayImage(imageNames[counter]);

}

void listFiles(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();  // Open the next file
    if (!entry) {
      // No more files
      break;
    }
    for (int i = 0; i < numTabs; i++) {
      Serial.print('\t');  // Print tabs for subdirectory levels
    }
    Serial.print(entry.name());  // Print file name
    if (entry.isDirectory()) {
      Serial.println("/");  // Indicate it's a directory
      listFiles(entry, numTabs + 1);  // Recursively list files in the directory
    } else {
      // Files have a size
      Serial.print("\t");
      Serial.print(entry.size());
      Serial.println(" bytes");
    }
    entry.close();
  }
}

void loop() {
  // Read button states
  leftButtonState = digitalRead(leftButtonPin);
  rightButtonState = digitalRead(rightButtonPin);

  if (leftButtonState == LOW) {
    digitalWrite(leftLedPin, HIGH);
    tft.fillScreen(WHITE);
    counter += 1;
    counter %= numImages;
    displayImage(imageNames[counter]);
  } else {
    digitalWrite(leftLedPin, LOW);
  }

  if (rightButtonState == LOW) {
    digitalWrite(rightLedPin, HIGH);
    drawSquare(RED);
  } else {
    digitalWrite(rightLedPin, LOW);
  }
}

void drawSquare(uint16_t color) {
  uint16_t thickness = 5;
  tft.fillRect(0, 0, SCREEN_WIDTH, thickness, color);
  tft.fillRect(0, 0 + SCREEN_HEIGHT - thickness, SCREEN_WIDTH, thickness, color);
  tft.fillRect(0, 0, thickness, SCREEN_HEIGHT, color);
  tft.fillRect(0 + SCREEN_WIDTH - thickness, 0, thickness, SCREEN_HEIGHT, color);
}

void displayImage(const char* filename) {
  File imgFile = SD.open(filename);
  if (!imgFile) {
    Serial.println("Error opening image file!");
    return;
  }

  const int bufferSize = imageWidth;
  uint16_t pixelBuffer[bufferSize];
  uint8_t tempBuffer[bufferSize * 2];

  for (int y = 0; y < imageHeight; y++){
      imgFile.read(tempBuffer, bufferSize * 2);
      for (int i = 0; i < bufferSize; i++) {
        pixelBuffer[i] = (tempBuffer[2*i] << 8) | tempBuffer[2*i + 1];
      }
      
      int starting_pix = (SCREEN_WIDTH-imageWidth)>>1;
      tft.setAddrWindow(starting_pix, y, starting_pix+imageWidth, y);

      tft.pushColors(pixelBuffer, imageWidth, true);
  }

  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.setFont(&FreeSans12pt7b);  // Set the custom font
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(filename, 0,0, &x1, &y1, &w, &h);
  Serial.println("Text bounds: " + String(w));
  // int str_hor_px = CHAR_SZ * 5 * strlen(filename);
  tft.setCursor((SCREEN_WIDTH-w)>>1, imageHeight+20+(h>>1));
  tft.print(filename);

  // Close the image file
  imgFile.close();
}