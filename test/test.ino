#include <SD.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include <Fonts/FreeSans12pt7b.h> 
#include "DLabImage.h"
#define INVERT_COLORS false

#define WHITE 0xFFFF
#define BLACK 0x0
#define RED 0xF800
#define LIGHT_GREEN 0x2727

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
File currentDir;
String filenames[2] = {"colors", "moods"};
int filenamePtr = 0;

// Image filenames on the SD card
const char* imageNames[] = {
  // "hambre",
  // "agua",
  // "dolor",
  // "dormir"
  "colors/blue",
  "colors/pink",
  "colors/yellow",
  "colors/green",
  "colors/red",
  "colors/orange",
};

// const char* imageNames[] = {
//   "moods/hungry",
//   "moods/gratitud",
//   "moods/happy",
//   "moods/funny",
// };

uint16_t adjustColor(uint16_t color) {
    return INVERT_COLORS ? ~color : color;
}

File imageFile;  // File object for the image
const int numImages = sizeof(imageNames) / sizeof(imageNames[0]); // Number of images

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
  const int ID = 0x9486;
  tft.begin(ID);
  tft.fillScreen(adjustColor(TFT_WHITE));

  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);

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
  
  // listFiles(SD.open("/"), 0);
  
  // img.drawImage()
  currentDir = SD.open(filenames[filenamePtr++]);
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

  if (leftButtonState == LOW && leftButtonLastState == HIGH) {
    digitalWrite(leftLedPin, HIGH);
    tft.fillScreen(adjustColor(WHITE));
    counter += 1;
    counter %= numImages;
    displayImage(imageNames[counter]);
  } else {
    digitalWrite(leftLedPin, LOW);
  }

  if (rightButtonState == LOW) {
    digitalWrite(rightLedPin, HIGH);
    uint16_t color = LIGHT_GREEN;
    // if(isRedSquareOn) color = WHITE;
    // isRedSquareOn=!isRedSquareOn;
    drawSquare(color);
  } else {
    digitalWrite(rightLedPin, LOW);
  }
}

void drawSquare(uint16_t color) {
  uint16_t thickness = 10;
  tft.fillRect(0, 0, SCREEN_WIDTH, thickness, color);
  tft.fillRect(0, 0 + SCREEN_HEIGHT - thickness, SCREEN_WIDTH, thickness, color);
  tft.fillRect(0, 0, thickness, SCREEN_HEIGHT, color);
  tft.fillRect(0 + SCREEN_WIDTH - thickness, 0, thickness, SCREEN_HEIGHT, color);
}

void displayImage(const char* filename) {
  DLabImage img(filename, SD);
  img.drawImage(tft, SD, false);
}
