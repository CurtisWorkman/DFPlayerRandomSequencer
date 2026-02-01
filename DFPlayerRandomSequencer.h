/*
  DFPlayerRandomSequencer.h - Library for playing random sound sequences with DFPlayer Mini
  Created for ESP32-based projects
*/

#ifndef DFPlayerRandomSequencer_h
#define DFPlayerRandomSequencer_h

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

class DFPlayerRandomSequencer {
  public:
    // Constructor
    DFPlayerRandomSequencer(HardwareSerial &serial);
    
    // Initialization
    bool begin(int rxPin, int txPin, int volume = 10);
    
    // Configuration
    void setSequenceInterval(unsigned long interval);  // Time between sequences (ms)
    void setSoundDelayRange(int minDelay, int maxDelay);  // Delay between sounds (ms)
    void setSequenceLengthRange(int minSounds, int maxSounds);  // Number of sounds per sequence
    void setMaxSoundNumber(int maxSound);  // Max sound file number in folder
    void setFolder(int folderNumber);  // Which folder to play from
    void setVolume(int volume);  // Set volume (0-30)
    void setTimeout(unsigned long timeout);  // DFPlayer timeout
    
    // Control
    void update();  // Call this in loop()
    void startSequence();  // Manually trigger a sequence
    void stopSequence();  // Stop current sequence
    bool isPlayingSequence();  // Check if sequence is active
    
    // Getters
    int getSoundsInCurrentSequence();
    int getSoundsPlayedInCurrentSequence();
    unsigned long getTimeSinceLastSequence();
    
    // Enable/disable debug output
    void enableDebug(bool enable);
    
  private:
    DFRobotDFPlayerMini _dfPlayer;
    HardwareSerial* _serial;
    
    // Timing variables
    unsigned long _lastSoundTime;
    unsigned long _randomInterval;
    unsigned long _lastSequenceTime;
    unsigned long _sequenceInterval;
    
    // Sequence variables
    int _soundsToPlay;
    int _soundsPlayed;
    bool _isPlaying;
    
    // Configuration
    int _minDelay;
    int _maxDelay;
    int _minSounds;
    int _maxSounds;
    int _maxSoundNumber;
    int _folder;
    bool _debugEnabled;
    
    // Helper functions
    void _playNextSound();
    void _debugPrint(const char* message);
    void _debugPrintln(const char* message);
};

#endif
