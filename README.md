# DFPlayerRandomSequencer Library

A simple Arduino library for the DFPlayer Mini MP3 module that plays random sound sequences at configurable intervals. Perfect for ambient sound effects, animatronics, props, and interactive projects.

## Features

- **Automatic Sequences**: Plays random sound sequences at regular intervals
- **Configurable**: Customize sequence timing, sound delays, and number of sounds
- **Non-blocking**: Uses `millis()` for timing - won't block your main loop
- **Manual Control**: Trigger sequences manually or query playback status
- **Debug Mode**: Optional serial output for troubleshooting
- **Easy Integration**: Simple API that works alongside your other code

## Installation

### Arduino IDE
1. Download this library as a ZIP file
2. Open Arduino IDE
3. Go to Sketch → Include Library → Add .ZIP Library
4. Select the downloaded ZIP file
5. Restart Arduino IDE

### Manual Installation
1. Download or clone this repository
2. Copy the `DFPlayerRandomSequencer` folder to your Arduino libraries directory:
   - Windows: `Documents/Arduino/libraries/`
   - Mac: `Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
3. Restart Arduino IDE

## Hardware Requirements

- ESP32 (or compatible Arduino board with hardware serial)
- DFPlayer Mini MP3 module
- MicroSD card formatted as FAT32
- Speaker (3-5W recommended)
- Power supply (5V)

## Wiring

```
ESP32          DFPlayer Mini
GPIO 6    ---> TX
GPIO 7    ---> RX
5V        ---> VCC
GND       ---> GND

Speaker   ---> SPK_1 and SPK_2
```

## SD Card Setup

1. Format microSD card as FAT32
2. Create a folder named `01` (or `02`, `03`, etc.)
3. Place MP3 files in the folder, named as:
   - `001.mp3`
   - `002.mp3`
   - `003.mp3`
   - etc.

## Basic Usage

```cpp
#include "DFPlayerRandomSequencer.h"

DFPlayerRandomSequencer soundPlayer(Serial1);

void setup() {
  Serial.begin(9600);
  soundPlayer.enableDebug(true);
  
  // Initialize: RX pin, TX pin, volume (0-30)
  soundPlayer.begin(6, 7, 10);
  
  // Optional configuration
  soundPlayer.setSequenceInterval(10000);    // 10 seconds between sequences
  soundPlayer.setSoundDelayRange(80, 500);   // Delay between sounds
  soundPlayer.setSequenceLengthRange(3, 8);  // Number of sounds per sequence
}

void loop() {
  soundPlayer.update();  // Call this every loop!
}
```

## API Reference

### Initialization

**`DFPlayerRandomSequencer(HardwareSerial &serial)`**
- Constructor. Pass your hardware serial object (e.g., `Serial1`)

**`bool begin(int rxPin, int txPin, int volume = 10)`**
- Initialize the DFPlayer
- Returns `true` on success, `false` on failure
- Volume range: 0-30

### Configuration

**`setSequenceInterval(unsigned long interval)`**
- Set time between automatic sequences (milliseconds)
- Default: 10000 (10 seconds)

**`setSoundDelayRange(int minDelay, int maxDelay)`**
- Set random delay range between sounds (milliseconds)
- Default: 80-500ms

**`setSequenceLengthRange(int minSounds, int maxSounds)`**
- Set how many sounds play per sequence
- Default: 3-8 sounds

**`setMaxSoundNumber(int maxSound)`**
- Set highest numbered sound file to use
- Default: 50 (uses files 001.mp3 to 050.mp3)

**`setFolder(int folderNumber)`**
- Set which folder to play from
- Default: 1 (folder `01` on SD card)

**`setVolume(int volume)`**
- Change volume (0-30)

**`setTimeout(unsigned long timeout)`**
- Set DFPlayer timeout (milliseconds)
- Default: 1000ms

**`enableDebug(bool enable)`**
- Enable/disable serial debug output

### Control

**`update()`**
- **MUST be called in `loop()`** - handles all timing and playback

**`startSequence()`**
- Manually trigger a new sequence

**`stopSequence()`**
- Stop the current sequence

**`isPlayingSequence()`**
- Returns `true` if a sequence is currently playing

### Status

**`getSoundsInCurrentSequence()`**
- Returns total sounds in current sequence

**`getSoundsPlayedInCurrentSequence()`**
- Returns how many sounds have played so far

**`getTimeSinceLastSequence()`**
- Returns milliseconds since last sequence started

## Examples

### Example 1: Basic Halloween Prop

```cpp
#include "DFPlayerRandomSequencer.h"

DFPlayerRandomSequencer soundPlayer(Serial1);

void setup() {
  soundPlayer.begin(6, 7, 20);  // Louder volume
  soundPlayer.setSequenceInterval(15000);  // Every 15 seconds
  soundPlayer.setMaxSoundNumber(20);  // 20 spooky sounds
}

void loop() {
  soundPlayer.update();
}
```

### Example 2: Motion-Triggered Sounds

```cpp
#include "DFPlayerRandomSequencer.h"

DFPlayerRandomSequencer soundPlayer(Serial1);
const int PIR_PIN = 4;

void setup() {
  pinMode(PIR_PIN, INPUT);
  soundPlayer.begin(6, 7, 15);
  soundPlayer.setSequenceInterval(30000);  // 30 sec cooldown
}

void loop() {
  soundPlayer.update();
  
  // Trigger on motion (if not already playing)
  if (digitalRead(PIR_PIN) && !soundPlayer.isPlayingSequence()) {
    soundPlayer.startSequence();
  }
}
```

## Troubleshooting

**No sound plays**
- Check wiring connections
- Verify SD card is formatted as FAT32
- Ensure folder `01` exists with numbered MP3 files
- Enable debug mode: `soundPlayer.enableDebug(true);`

**DFPlayer fails to initialize**
- Check Serial1 RX/TX connections (might be swapped)
- Verify SD card is inserted
- Try a different SD card
- Check power supply (DFPlayer needs stable 5V)

**Sounds play too close together**
- Increase delay range: `setSoundDelayRange(200, 1000);`

**Not enough variety**
- Increase max sounds: `setMaxSoundNumber(100);`
- Add more MP3 files to your SD card

## Dependencies

- [DFRobotDFPlayerMini](https://github.com/DFRobot/DFRobotDFPlayerMini) library

## License

This library is released under the MIT License.

## Credits

Created for ESP32-based projects. Based on the DFRobot DFPlayer Mini library.
