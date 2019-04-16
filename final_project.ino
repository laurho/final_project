//////////////////////////////
// Signal Processing project//
//////////////////////////////

// Init pins
const int ledPin = 13;
const int buttonPins[8] = {2, 3, 4, 5, 6, 7, 8, 12};
const int playButton = -1;
const int clearButton = -1;

volatile byte state = LOW;

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
  bitSet(TCCR2B, WGM22);        // set WGM22[2] = 1
  bitSet(TIMSK2, OCIE2A);       // enable interrupt on OCR2A register
  OCR2A = 255;                  // set Output Compare Register value
  TCCR2B |= (1 << CS22) | (0 << CS21) | (1 << CS20) ; // set the prescaler to 1024
}

void loop() {
  Serial.println(state);
  digitalWrite(ledPin, state);
  delay(100);
}

ISR(TIMER2_COMPA_vect) {
  state = !state;
}
