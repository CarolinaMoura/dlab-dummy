#include <SD.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

const int chipSelect = 53;  // Arduino pin connected to SD card CS (TFT port 34)
const char* f_name = "check";

void setup() {
  Serial.begin(250000);  // Initialize serial communication
  const int ID = 0x9486;
  tft.begin(ID);
  tft.fillScreen(TFT_GREEN);
  Serial.println("Arduino ready to receive image data...");

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
  } else {
    Serial.println("SD card initialized.");
  }

  // createFile(f_name);
  // checkFileExistence(f_name);
  listFiles(SD.open("/"), 0);

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

void checkFileExistence(const char* filename) {
  if (SD.exists(filename)) {
    Serial.println("File exists.");
  } else {
    Serial.println("File doesn't exist.");
  }
}

void createFile(const char* filename) {
  Serial.println(filename);
  File file = SD.open(filename, FILE_WRITE);  // Open the file in write mode
  if (file) {
    Serial.println("File created successfully.");
    file.close();
  } else {
    Serial.println("Failed to create file.");
  }
}

void readFromSerial(){
  if(Serial.available()){
    const char* filename = f_name;
    if(SD.exists(filename)){
      SD.remove(filename);
      Serial.println("Removed the filename");
    } else Serial.println("Didn't find it");

    File file = SD.open(filename, FILE_WRITE);
    Serial.println("Created it");
    
    unsigned long lastByteTime = millis();

    while (true) {
      if (Serial.available()) {
        char c = Serial.read();
        file.write(c);
        lastByteTime = millis(); // Update the time of the last received byte
      }

      // Timeout if no data is received for 2 seconds
      if (millis() - lastByteTime > 2000) {
        break;
      }
    }

    file.close();
    Serial.println("File write complete.");
  }
}

void displayImageFromSD(const char *filename) {
  File file = SD.open(filename, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file!");
    return;
  }

  // Dimensions of the image
  int16_t imgWidth = 2;
  int16_t imgHeight = 2;

  // Ensure the image fits on the screen
  int16_t screenWidth = tft.width();
  int16_t screenHeight = tft.height();


  uint16_t buffer[imgWidth];  // Buffer for one row of pixels

  Serial.println(file.size());

  for (int y = 0; y < imgHeight; y++) {
    // Read one row of pixel data (2 bytes per pixel)
    file.read((uint8_t *)buffer, imgWidth * 2);

    // Push the row to the display
    tft.setAddrWindow(0, y, imgWidth - 1, y);  // Set the row window
    tft.pushColors(buffer, imgWidth, true);    // Push the row of pixels
  }

  file.close();
  Serial.println("Image displayed successfully.");
}

void loop() {
  readFromSerial();
//   displayImageFromSD(f_name);
}
