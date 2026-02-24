#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal.h>
#include <RDA5807M.h>
#include "helpers.h"

RDA5807M radio;
DFRobotDFPlayerMini mp3;
LiquidCrystal lcd(3, 2, 4, 5, 6, 7);

bool isActive = false;
bool isMP3PlayerShuffle = false;

unsigned short volumeMP3 = START_VOLUME_MP3; // Min: 0, Max: 30
unsigned short volumeFM = START_VOLUME_FM;  // Min: 0, Max: 15

AudioSource currentAudioSource = MP3_PLAYER;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  print_lcd_message(lcd, "Starting...");
  delay(1000);
  initMP3(mp3, lcd);
  print_lcd_message(lcd, "Done.");
}

void loop() {
  int buttonOkState = buttonPressDB(BUTTON_OK);
  int buttonUpState = buttonPressDB(BUTTON_UP);
  int buttonDownState = buttonPressDB(BUTTON_DOWN);
  int buttonRightState = buttonPressDB(BUTTON_RIGHT);
  int buttonLeftState = buttonPressDB(BUTTON_LEFT);

  if (buttonOkState == BUTTON_NORMAL) {
    isActive = !isActive;
    pauseOrPlayDevice(currentAudioSource, isActive, radio, mp3);
  } else if (buttonOkState == BUTTON_HELD) {
    currentAudioSource = (currentAudioSource == MP3_PLAYER) ? FM_RADIO : MP3_PLAYER;
    isActive = true; // Automatically play the new source when switching
    switchAudioSource(currentAudioSource, radio, mp3, lcd);
  } 
  
  else if (buttonUpState == BUTTON_NORMAL) {
    increaseVolume(currentAudioSource, volumeMP3, radio, mp3);
  }

  else if (buttonDownState == BUTTON_NORMAL) {
    decreaseVolume(currentAudioSource, volumeMP3, radio, mp3);
  }

  else if (buttonRightState == BUTTON_NORMAL) {
    if (currentAudioSource == MP3_PLAYER) {
      mp3.next();
    } else {
      radio.seekUp();
    }
  }

  else if (buttonLeftState == BUTTON_NORMAL) {
    if (currentAudioSource == MP3_PLAYER) {
      mp3.previous();
    } else {
      radio.seekDown();
    }
  }

  if (isActive) {
    if (currentAudioSource == MP3_PLAYER) {
      printMP3Status(lcd, mp3, volumeMP3);
    } else {
      printFMStatus(lcd, radio, volumeFM);
    }
  } else {
    print_lcd_message(lcd, "Paused");
  }
  
  delay(10);
}