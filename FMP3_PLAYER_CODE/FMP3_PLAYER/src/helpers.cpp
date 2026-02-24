#include "helpers.h"
#include <RDA5807M.h>
#include <LiquidCrystal.h>

/**
 * Print a popup message on the LCD.
 */
void print_lcd_message(LiquidCrystal& lcd, const char* message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
}

/**
 * Initialize the FM radio and set the initial frequency and volume.
 * Turns on the transistor to power the FM radio and turns off the MP3 player.
 * If initialization fails, it displays an error message on the LCD and halts the program (Power Cycle Required).
 */
void initFM(RDA5807M& radio, LiquidCrystal& lcd) {
  digitalWrite(FM_TRANSISTOR_PIN, LOW); // Turn on the transistor to power the FM radio
  digitalWrite(MP3_TRANSISTOR_PIN, HIGH); // Turn off the MP3 player
  print_lcd_message(lcd, "Initializing FM...");

  radio._wireDebug(true);
  radio.debugEnable(true);

  if (!radio.initWire(Wire)) {
    print_lcd_message(lcd, "FM Init Failed");
    while (1);
  }

  radio.setBand(FIX_BAND);
  radio.setMono(false);
  radio.setMute(false);

  radio.setFrequency(START_FREQUENCY); // Set initial frequency to 88.1 MHz
  print_lcd_message(lcd, "FM Initialized");
  radio.setVolume(START_VOLUME_FM);
}

/**
 * Initialize the MP3 player and set the initial volume.
 * Turns on the transistor to power the MP3 player and turns off the FM radio.
 * If initialization fails, it displays an error message on the LCD and halts the program (Power Cycle Required).
 */
void initMP3(DFRobotDFPlayerMini& mp3, LiquidCrystal& lcd) {
  digitalWrite(MP3_TRANSISTOR_PIN, LOW); // Turn on the transistor to power the MP3 player
  digitalWrite(FM_TRANSISTOR_PIN, HIGH); // Turn off the FM radio
  print_lcd_message(lcd, "Initializing MP3...");

  if (!mp3.begin(Serial, true, true)) {
    print_lcd_message(lcd, "MP3 Init Failed");
    while(1);
  }

  print_lcd_message(lcd, "MP3 Initialized");
  delay(1000);
  mp3.volume(START_VOLUME_MP3);
}

/**
 * Initialize the button pins as input.
 */
void initButtons() {
    pinMode(BUTTON_OK, INPUT);
    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_DOWN, INPUT);
    pinMode(BUTTON_RIGHT, INPUT);
    pinMode(BUTTON_LEFT, INPUT);
}

/**
 * Check if a button is pressed and held for a certain duration.
 * Returns 1 if the button is pressed normally, and 2 if held for over 3 seconds, otherwise returns 0.
 */
int buttonPressDB(int buttonPin) {
    int holdTime = 200;
    if (digitalRead(buttonPin) == HIGH) {
        delay(200); // Debounce delay
        if (digitalRead(buttonPin) == HIGH) {
          // Button is still pressed after debounce, check for hold duration
          while (digitalRead(buttonPin) == HIGH) {
              holdTime++;
              delay(1);
          }
          // Check if the button is held for more than 3 seconds
          if (holdTime >= 3000) {
              return 2;
          }
          return 1;
        }
    }
    return 0;
}

/**
 * Pauses or plays the specified audio source (FM radio or MP3 player) based on the current active state.
 * If the source is currently active, it will be paused; if it is inactive, it will be played.
 * The function also updates the active state accordingly.
 */
void pauseOrPlayDevice(AudioSource source, bool isActive, RDA5807M& radio, DFRobotDFPlayerMini& mp3) {
  if (isActive) {
    if (source == MP3_PLAYER) {
      mp3.start();
    } else {
      radio.setMute(false);
    }
  } else {
    if (source == MP3_PLAYER) {
      mp3.pause();
    } else {
      radio.setMute(true);
    }
  }
}

