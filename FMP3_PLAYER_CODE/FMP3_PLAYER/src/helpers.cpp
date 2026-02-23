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

void print_lcd_message(LiquidCrystal& lcd, const char* message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
}

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

  radio.setFrequency(START_FREQUENCY); // Set initial frequency to 87.5 MHz

  radio.setVolume(START_VOLUME_FM);
}

void initMP3(DFRobotDFPlayerMini& mp3, LiquidCrystal& lcd) {
  digitalWrite(MP3_TRANSISTOR_PIN, LOW); // Turn on the transistor to power the MP3 player
  digitalWrite(FM_TRANSISTOR_PIN, HIGH); // Turn off the FM radio
  print_lcd_message(lcd, "Initializing MP3...");

  if (!mp3.begin(Serial, true, true)) {
    print_lcd_message(lcd, "MP3 Init Failed");
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

/**
 * Check if a button is pressed and held for a certain duration.
 * Returns 1 if the button is pressed normally, and 2 if held for over 3 seconds, otherwise returns 0.
 */
int buttonPressDB(int buttonPin) {
    int holdTime = 200;
    if (digitalRead(buttonPin) == HIGH) {

        delay(200); // Debounce delay

        if (digitalRead(buttonPin) == HIGH) {

            while (digitalRead(buttonPin) == HIGH) {
                holdTime++;
                delay(1);
            }

            if (holdTime >= 3000) {
                return 2;
            }

            return 1;
        }
    }
    return 0;
}