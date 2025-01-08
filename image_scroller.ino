#include <SD.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

MCUFRIEND_kbv tft;

const int leftButtonPin = 2;     // blue
const int rightButtonPin = 3;    // red
const int leftLedPin = 9;
const int rightLedPin = 8;

int leftButtonState = 0;
int rightButtonState = 0;
bool scroll = 0;
int counter = 0;
int i = 0;

int imageWidth = 320; 
int imageHeight = 480;

// Image filenames on the SD card
const char* imageNames[] = {
  "kris",
  "cat_red",
  "cat_bl~6"
};

File imageFile;  // File object for the image
const int numImages = sizeof(imageNames) / sizeof(imageNames[0]); // Number of images

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
  const int ID = 0x9486;
  tft.begin(ID);
  tft.fillScreen(TFT_GREEN);

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

  // displayImage("rgb_cat");
  
  // Display image on TFT
  // displayImage("/rgb_cat");  // Assuming your image is saved as "image.rgb565"
  // listFiles(SD.open("/"), 0);

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

  // Implement logic to scroll image
  if (scroll == 1) {
    // Wait 1 second before displaying next image
    counter +=1;
    counter %= 10;
    if (counter == 0) {
      i ++;
      i %= numImages;
      // Open image file from SD card.
      imageFile = SD.open(imageNames[i]);
      if (imageFile) {
        Serial.print("Displaying image: ");
        Serial.println(imageNames[i]);

        // Assuming binary file; you can modify this to support other formats as needed.
        displayImage(imageNames[i]);

        // Close the image file after display
        imageFile.close();
      } else {
        Serial.print("Error opening file: ");
        Serial.println(imageNames[i]);
      }
    }
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

  delay(100);
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

  for (int y = 0; y < imageHeight; y++) {
      imgFile.read(tempBuffer, bufferSize * 2);
      for (int i = 0; i < bufferSize; i++) {
        pixelBuffer[i] = (tempBuffer[2*i] << 8) | tempBuffer[2*i + 1];
      }
      
      tft.setAddrWindow(0, y, imageWidth, y);

      tft.pushColors(pixelBuffer, imageWidth, true);
  }

  // Close the image file
  imgFile.close();
}