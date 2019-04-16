//////////////////////////////
// Signal Processing project//
//////////////////////////////

// Init pins
const int ledPin = 13;
const int buttonPins[8] = {2, 3, 4, 5, 6, 7, 8, 12};
const int playButton = A1;
const int clearButton = A2;

const int elevator = 11;
const int gate = 9;
const int selector = 10;

volatile byte state = LOW;
int buttonState[8] = {0, 0, 0, 0, 0, 0, 0, 0};
char notes[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'c'};
String song = "";
boolean playButtonPressed = false;
boolean clearButtonPressed = false;

void setup() {
  // set the pin modes
  pinMode(ledPin, OUTPUT);
  for (int i = 0; i < 8; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(playButton, INPUT);
  pinMode(clearButton, INPUT);

  // enable interrupts
  interrupts();
  Serial.begin(19200);
  bitSet(TCCR1B, WGM12);        // set WGM1[2] = 1
  bitSet(TIMSK1, OCIE1A);      // enable interrupt on OCR1A register
  OCR1A = 60000;                // set Output Compare Register value
  TCCR1B = TCCR1B | 00000101;   // set prescaler to 1024
}

void loop() {
  if (clearButtonPressed) {
    song = "";
    Serial.println("Song cleared!");
    clearButtonPressed = false;
  }

  else if (playButtonPressed) {
    Serial.println(song);
    playButtonPressed = false;
  }
  else {
    for (int i = 0; i < 8; i++) {
      buttonState[i] = digitalRead(buttonPins[i]);
      if (buttonState[i] == HIGH) {
        song += notes[i];
      }
    }
  }
}

ISR(TIMER1_COMPA_vect) {
  clearButtonPressed = digitalRead(clearButton);
  playButtonPressed = digitalRead(playButton);
}
