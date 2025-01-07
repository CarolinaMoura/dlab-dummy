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
bool scroll = 0;
int counter = 0;
int i = 0;

int imageWidth = 320;  // Width of your image (modify as per your image size)
int imageHeight = 480; // Height of your image (modify as per your image size)

// Image filenames on the SD card
const char* imageNames[] = {
  "rgb_cat", 
  "rgb_cat2", 
  "rgb_cat",
  "rgb_cat2"
};

File imageFile;  // File object for the image
const int numImages = sizeof(imageNames) / sizeof(imageNames[0]); // Number of images

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
  const int ID = 0x9486;
  tft.begin(ID);
  tft.fillScreen(~TFT_GREEN);

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
  
  // Display image on TFT
  // displayImage("/rgb_cat");  // Assuming your image is saved as "image.rgb565"
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
      i %= 4;
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

// Function to read and display an image from the SD card
void displayImage(const char* filename) {
  File imgFile = SD.open(filename);
  if (!imgFile) {
    Serial.println("Error opening image file!");
    return;
  }

  // Read image pixel by pixel and display on TFT screen
  for (int y = 0; y < imageHeight; y++) {
    for (int x = 0; x < imageWidth; x++) {
      // Read 2 bytes for each pixel (RGB565 format)
      uint16_t pixelData = imgFile.read() << 8 | imgFile.read();  // Combine the two bytes

      // Extract RGB565 components
      uint8_t r = (pixelData >> 11) & 0x1F;  // Red (5 bits)
      uint8_t g = (pixelData >> 5) & 0x3F;   // Green (6 bits)
      uint8_t b = pixelData & 0x1F;          // Blue (5 bits)

      // Scale components to 8 bits (for TFT compatibility)
      r = (r << 3) | (r >> 2);
      g = (g << 2) | (g >> 4);
      b = (b << 3) | (b >> 2);

      // Set the pixel color on the TFT screen
      tft.drawPixel(x, y, pixelData);
    }
  }

  // Close the image file
  imgFile.close();
}