/**
 * Switches the audio source between FM radio and MP3 player.
 * When switching to the MP3 player, it mutes the FM radio and initializes the MP3 player.
 * When switching to the FM radio, it stops the MP3 player and initializes the FM radio.
 */
void switchAudioSource(AudioSource newSource, RDA5807M& radio, DFRobotDFPlayerMini& mp3, LiquidCrystal& lcd) {
  if (newSource == MP3_PLAYER) {
    radio.setMute(true);
    initMP3(mp3, lcd);
  } else {
    mp3.stop();
    initFM(radio, lcd);
  }
}

/**
 * Increases the volume of the specified audio source (FM radio or MP3 player) by one level, up to the maximum allowed volume.
 * For the MP3 player, the maximum volume is 30; for the FM radio, the maximum volume is 15.
 * The function updates the volume on the respective device and returns the new volume level.
 */
unsigned short increaseVolume(AudioSource source, unsigned short currentVolume, RDA5807M& radio, DFRobotDFPlayerMini& mp3) {
  if (source == MP3_PLAYER) {
    currentVolume = min(currentVolume + 1, 30);
    mp3.volume(currentVolume);
  } else {
    currentVolume = min(currentVolume + 1, 15);
    radio.setVolume(currentVolume);
  }
  return currentVolume;
}

/**
 * Decreases the volume of the specified audio source (FM radio or MP3 player) by one level, down to the minimum allowed volume (0).
 * For the MP3 player, the maximum volume is 30; for the FM radio, the maximum volume is 15.
 * The function updates the volume on the respective device and returns the new volume level.
 */
unsigned short decreaseVolume(AudioSource source, unsigned short currentVolume, RDA5807M& radio, DFRobotDFPlayerMini& mp3) {
  if (source == MP3_PLAYER) {
    currentVolume = max(currentVolume - 1, 0);
    mp3.volume(currentVolume);
  } else {
    currentVolume = max(currentVolume - 1, 0);
    radio.setVolume(currentVolume);
  }
  return currentVolume;
}

/**
 * Prints the current status of the MP3 player on the LCD.
 * Displays the currently playing file number and a visual representation of the volume level using a bar.
 * The volume bar is scaled to fit within 14 characters, with '+' representing the current volume 
 * level and '-' representing the remaining volume levels up to the maximum of 30.
 */
void printMP3Status(LiquidCrystal& lcd, DFRobotDFPlayerMini& mp3, unsigned short volumeMP3) {
  int currentFile = mp3.readCurrentFileNumber();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MP3 | ");
  lcd.print(currentFile);
  lcd.setCursor(0, 1);
  char volStr[3];
  sprintf(volStr, "%02d", volumeMP3);
  lcd.print(volStr);

  double volumeBarLength = (volumeMP3 / 30.0) * 14; // Scale volume to fit within 14 characters
  for (int i = 0; i < 14; i++) {
    if (i < volumeBarLength) {
      lcd.print("+");
    } else {
      lcd.print("-");
    }
  }
}

/**
 * Prints the current status of the FM radio on the LCD.
 * Displays the currently tuned frequency and a visual representation of the volume level using a bar.
 * The volume bar is scaled to fit within 14 characters, with '+' representing the current volume
 * level and '-' representing the remaining volume levels up to the maximum of 15.
 */
void printFMStatus(LiquidCrystal& lcd, RDA5807M& radio, unsigned short volumeFM) {
  int currentFrequency = radio.getFrequency();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FM | ");
  lcd.print(currentFrequency);
  lcd.setCursor(0, 1);
  char volStr[3];
  sprintf(volStr, "%02d", volumeFM);
  lcd.print(volStr);

  double volumeBarLength = (volumeFM / 15.0) * 14; // Scale volume to fit within 14 characters
  for (int i = 0; i < 14; i++) {
    if (i < volumeBarLength) {
      lcd.print("+");
    } else {
      lcd.print("-");
    }
  }
}

