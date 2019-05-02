///////////////////
// Final Project //
///////////////////
#include <Servo.h>
Servo selectorServo;  // create servo object to control a servo
Servo gateServo;

// Init pins
const int ledPin = 13;
const int buttonPins[8] = {2, 3, 4, 5, 6, 7, 8, 12};
const int playButton = A1;
const int clearButton = A2;
const int potPin = A0;
const int elevatorPin = 11;
const int gatePin = 9;
const int selectorPin = 10;

// Button/Note values
volatile byte state = LOW;
int buttonState[8] = {0, 0, 0, 0, 0, 0, 0, 0};
char notes[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'c'};
const int C = 92; const int D = 100; const int E = 113; const int F = 129;
const int G = 142; const int A = 158; const int B = 167; const int c = 180;
String song = "";
volatile boolean notesPressed[8] = {false, false, false, false, false, false, false, false};
volatile boolean playButtonPressed = false;
volatile boolean clearButtonPressed = false;

// Misc.
const int GATE_DELAY = 5000;
boolean blink = false;

void setup() {
  // Set up the servos
  selectorServo.attach(selectorPin);
  gateServo.attach(gatePin);
  // Set up the DC motor
  pinMode(elevatorPin, OUTPUT);

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
    playSong(song);
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
//  analogWrite(elevatorPin, analogRead(potPin));
  analogWrite(elevatorPin, 250);
  
  delay(175);
}

void playSong(String song) {
  //For each note: move to the correct angle
  for (int i = 0; i < song.length(); i++) {
    int time_elapsed = 0;
    switch (song[i]) {
      case 'C':
        selectorServo.write(C);
        break;
      case 'D':
        selectorServo.write(D);
        break;
      case 'E':
        selectorServo.write(E);
        break;
      case 'F':
        selectorServo.write(F);
        break;
      case 'G':
        selectorServo.write(G);
        break;
      case 'A':
        selectorServo.write(A);
        break;
      case 'B':
        selectorServo.write(B);
        break;
      case 'c':
        selectorServo.write(c);
        break;
      default:
        break;
    }
    gateServo.write(90); //Open gate
    while (time_elapsed < GATE_DELAY) { //Wait a bit of time
      time_elapsed += 1;
    }
    gateServo.write(0); //Close gate
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
