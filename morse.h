char* letters[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."          // S-Z
};

char* numbers[] = {
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

void flashDotOrDash(char dotOrDash, int dotDelay, int pin) {
  digitalWrite(pin, HIGH);
  if (dotOrDash == '.') {
    delay(dotDelay);           
  }
  else { 
    // must be a dash 
    delay(dotDelay * 3);           
  }
  digitalWrite(pin, LOW);    
  delay(dotDelay); // gap between flashes
}

void flashSequence(char* sequence, int dotDelay, int pin) {
  int i = 0;
  while (sequence[i] != NULL) {
    flashDotOrDash(sequence[i], dotDelay, pin);
    i++;
  }
  delay(dotDelay * 3);    // gap between letters
}

void flashString(char *message, int dotDelay, int pin) {
  int i = 0;
  while (message[i] != 0) {
    char ch = message[i];
    i++;
    if (ch >= 'a' && ch <= 'z') {
      flashSequence(letters[ch - 'a'], dotDelay, pin);
    }
    else if (ch >= 'A' && ch <= 'Z') {
      flashSequence(letters[ch - 'A'], dotDelay, pin);
    }
    else if (ch >= '0' && ch <= '9') {
      flashSequence(numbers[ch - '0'], dotDelay, pin);
    }
    else if (ch == ' ') {
      delay(dotDelay * 4);  // gap between words  
    }
  }
}




