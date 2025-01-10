#include <SD.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include <Fonts/FreeSans12pt7b.h>  // Include the desired font header
// #include <TMRpcm.h>    // option 1

#define WHITE 0xFFFF
#define BLACK 0x0
#define RED 0xF800

#define audioBufferSize 512 // Buffer size for reading audio data

MCUFRIEND_kbv tft;
// TMRpcm audio;  // option 1

const int audioPin = 12;        // audio pin

File wavFile;
byte audioBuffer[audioBufferSize];
const int sampleRate = 16000; // Adjust to match your WAV file's sample rate

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
  const int ID = 0x9486;
  tft.begin(ID);
  tft.fillScreen(~TFT_WHITE);

  
  // Initialize SD card
  if (!SD.begin(53)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  tft.setRotation(2);

  pinMode(audioPin, OUTPUT);   // Configure PWM pin  // option 2
  
  // audio.speakerPin = audioPin;  // option 1
  // audio.setVolume(1);   // option 1
  // if (!audio.CSPin) {    // option 1
  //   audio.CSPin = 53;    // option 1
  // }

}

void loop() {
  tft.fillScreen(~RED);
  playAudio("kris_wav.wav");
  tft.fillScreen(~TFT_WHITE);
  delay(4000);
}

// option 1
// void playAudio(const char *filename) {
//   if (SD.exists(filename)) {
//     audio.play(filename); // Play the audio file

//     // Wait until playback is finished
//     while (audio.isPlaying()) {
//       delay(100); // Small delay to prevent freezing
//     }

//     audio.stopPlayback(); // Ensure playback is stopped and file is closed
//   } else {
//     Serial.print("File not found...");
//   }
// }

//option 2
// Function to play audio
void playAudio(const char *filename) {
  File audioFile = SD.open(filename);
  if (!audioFile) {
    Serial.print("File not found: ");
    Serial.println(filename);
    return;
  }

  Serial.print("Playing: ");
  Serial.println(filename);

  // Read and output audio data
  while (audioFile.available()) {
    uint8_t sample = audioFile.read(); // Read one byte
    uint8_t volume = 128; // Adjust between 0 (mute) and 255 (full volume)
    // analogWrite(audioPin, (sample * volume) / 255);
    analogWrite(audioPin, sample);    // Output to PWM pin
    Serial.println(sample);
    delayMicroseconds(62);           // Adjust for sample rate (e.g., 8 kHz = 125 µs)
  }

  audioFile.close();
  Serial.println("Playback finished.");
}
