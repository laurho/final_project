// Timer compare version
const int ledPin = 8;
volatile byte state = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
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
