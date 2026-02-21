#include "helpers.h"
#include <RDA5807M.h>
#include <LiquidCrystal.h>

#define FIX_BAND RADIO_BAND_FM
#define MP3_TRANSISTOR_PIN A0
#define FM_TRANSISTOR_PIN A1
#define START_FREQUENCY 8750
#define START_VOLUME_FM 15
#define START_VOLUME_MP3 10
#define BUTTON_OK 13
#define BUTTON_UP 12
#define BUTTON_DOWN 11
#define BUTTON_RIGHT 10
#define BUTTON_LEFT 9

void initFM(RDA5807M& radio, LiquidCrystal& lcd) {
  digitalWrite(FM_TRANSISTOR_PIN, HIGH); // Turn on the transistor to power the FM radio

  radio._wireDebug(true);
  radio.debugEnable(true);

  if (!radio.initWire(Wire)) {
    while (1);
  }

  radio.setBand(FIX_BAND);
  radio.setMono(false);
  radio.setMute(false);

  radio.setFrequency(START_FREQUENCY); // Set initial frequency to 87.5 MHz

  radio.setVolume(START_VOLUME_FM);
}

void initMP3(DFRobotDFPlayerMini& mp3, LiquidCrystal& lcd) {
  digitalWrite(MP3_TRANSISTOR_PIN, HIGH); // Turn on the transistor to power the MP3 player

  if (!mp3.begin(Serial, true, true)) {
    lcd.setCursor(0, 0);
    lcd.print("Unable to begin MP3 player:");
    while(1);
  }
  delay(1000);
  mp3.volume(START_VOLUME_MP3);
}

void initButtons() {
    pinMode(BUTTON_OK, INPUT);
    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_DOWN, INPUT);
    pinMode(BUTTON_RIGHT, INPUT);
    pinMode(BUTTON_LEFT, INPUT);
}

bool buttonPressDB(int buttonPin) {
    if (digitalRead(buttonPin) == HIGH) {
        delay(200); // Debounce delay
        if (digitalRead(buttonPin) == HIGH) {
            while (digitalRead(buttonPin) == HIGH); // Wait for button release
            return true;
        }
    }
    return false;
}

void print_lcd_message(LiquidCrystal& lcd, const char* message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
}