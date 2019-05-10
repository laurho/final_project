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
const int C = 20; const int D = 35; const int E = 48; const int F = 60;
const int G = 65; const int A = 81; const int B = 88; const int c = 106;
String song = "";
volatile boolean notesPressed[8] = {false, false, false, false, false, false, false, false};
volatile boolean playButtonPressed = false;
volatile boolean clearButtonPressed = false;

// Misc.
const int GATE_DELAY = 60;
boolean blink_led = false;
unsigned long curr_time = 0;

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
}

void loop() {
  curr_time = millis();

  if (analogRead(clearButton) == 1023) {
    song = "";
    digitalWrite(ledPin, LOW);
    Serial.println("Song cleared!");
    clearButtonPressed = false;
  } else if (analogRead(playButton) == 1023) {
    blink_led = true;
    Serial.println(song);
    playSong(song);
  }
  
  for (int i = 0; i < 8; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      song += notes[i];
    }
  }

  if (song.length() > 0) {
    digitalWrite(ledPin, 150);
    if (blink_led) {
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
  delay(150);
}

void releaseOneMarble() {
  gateServo.write(165);
  delay(750);
  gateServo.write(140);
  delay(60);
  gateServo.write(165);
}

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
    //releaseOneMarble();
    gateServo.write(140); //Open gate
    while (time_elapsed < GATE_DELAY) { //Wait a bit of time
      time_elapsed += 1;
    }
    gateServo.write(165); //Close gate
    delay(1000);
  }
}
