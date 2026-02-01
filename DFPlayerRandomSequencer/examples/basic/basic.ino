/*
  DFPlayerRandomSequencer - Basic Example
  
  This example demonstrates how to use the DFPlayerRandomSequencer library
  to play random sound sequences at regular intervals.
  
  Hardware Connections (ESP32):
  - DFPlayer RX -> GPIO 7
  - DFPlayer TX -> GPIO 6
  - DFPlayer VCC -> 5V
  - DFPlayer GND -> GND
  - Speaker -> DFPlayer SPK_1 and SPK_2
*/

#include "DFPlayerRandomSequencer.h"

// Create sequencer object using Serial1
DFPlayerRandomSequencer soundPlayer(Serial1);

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println(F("DFPlayer Random Sequencer Example"));
  
  // Enable debug output
  soundPlayer.enableDebug(true);
  
  // Initialize the player (RX pin, TX pin, volume)
  if (!soundPlayer.begin(6, 7, 10)) {
    Serial.println(F("Failed to initialize DFPlayer!"));
    while (true) {
      delay(1000);
    }
  }
  
  // Configure the sequencer (optional - these are the defaults)
  soundPlayer.setSequenceInterval(10000);      // 10 seconds between sequences
  soundPlayer.setSoundDelayRange(80, 500);     // 80-500ms between sounds
  soundPlayer.setSequenceLengthRange(3, 8);    // 3-8 sounds per sequence
  soundPlayer.setMaxSoundNumber(50);           // Sound files numbered 1-50
  soundPlayer.setFolder(1);                    // Play from folder 01
  
  Serial.println(F("Setup complete!"));
}

void loop() {
  // This is all you need in your loop!
  soundPlayer.update();
  
  // You can still do other things here
  // The library handles all the timing automatically
}
