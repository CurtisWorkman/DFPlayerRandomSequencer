/*
  DFPlayerRandomSequencer - Advanced Example
  
  This example shows how to manually control sequences and
  integrate the library with other code.
*/

#include "DFPlayerRandomSequencer.h"

DFPlayerRandomSequencer soundPlayer(Serial1);

const int BUTTON_PIN = 2;  // Button to manually trigger sequence
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  Serial.println(F("DFPlayer Advanced Example"));
  
  soundPlayer.enableDebug(true);
  
  if (!soundPlayer.begin(6, 7, 15)) {
    Serial.println(F("Failed to initialize!"));
    while (true) delay(1000);
  }
  
  // Custom configuration
  soundPlayer.setSequenceInterval(20000);      // 20 seconds between auto sequences
  soundPlayer.setSoundDelayRange(100, 800);    // Longer delays between sounds
  soundPlayer.setSequenceLengthRange(5, 10);   // More sounds per sequence
  soundPlayer.setMaxSoundNumber(100);          // More sound files available
  soundPlayer.setFolder(2);                    // Use folder 02
  
  Serial.println(F("Setup complete! Press button to trigger manual sequence."));
}

void loop() {
  // Update the sequencer
  soundPlayer.update();
  
  // Check button for manual trigger
  bool buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50);  // Debounce
    
    if (!soundPlayer.isPlayingSequence()) {
      Serial.println(F("Button pressed - starting manual sequence!"));
      soundPlayer.startSequence();
    } else {
      Serial.println(F("Sequence already playing..."));
    }
  }
  lastButtonState = buttonState;
  
  // Example: Print status every 5 seconds
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 5000) {
    if (!soundPlayer.isPlayingSequence()) {
      Serial.print(F("Idle - Next auto sequence in: "));
      Serial.print((soundPlayer.getTimeSinceLastSequence() / 1000));
      Serial.println(F(" seconds"));
    } else {
      Serial.print(F("Playing sound "));
      Serial.print(soundPlayer.getSoundsPlayedInCurrentSequence());
      Serial.print(F(" of "));
      Serial.println(soundPlayer.getSoundsInCurrentSequence());
    }
    lastPrint = millis();
  }
  
  // Your other code can go here
  // The library runs non-blocking in the background
}
