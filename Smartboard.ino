//// #include

// Date
#include <EEPROMex.h>
#include <EEPROMVar.h>
#include <TimeLib.h>

// LCD
#include <LiquidCrystal.h>

// LEDS
#include <FastLED.h>

//// #define

// Smartboard
#define DATE_ADDRESS 15
#define MAX_TIME_PASSED 10000
#define LONG_PRESS_TIME 2000

// Date
#define ADD_PIN A1
#define SUB_PIN A2
#define OK_PIN A3
#define DATE_PRECISE 5
#define DATE_SHORT 3
#define YEAR_INDEX 0
#define MONTH_INDEX 1
#define DAY_INDEX 2
#define HOUR_INDEX 3
#define MINUTE_INDEX 4
#define LEAP_YEAR(Y) ( ((1970+(Y)) > 0) && !((1970 + (Y)) % 4) && ( ((1970 + (Y)) % 100) || !((1970 + (Y)) % 400) ) )

// LCD
#define OnOff_PIN A7
#define RS_PIN 2
#define E_PIN 3
#define D4_PIN 4
#define D5_PIN 5
#define D6_PIN 6
#define D7_PIN 7

// LEDs
#define LEDS_PIN 8
#define NUM_LEDS 28
#define RED_THRESHOLD 86400
#define ORANGE_THRESHOLD 259200
#define YELLOW_THRESHOLD 432000
#define RED 0
#define ORANGE 24
#define YELLOW 42
#define GREEN 85
#define BLUE 150

// Multiplexer
#define SIG_PIN A4
#define ADDRESS_LENGTH 4
#define NUM_BUTTONS 14

// Photodetector
#define PD_PIN A6
#define LED_PIN A5
#define NUM_SAMPLES 4
#define THRESHOLD 0.5


//// Global constants and variables

// Smartboard
unsigned long last_input_time;
unsigned long press_begin;
bool active_entries[NUM_BUTTONS];

// Date
static  const uint8_t month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const String date_parts[5] = {"Year", "Month", "Day", "Hours", "Minutes"};

// LCD
LiquidCrystal lcd(RS_PIN, E_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);
bool on = false;

// LEDs
CRGB leds[NUM_LEDS];

// Multiplexer
const byte ADDRESS_PINS[ADDRESS_LENGTH] = {12, 11, 10, 9};


void setup() {

  // Date
  pinMode(ADD_PIN, INPUT_PULLUP);
  pinMode(SUB_PIN, INPUT_PULLUP);
  pinMode(OK_PIN, INPUT_PULLUP);

  // LCD
  pinMode(OnOff_PIN, OUTPUT);

  // LEDs
  FastLED.addLeds<NEOPIXEL, LEDS_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(50, 255, 50);
  }
  FastLED.show();
  delay(1000);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(0, 255, 0);
  }
  FastLED.show();

  // Multiplexer
  pinMode(SIG_PIN, INPUT_PULLUP);
  for(int i = 0; i < ADDRESS_LENGTH; i++)
  {
    pinMode(ADDRESS_PINS[i], OUTPUT);
  }

  // Photodetector
  pinMode(PD_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);

  switch_on();
  delay(1000);
  last_input_time = millis();
  ask_for_time(DATE_ADDRESS);
}

void loop() {
  if (on)
  {
    if (millis() > last_input_time + MAX_TIME_PASSED)
    {
      switch_off();

      // Eventually switch off the leds, if you don't want them always on:
      // lights_off();
    }

    lights_on();

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
      if (read_button(i))
      {
        press_begin = millis();
        select(i);
        while(read_button(i));
        Serial.println("STOP!");
        if ( (millis() - press_begin) > LONG_PRESS_TIME)
        {
          lights_off();
          delete_entry(i);
        }
        else
        {
          ask_for_time(i);
        }
        delay(20);
      }  
    }
  }
  else
  {
    if (movement())
    {
      switch_on();
      last_input_time = millis();
    }
  }
}
