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

RDA5807M radio;
DFRobotDFPlayerMini mp3;
LiquidCrystal lcd(3, 2, 4, 5, 6, 7);

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
  print_lcd_message(lcd, "Initializing...");
  delay(1000);
  initFM(radio, lcd);
  print_lcd_message(lcd, "Done.");
}

void loop() {

  delay(10);
}