#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal.h>
#include <RDA5807M.h>
#include <SoftwareSerial.h>


#define FIX_BAND RADIO_BAND_FM  ///< The band that will be tuned by this sketch is FM.
#define MP3_TRANSISTOR_PIN A0
#define FM_TRANSISTOR_PIN A1
#define BUTTON_OK 13
#define BUTTON_UP 12

SoftwareSerial softSerial(/*rx =*/A3, /*tx =*/A2);

void(* reset) (void) = 0; // declare reset function at address 0


RDA5807M radio;
DFRobotDFPlayerMini mp3;
LiquidCrystal lcd(3, 2, 4, 5, 6, 7);


unsigned short volumeMP3 = 10; // Min: 0, Max: 30
unsigned short volumeFM = 5;  // Min: 0, Max: 15


char* FM_frequency_str;


enum AudioSource {
  FM_RADIO = 0,
  MP3_PLAYER = 1
};


AudioSource currentAudioSource = MP3_PLAYER;


void initFM() {
  digitalWrite(FM_TRANSISTOR_PIN, HIGH); // Turn on the transistor to power the FM radio

  Serial.println("Initializing FM radio...");

  if (!radio.initWire(Wire)) {
    Serial.println("Couldn't find RDA5807M");
    reset();  
  }

  radio.setBand(FIX_BAND);
  radio.setMono(false);
  radio.setMute(false);

  radio.setVolume(volumeFM);
  Serial.println("FM radio initialized.");
}

void initMP3() {
  digitalWrite(MP3_TRANSISTOR_PIN, HIGH); // Turn on the transistor to power the MP3 player

  //Serial.println("Initializing MP3 player...");
  if (!mp3.begin(Serial, true, true)) {
    //Serial.println("Unable to begin MP3 player:");
    lcd.setCursor(0, 0);
    lcd.print("Unable to begin MP3 player:");
    while(1);
  }
  delay(1000);
  mp3.volume(volumeMP3);
  //Serial.println("MP3 player initialized.");
}

void printDetail(uint8_t type, int value){
  lcd.clear();
  switch (type) {
    case TimeOut:
      lcd.print("Time Out!");
      break;
    case WrongStack:
      lcd.print("Stack Wrong!");
      break;
    case DFPlayerCardInserted:
      lcd.print("Card Inserted!");
      break;
    case DFPlayerCardRemoved:
      lcd.print("Card Removed!");
      break;
    case DFPlayerCardOnline:
      lcd.print("Card Online!");
      break;
    case DFPlayerUSBInserted:
      lcd.print("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      lcd.print("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      lcd.setCursor(0, 0);
      lcd.print("Track:");
      lcd.print(value);
      lcd.setCursor(0, 1);
      lcd.print("Play Finished!");
      break;
    case DFPlayerError:
      lcd.setCursor(0, 0);
      lcd.print("DFPlayerError:");
      lcd.setCursor(0, 1);
      switch (value) {
        case Busy:
          lcd.print("Card not found");
          break;
        case Sleeping:
          lcd.print("Sleeping");
          break;
        case SerialWrongStack:
          lcd.print("Wrong Stack");
          break;
        case CheckSumNotMatch:
          lcd.print("Checksum Error");
          break;
        case FileIndexOut:
          lcd.print("Index Out Bound");
          break;
        case FileMismatch:
          lcd.print("File Not Found");
          break;
        case Advertise:
          lcd.print("In Advertise");
          break;
        default:
          lcd.print("Unknown Error");
          break;
      }
      break;
    default:
      lcd.print("Unknown Event");
      break;
  }
}

void setup() {
  //Serial.begin(4800);
  pinMode(BUTTON_OK, INPUT);
  pinMode(BUTTON_UP, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(1000);
  initMP3();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done.");
}

void loop() {
  if (mp3.available()) {
    printDetail(mp3.readType(), mp3.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  if (digitalRead(BUTTON_OK) == HIGH) {
    mp3.play(1); // Play the first track on the SD card when the button is pressed
  }
  if (digitalRead(BUTTON_UP) == HIGH) {
    mp3.next();
  }
}