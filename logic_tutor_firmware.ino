/*
ATTiny1614 1MHz
*/

const int IN_A_LED = 7;
const int IN_B_LED = 6;
const int IN_C_LED = 5;

const int IN_A_SW = 0;
const int IN_B_SW = 1;
const int IN_C_SW = 2;

const byte UNUSED_PINS[] = {9, 10, 3, 4, 8, 9, 10, 11};

boolean aState = false;
boolean bState = false;
boolean cState = false;

void setup() {
  pinMode(IN_A_LED, OUTPUT);
  pinMode(IN_B_LED, OUTPUT);
  pinMode(IN_C_LED, OUTPUT);
  pinMode(IN_A_SW, INPUT_PULLUP);
  pinMode(IN_B_SW, INPUT_PULLUP);
  pinMode(IN_C_SW, INPUT_PULLUP);
  for (int i = 0; i < sizeof(UNUSED_PINS); i++) {
    pinMode(UNUSED_PINS[i], OUTPUT); // prevent floating inputs
  }
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
