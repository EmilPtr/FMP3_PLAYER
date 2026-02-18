#include <Arduino.h>
#include <RDA5807M.h>


#define FIX_BAND RADIO_BAND_FM  ///< The band that will be tuned by this sketch is FM.
#define FIX_STATION 1      ///< The station that will be tuned by this sketch is 106.80 MHz.
#define FIX_VOLUME 10           ///< The volume that will be set by this sketch is level 4.

RDA5807M radio;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting up...");

  radio.debugEnable(true);  // enable debug output to Serial port
  radio._wireDebug(false);  // enable debug output of I2C communication to Serial port
  
  if (!radio.initWire(Wire)) {
    Serial.println("Failed to initialize I2C!");
    while (1);
  }

  radio.setBand(FIX_BAND);
  radio.setFrequency(FIX_STATION);
  radio.setVolume(FIX_VOLUME);
  radio.setMono(false);
  radio.setMute(false);

  Serial.println("Setup complete.");
}

void loop() {
  // put your main code here, to run repeatedly:
  char s[12];
  radio.formatFrequency(s, sizeof(s));
  Serial.print("Station:");
  Serial.println(s);

  radio.seekUp();

  Serial.print("Radio:");
  radio.debugRadioInfo();

  Serial.print("Audio:");
  radio.debugAudioInfo();

  delay(3000);
}
