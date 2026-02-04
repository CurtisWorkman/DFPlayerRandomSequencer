/*
  DFPlayerRandomSequencer.cpp - Library for playing random sound sequences with DFPlayer Mini
  Created for ESP32-based projects
*/

#include "DFPlayerRandomSequencer.h"

DFPlayerRandomSequencer::DFPlayerRandomSequencer(HardwareSerial &serial) {
  _serial = &serial;
  
  // Default configuration
  _sequenceInterval = 10000;  // 10 seconds
  _minDelay = 80;
  _maxDelay = 500;
  _minSounds = 3;
  _maxSounds = 8;
  _maxSoundNumber = 50;
  _folder = 1;
  _debugEnabled = false;
  
  // Initial state
  _isPlaying = false;
  _soundsToPlay = 0;
  _soundsPlayed = 0;
  _lastSoundTime = 0;
  _lastSequenceTime = 0;
  _randomInterval = 0;
  _sequencingActive = false;
}

bool DFPlayerRandomSequencer::begin(int rxPin, int txPin, int volume) {
  randomSeed(esp_random());
  
  _serial->begin(9600, SERIAL_8N1, rxPin, txPin);
  
  if (_debugEnabled) {
    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  }
  
  if (!_dfPlayer.begin(*_serial, true, true)) {
    if (_debugEnabled) {
      Serial.println(F("Unable to begin DFPlayer"));
      Serial.println(F("1. Please recheck the connection!"));
      Serial.println(F("2. Please insert the SD card!"));
    }
    return false;
  }
  
  if (_debugEnabled) {
    Serial.println(F("DFPlayer Mini online."));
  }
  
  _dfPlayer.setTimeOut(1000);
  _dfPlayer.volume(volume);
  
  // Initialize timing
  _lastSequenceTime = millis();
  _randomInterval = random(_minDelay, _maxDelay);
  
  return true;
}

void DFPlayerRandomSequencer::startSequencing() {
  _sequencingActive = true;
  _lastSequenceTime = millis();
  
  if (_debugEnabled) {
    Serial.println(F("Sequencing started"));
  }
}

void DFPlayerRandomSequencer::stopSequencing() {
  _sequencingActive = false;
  _isPlaying = false;
  _soundsPlayed = 0;
  _soundsToPlay = 0;
  
  if (_debugEnabled) {
    Serial.println(F("Sequencing stopped"));
  }
}

bool DFPlayerRandomSequencer::isSequencingActive() {
  return _sequencingActive;
}

void DFPlayerRandomSequencer::setSequenceInterval(unsigned long interval) {
  _sequenceInterval = interval;
}

void DFPlayerRandomSequencer::setSoundDelayRange(int minDelay, int maxDelay) {
  _minDelay = minDelay;
  _maxDelay = maxDelay;
}

void DFPlayerRandomSequencer::setSequenceLengthRange(int minSounds, int maxSounds) {
  _minSounds = minSounds;
  _maxSounds = maxSounds;
}

void DFPlayerRandomSequencer::setMaxSoundNumber(int maxSound) {
  _maxSoundNumber = maxSound;
}

void DFPlayerRandomSequencer::setFolder(int folderNumber) {
  _folder = folderNumber;
}

void DFPlayerRandomSequencer::setVolume(int volume) {
  _dfPlayer.volume(volume);
}

void DFPlayerRandomSequencer::setTimeout(unsigned long timeout) {
  _dfPlayer.setTimeOut(timeout);
}

void DFPlayerRandomSequencer::enableDebug(bool enable) {
  _debugEnabled = enable;
}

void DFPlayerRandomSequencer::update() {
  // Only run if sequencing is active
  if (!_sequencingActive) {
    return;
  }
  
  // Check if it's time to start a new sequence
  if (!_isPlaying && (millis() - _lastSequenceTime >= _sequenceInterval)) {
    startSequence();
  }
  
  // Play sounds if sequence is active
  if (_isPlaying && (millis() - _lastSoundTime > _randomInterval)) {
    if (_soundsPlayed < _soundsToPlay) {
      _playNextSound();
    } else {
      // Sequence complete
      stopSequence();
    }
  }
}

void DFPlayerRandomSequencer::startSequence() {
  _soundsToPlay = random(_minSounds, _maxSounds);
  _soundsPlayed = 0;
  _isPlaying = true;
  _lastSoundTime = millis();
  _lastSequenceTime = millis();
  _randomInterval = 0;  // Play first sound immediately
  
  if (_debugEnabled) {
    Serial.print(F("Starting sequence! Will play "));
    Serial.print(_soundsToPlay);
    Serial.println(F(" sounds"));
  }
}

void DFPlayerRandomSequencer::stopSequence() {
  _isPlaying = false;
  
  if (_debugEnabled) {
    Serial.println(F("Sequence complete!"));
    Serial.println(F("Waiting for next interval..."));
  }
}

bool DFPlayerRandomSequencer::isPlayingSequence() {
  return _isPlaying;
}

int DFPlayerRandomSequencer::getSoundsInCurrentSequence() {
  return _soundsToPlay;
}

int DFPlayerRandomSequencer::getSoundsPlayedInCurrentSequence() {
  return _soundsPlayed;
}

unsigned long DFPlayerRandomSequencer::getTimeSinceLastSequence() {
  return millis() - _lastSequenceTime;
}

void DFPlayerRandomSequencer::_playNextSound() {
  int sound = random(1, _maxSoundNumber + 1);
  
  if (_debugEnabled) {
    Serial.print(F("Playing sound "));
    Serial.print(_soundsPlayed + 1);
    Serial.print(F(" of "));
    Serial.print(_soundsToPlay);
    Serial.print(F(": "));
    Serial.println(sound);
  }
  
  _dfPlayer.playFolder(_folder, sound);
  
  _soundsPlayed++;
  _lastSoundTime = millis();
  
  // Generate next interval
  _randomInterval = random(_minDelay, _maxDelay);
  
  if (_debugEnabled) {
    Serial.print(F("Next interval: "));
    Serial.println(_randomInterval);
  }
}

void DFPlayerRandomSequencer::_debugPrint(const char* message) {
  if (_debugEnabled) {
    Serial.print(message);
  }
}

void DFPlayerRandomSequencer::_debugPrintln(const char* message) {
  if (_debugEnabled) {
    Serial.println(message);
  }
}
