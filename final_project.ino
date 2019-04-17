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
volatile boolean notesPressed[8] = {false, false, false, false, false, false, false, false};
volatile boolean playButtonPressed = false;
volatile boolean clearButtonPressed = false;
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
  if (clearButtonPressed) {
    song = "";
    digitalWrite(ledPin, LOW);
    Serial.println("Song cleared!");
  } else if (playButtonPressed) {
    Serial.println(song);
  }
  for (int i = 0; i < 8; i++) {
    if (notesPressed[i]) {
      song += notes[i];
    }
  }
  if (song.length() > 0) {
    digitalWrite(ledPin, 150);
    if (blink) {
      digitalWrite(ledPin, LOW);
      delay(200);
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
      delay(200);
      digitalWrite(ledPin, HIGH);
      blink = false;
    }
  }
  delay(175);

}

ISR(TIMER1_COMPA_vect) {
  if (clearButtonPressed == LOW && analogRead(clearButton) != 0) {
    clearButtonPressed = true;
  } else if (clearButtonPressed == HIGH && analogRead(clearButton) == 0) {
    clearButtonPressed = false;
  }

  if (playButtonPressed == LOW && analogRead(playButton) != 0) {
    playButtonPressed = true;
    blink = true;
  } else if (playButtonPressed == HIGH && analogRead(playButton) == 0) {
    playButtonPressed = false;
  }

  for (int i = 0; i < 8; i++) {
    if (notesPressed[i] == LOW && digitalRead(buttonPins[i]) == HIGH) {
      notesPressed[i] = true;
    } else if (notesPressed[i] == HIGH && digitalRead(buttonPins[i]) == LOW) {
      notesPressed[i] = false;
    }
  }
}
