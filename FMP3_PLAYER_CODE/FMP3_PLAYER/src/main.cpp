#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal.h>
#include <RDA5807M.h>
#include "helpers.h"

#define START_VOLUME_FM 15
#define START_VOLUME_MP3 10

#define BUTTON_OK 13
#define BUTTON_UP 12
#define BUTTON_DOWN 11
#define BUTTON_RIGHT 10
#define BUTTON_LEFT 9

#define BUTTON_NORMAL 1
#define BUTTON_HELD 2

RDA5807M radio;
DFRobotDFPlayerMini mp3;
LiquidCrystal lcd(3, 2, 4, 5, 6, 7);

bool isActive = false;
bool isMP3PlayerShuffle = false;

unsigned short volumeMP3 = START_VOLUME_MP3; // Min: 0, Max: 30
unsigned short volumeFM = START_VOLUME_FM;  // Min: 0, Max: 15

enum AudioSource {
  FM_RADIO = 0,
  MP3_PLAYER = 1
};

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
  if (buttonOkState == BUTTON_NORMAL) {
    isActive = !isActive;
    if (isActive) {
      if (currentAudioSource == MP3_PLAYER) {
        mp3.start();
      } else {
        radio.setMute(false);
      }
    } else {
      if (currentAudioSource == MP3_PLAYER) {
        mp3.pause();
      } else {
        radio.setMute(true);
      }
    }
  } else if (buttonOkState == BUTTON_HELD) {
    currentAudioSource = (currentAudioSource == MP3_PLAYER) ? FM_RADIO : MP3_PLAYER;
    if (isActive) {
      if (currentAudioSource == MP3_PLAYER) {
        radio.setMute(true);
        initMP3(mp3, lcd);
      } else {
        mp3.stop();
        initFM(radio, lcd);
      }
    }
  } 
  
  else if (buttonPressDB(BUTTON_UP) == BUTTON_NORMAL) {
    if (currentAudioSource == MP3_PLAYER) {
      volumeMP3 = min(volumeMP3 + 1, 30);
      mp3.volume(volumeMP3);
    } else {
      volumeFM = min(volumeFM + 1, 15);
      radio.setVolume(volumeFM);
    }
  }

  else if (buttonPressDB(BUTTON_DOWN) == BUTTON_NORMAL) {
    if (currentAudioSource == MP3_PLAYER) {
      volumeMP3 = max(volumeMP3 - 1, 0);
      mp3.volume(volumeMP3);
    } else {
      volumeFM = max(volumeFM - 1, 0);
      radio.setVolume(volumeFM);
    }
  }

  else if (buttonPressDB(BUTTON_RIGHT) == BUTTON_NORMAL) {
    if (currentAudioSource == MP3_PLAYER) {
      mp3.next();
    } else {
      radio.seekUp();
    }
  }

  else if (buttonPressDB(BUTTON_LEFT) == BUTTON_NORMAL) {
    if (currentAudioSource == MP3_PLAYER) {
      mp3.previous();
    } else {
      radio.seekDown();
    }
  }

  if (isActive) {
    if (currentAudioSource == MP3_PLAYER) {
      print_lcd_message(lcd, "Playing MP3");
    } else {
      print_lcd_message(lcd, "Playing FM");
    }
  } else {
    print_lcd_message(lcd, "Paused");
  }
  
  delay(10);
}