///////////////////
// Final Project //
///////////////////
#include <Servo.h>
Servo selectorServo;
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
const int C = 20; const int D = 35; const int E = 48; const int F = 60;
const int G = 65; const int A = 81; const int B = 88; const int c = 106;
String song = "";
volatile boolean notesPressed[8] = {false, false, false, false, false, false, false, false};
volatile boolean playButtonPressed = false;
volatile boolean clearButtonPressed = false;

// Misc.
const int GATE_DELAY = 5000;
boolean blink_led = false;
const int closeGate = 165;
const int openGate = 140;

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
  bitSet(TCCR2B, WGM21);       // set WGM2[1] = 1
  bitSet(TIMSK2, OCIE2B);      // enable interrupt on OCR2B register
  OCR2B = 60;                  // set Output Compare Register value
  TCCR2B = TCCR2B | 00000111;  // set prescaler to 1024
}

void loop() {
  if (clearButtonPressed) { //Clear the song input and turn off led indicator
    song = "";
    digitalWrite(ledPin, LOW);
  } else if (playButtonPressed) { //Play song
    playSong(song);
  }
  for (int i = 0; i < 8; i++) { //Add the notes if they were pressed
    if (notesPressed[i]) {
      song += notes[i];
    }
  }
  if (song.length() > 0) { // If there is a song in queue, light led indicator
    digitalWrite(ledPin, 150);
    if (blink_led) { //blink_led if play is pressed
      digitalWrite(ledPin, LOW);
      delay(200);
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
      delay(200);
      digitalWrite(ledPin, HIGH);
      blink_led = false;
    }
  }
  analogWrite(elevatorPin, elevatorSpeed()); //Set the elevator to a particular speed

  delay(175);
}

// Dependign on the potentiometer, change the elevator speed
int elevatorSpeed() {
  int potVal = analogRead(potPin);
  if (potVal < 341) {
    return 150;
  } else if (potVal < 682) {
    return 200;
  } else {
    return 255;
  }
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
    gateServo.write(openGate);
    while (time_elapsed < elevatorSpeed()) { //Wait a bit of time depending on speed
      time_elapsed += 1;
    }
    gateServo.write(closeGate);
  }
}

// Timer interrupt to check for all button presses
ISR(TIMER2_COMPB_vect) {
  //Clear button
  if (clearButtonPressed == LOW && analogRead(clearButton) != 0) {
    clearButtonPressed = true;
  } else if (clearButtonPressed == HIGH && analogRead(clearButton) == 0) {
    clearButtonPressed = false;
  }

  //Play button
  if (playButtonPressed == LOW && analogRead(playButton) != 0) {
    playButtonPressed = true;
    blink_led = true;
  } else if (playButtonPressed == HIGH && analogRead(playButton) == 0) {
    playButtonPressed = false;
  }

  //Note buttons
  for (int i = 0; i < 8; i++) {
    if (notesPressed[i] == LOW && digitalRead(buttonPins[i]) == HIGH) {
      notesPressed[i] = true;
    } else if (notesPressed[i] == HIGH && digitalRead(buttonPins[i]) == LOW) {
      notesPressed[i] = false;
    }
  }
}
