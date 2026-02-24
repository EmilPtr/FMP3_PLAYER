#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal.h>
#include <RDA5807M.h>

#define FIX_BAND RADIO_BAND_FM
#define MP3_TRANSISTOR_PIN A0
#define FM_TRANSISTOR_PIN A1
#define START_FREQUENCY 8810
#define START_VOLUME_FM 15
#define START_VOLUME_MP3 10
#define BUTTON_OK 13
#define BUTTON_UP 12
#define BUTTON_DOWN 11
#define BUTTON_RIGHT 10
#define BUTTON_LEFT 9
#define BUTTON_NORMAL 1
#define BUTTON_HELD 2

enum AudioSource {
  FM_RADIO = 0,
  MP3_PLAYER = 1
};

/**
 * Initialize the FM radio and set the initial frequency and volume.
 * Turns on the transistor to power the FM radio and turns off the MP3 player.
 * If initialization fails, it displays an error message on the LCD and halts the program (Power Cycle Required).
 */
void initFM(RDA5807M& radio, LiquidCrystal& lcd);

/**
 * Initialize the MP3 player and set the initial volume.
 * Turns on the transistor to power the MP3 player and turns off the FM radio.
 * If initialization fails, it displays an error message on the LCD and halts the program (Power Cycle Required).
 */
void initMP3(DFRobotDFPlayerMini& mp3, LiquidCrystal& lcd);

/**
 * Initialize the button pins as input.
 */
void initButtons();

/**
 * Check if a button is pressed and held for a certain duration.
 * Returns 1 if the button is pressed normally, and 2 if held for over 3 seconds, otherwise returns 0.
 */
int buttonPressDB(int buttonPin);

/**
 * Print a popup message on the LCD.
 */
void print_lcd_message(LiquidCrystal& lcd, const char* message);

/**
 * Pauses or plays the specified audio source (FM radio or MP3 player) based on the current active state.
 * If the source is currently active, it will be paused; if it is inactive, it will be played.
 * The function also updates the active state accordingly.
 */
void pauseOrPlayDevice(AudioSource source, bool isActive, RDA5807M& radio, DFRobotDFPlayerMini& mp3);

/**
 * Switches the audio source between FM radio and MP3 player.
 * When switching to the MP3 player, it mutes the FM radio and initializes the MP3 player.
 * When switching to the FM radio, it stops the MP3 player and initializes the FM radio.
 */
void switchAudioSource(AudioSource newSource, RDA5807M& radio, DFRobotDFPlayerMini& mp3, LiquidCrystal& lcd);

/**
 * Increases the volume of the specified audio source (FM radio or MP3 player) by one level, up to the maximum allowed volume.
 * For the MP3 player, the maximum volume is 30; for the FM radio, the maximum volume is 15.
 * The function updates the volume on the respective device and returns the new volume level.
 */
unsigned short increaseVolume(AudioSource source, unsigned short currentVolume, RDA5807M& radio, DFRobotDFPlayerMini& mp3);

/**
 * Decreases the volume of the specified audio source (FM radio or MP3 player) by one level, down to the minimum allowed volume (0).
 * For the MP3 player, the maximum volume is 30; for the FM radio, the maximum volume is 15.
 * The function updates the volume on the respective device and returns the new volume level.
 */
unsigned short decreaseVolume(AudioSource source, unsigned short currentVolume, RDA5807M& radio, DFRobotDFPlayerMini& mp3);

/**
 * Prints the current status of the MP3 player on the LCD.
 * Displays the currently playing file number and a visual representation of the volume level using a bar.
 * The volume bar is scaled to fit within 14 characters, with '+' representing the current volume 
 * level and '-' representing the remaining volume levels up to the maximum of 30.
 */
void printMP3Status(LiquidCrystal& lcd, DFRobotDFPlayerMini& mp3, unsigned short volumeMP3);

/**
 * Prints the current status of the FM radio on the LCD.
 * Displays the currently tuned frequency and a visual representation of the volume level using a bar.
 * The volume bar is scaled to fit within 14 characters, with '+' representing the current volume 
 * level and '-' representing the remaining volume levels up to the maximum of 15.
 */
void printFMStatus(LiquidCrystal& lcd, RDA5807M& radio, unsigned short volumeFM);

#endif // HELPERS_H