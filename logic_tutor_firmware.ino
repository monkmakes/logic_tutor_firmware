/*
ATTiny1614 1MHz
*/

#include "morse.h"

const int IN_A_LED = 7;
const int IN_B_LED = 6;
const int IN_C_LED = 5;

const int IN_A_SW = 0;
const int IN_B_SW = 1;
const int IN_C_SW = 2;
const int BATT_MON = 10; // to do implement voltage monitoring

const float ANALOG_REF_V = 2.5;
const float V_DIODE = 0.3;
const float MIN_BATT_V = 1.1;

const byte UNUSED_PINS[] = {9, 3, 4, 8, 9};

const int SEQUENCE_DELAY = 1000; // mS -- MUST MATCH SAME CONST IN AUTOMATED TESTER

boolean aState = false;
boolean bState = false;
boolean cState = false;

void setup() {
  analogReference(INTERNAL2V5);
  pinMode(IN_A_LED, OUTPUT);
  pinMode(IN_B_LED, OUTPUT);
  pinMode(IN_C_LED, OUTPUT);
  pinMode(IN_A_SW, INPUT_PULLUP);
  pinMode(IN_B_SW, INPUT_PULLUP);
  pinMode(IN_C_SW, INPUT_PULLUP);
  for (int i = 0; i < sizeof(UNUSED_PINS); i++) {
    pinMode(UNUSED_PINS[i], OUTPUT); // prevent floating inputs
  }
  if (readBatteryVoltage() < MIN_BATT_V) getAttention(); // warn of low battery, but then continue anyway
  checkForStartupOptions();
}

void loop() {
  checkInput(IN_A_SW, IN_A_LED, &aState);
  checkInput(IN_B_SW, IN_B_LED, &bState);
  checkInput(IN_C_SW, IN_C_LED, &cState);
}

void checkInput(int swPin, int ledPin, boolean *state) {
  if (digitalRead(swPin) == LOW) {
    while (digitalRead(swPin) == LOW) {}; // wait for release
    delay(100); // debounce
    *state = ! *state;
    digitalWrite(ledPin, *state);
  }
}

void checkForStartupOptions() {
  if ((digitalRead(IN_A_SW) == LOW) && (digitalRead(IN_B_SW) == LOW) && (digitalRead(IN_C_SW) == LOW)) {
    morseCredits();
  }
  if ((digitalRead(IN_A_SW) == LOW) && (digitalRead(IN_B_SW) == HIGH) && (digitalRead(IN_C_SW) == HIGH)) {
    testSequence();
  }
  if ((digitalRead(IN_A_SW) == HIGH) && (digitalRead(IN_B_SW) == LOW) && (digitalRead(IN_C_SW) == HIGH)) {
    batteryTest();
  }
}

void morseCredits() {
  getAttention();
  flashString("Simon Monk and David Whale 2026", 200, IN_A_LED);
}

void getAttention() {
  int numBlinks = 20;
  for (int count = 0; count < numBlinks; count ++) {
    setButtons(0b111);
    delay(50);
    setButtons(0b000);
    delay(50);
  }
}

void testSequence() {
  // exercise all ABC button combinations for automated tester
  while (true) {
    for (int i = 0; i < 8; i++) {
      setButtons(i);
      delay(SEQUENCE_DELAY);
    }
    delay(SEQUENCE_DELAY * 2);
  }
}

void batteryTest() {
  while (true) {
    float v = readBatteryVoltage();
    setButtons(0); // all LEDs off
    if (v > MIN_BATT_V) digitalWrite(IN_C_LED, true);
    if (v > 1.25) digitalWrite(IN_B_LED, true);
    if (v > 1.5) digitalWrite(IN_A_LED, true);
  }
}

float readBatteryVoltage() {
  int reading = analogRead(BATT_MON);
  float v = (reading / 1023.0) * ANALOG_REF_V + V_DIODE;
  return v;
}

void setButtons(int abc) {
  digitalWrite(IN_A_LED, bitRead(abc, 0));
  digitalWrite(IN_B_LED, bitRead(abc, 1));
  digitalWrite(IN_C_LED, bitRead(abc, 2));
}
