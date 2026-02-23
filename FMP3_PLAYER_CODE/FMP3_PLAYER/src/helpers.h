#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal.h>
#include <RDA5807M.h>

void initFM(RDA5807M& radio, LiquidCrystal& lcd);
void initMP3(DFRobotDFPlayerMini& mp3, LiquidCrystal& lcd);
void initButtons();
int buttonPressDB(int buttonPin);
void print_lcd_message(LiquidCrystal& lcd, const char* message);

#endif // HELPERS_H