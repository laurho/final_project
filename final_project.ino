///////////////////
// Final Project //
///////////////////
#include <Servo.h>
Servo myservo;  // create servo object to control a servo

// Init pins
const int ledPin = 13;
const int buttonPins[8] = {2, 3, 4, 5, 6, 7, 8, 12};
const int playButton = A1;
const int clearButton = A2;

const int elevatorPin = 11;
const int gatePin = 9;
const int selectorPin = 10;

volatile byte state = LOW;
int buttonState[8] = {0, 0, 0, 0, 0, 0, 0, 0};
char notes[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'c'};
const int C = 40; const int D = 55; const int E = 75; const int F = 85;
const int G = 95; const int A = 105; const int B = 120; const int c = 135;
String song = "";
volatile boolean notesPressed[8] = {false, false, false, false, false, false, false, false};
volatile boolean playButtonPressed = false;
volatile boolean clearButtonPressed = false;
boolean blink = false;

void setup() {
  myservo.attach(selectorPin);
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
  bitSet(TCCR2B, WGM22);        // set WGM1[2] = 1
  bitSet(TIMSK2, OCIE2A);      // enable interrupt on OCR1A register
  OCR2A = 56;                // set Output Compare Register value
  TCCR2B = TCCR2B | 00000101;   // set prescaler to 1024
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

void playSong(String song) {
  //For each note
  //Move to the correct angle
  //Open the gate
  //Close the gate
  for (int i = 0; i < song.length(); i++) {
    if (song[i] == 'C') {
      myservo.write(C);
    } else if (song[i] == 'D') {
      myservo.write(D);
    } else if (song[i] == 'E') {
      myservo.write(E);
    } else if (song[i] == 'F') {
      myservo.write(F);
    } else if (song[i] == 'G') {
      myservo.write(G);
    } else if (song[i] == 'A') {
      myservo.write(A);
    } else if (song[i] == 'B') {
      myservo.write(B);
    } else {
      myservo.write(c); 
    }
  }

}

ISR(TIMER2_COMPA_vect) {
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
