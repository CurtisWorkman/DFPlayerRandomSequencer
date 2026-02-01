#include "DFRobotDFPlayerMini.h"
#define FPSerial Serial1
DFRobotDFPlayerMini myDFPlayer;

// Timing variables
unsigned long lastSoundTime = 0;
unsigned long randomInterval = 0;
unsigned long lastSequenceTime = 0;
const unsigned long SEQUENCE_INTERVAL = 10000;  // 10 seconds

// Sequence variables
int soundsToPlay = 0;
int soundsPlayed = 0;
bool isPlaying = false;

void setup() {
  Serial.begin(9600);
  randomSeed(esp_random());

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  FPSerial.begin(9600, SERIAL_8N1, /*rx =*/6, /*tx =*/7);

  if (!myDFPlayer.begin(FPSerial, /*isACK = */ true, /*doReset = */ true)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(1000);
    }
  }

  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.setTimeOut(1000);
  myDFPlayer.volume(10);

  // Initialize timing
  lastSequenceTime = millis();
  randomInterval = random(80, 500);
}

void loop() {
  // Check if 10 seconds have passed and no sequence is currently playing
  if (!isPlaying && (millis() - lastSequenceTime >= SEQUENCE_INTERVAL)) {
    // Start new sequence
    soundsToPlay = random(3, 8);  // Random between 5 and 10
    soundsPlayed = 0;
    isPlaying = true;
    lastSoundTime = millis();
    lastSequenceTime = millis();  // Reset the 10-second timer
    randomInterval = 0;           // Play first sound immediately

    Serial.print("Starting sequence! Will play ");
    Serial.print(soundsToPlay);
    Serial.println(" sounds");
  }

  // Play sounds if sequence is active
  if (isPlaying && (millis() - lastSoundTime > randomInterval)) {
    if (soundsPlayed < soundsToPlay) {
      int sound = random(1, 50);

      Serial.print("Playing sound ");
      Serial.print(soundsPlayed + 1);
      Serial.print(" of ");
      Serial.print(soundsToPlay);
      Serial.print(": ");
      Serial.println(sound);

      myDFPlayer.playFolder(1, sound);
      soundsPlayed++;
      lastSoundTime = millis();

      // Generate next interval
      randomInterval = random(80, 500);
      Serial.print("Next interval: ");
      Serial.println(randomInterval);
    } else {
      // Sequence complete
      isPlaying = false;
      Serial.println("Sequence complete!");
      Serial.println("Waiting for next 10-second interval...");
    }
  }
}