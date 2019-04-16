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
boolean blink = false;

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
  OCR1A = 7812;                // set Output Compare Register value
  TCCR1B = TCCR1B | 00000101;   // set prescaler to 1024
}

void loop() {
  for (int i = 0; i < 8; i++) {
    buttonState[i] = digitalRead(buttonPins[i]);
    if (buttonState[i] == HIGH) {
      song += notes[i];
    }
  }
  if (song.length() > 0) {
    digitalWrite(ledPin, HIGH);
    if (blink) {
      digitalWrite(ledPin, LOW);
      delay(200);
      digitalWrite(ledPin, HIGH);
      blink = false;
    }
  }
  delay(175);

}

ISR(TIMER1_COMPA_vect) {
  if (clearButtonPressed == LOW && digitalRead(clearButton) == HIGH) {
    clearButtonPressed = true;
    song = "";
    digitalWrite(ledPin, LOW);
    Serial.println("Song cleared!");
  } else if (clearButtonPressed == HIGH && digitalRead(clearButton) == LOW) {
    clearButtonPressed = false;
  }

  if (playButtonPressed == LOW && digitalRead(playButton) == HIGH) {
    playButtonPressed = true;
    Serial.println(song);
    blink = true;
  } else if (playButtonPressed == HIGH && digitalRead(playButton) == LOW) {
    playButtonPressed = false;
  }
}
