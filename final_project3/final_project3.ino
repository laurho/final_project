///////////////////
// Final Project //
///////////////////
#include <Servo.h>
Servo selectorServo;  // create servo object to control a servo
Servo gateServo;

// Init pins
const int ledPin = 13;
const int buttonPins[8] = {11, 3, 4, 5, 6, 7, 8, 12};
const int playButton = 2;
const int clearButton = A2;
const int potPin = A0;
//const int elevatorPin = 11;
const int gatePin = 9;
const int selectorPin = 10;

// Button/Note values
volatile byte state = LOW;
int buttonState[8] = {0, 0, 0, 0, 0, 0, 0, 0};
char notes[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'c'};
const int C = 23; const int D = 37; const int E = 48; const int F = 60;
const int G = 76; const int A = 87; const int B = 98; const int c = 106;
String song = "";
volatile boolean notesPressed[8] = {false, false, false, false, false, false, false, false};
volatile boolean playButtonPressed = false;
volatile boolean clearButtonPressed = false;

// Misc.
const int GATE_DELAY = 60;
boolean blink_led = false;
unsigned long curr_time = 0;
unsigned long curr_timeB = 0;
const int closeGate = 165;
const int openGate = 140;

void setup() {
  // Set up the servos
  selectorServo.attach(selectorPin);
  gateServo.attach(gatePin);
  //  // Set up the DC motor
  //  pinMode(elevatorPin, OUTPUT);

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

  gateServo.write(closeGate);
  pinMode(playButton, INPUT);
  attachInterrupt(digitalPinToInterrupt(playButton), play, RISING);
}

void loop() {
  if (analogRead(clearButton) >= 1000) {
    song = "";
    digitalWrite(ledPin, LOW);
    Serial.println("Song cleared!");
    clearButtonPressed = false;
  }
  //  } else if (analogRead(playButton) >= 1000) {
  //    playSong(song);
  //  }

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

void test_gate() {
  gateServo.write(140);
  delay(60);
  gateServo.write(165);
}

void releaseOneMarble() {
  gateServo.write(165);
  delay(elevatorSpeed());
  gateServo.write(140);
  delay(60);
  gateServo.write(165);
}

int elevatorSpeed() {
  int potVal = analogRead(potPin);
  if (potVal < 341) {
    return 2000; // used to be 150
  } else if (potVal < 682) {
    return 1200; // used to be 200
  } else {
    return 750; // used to be 255
  }
}

void play() {
  if (millis() - curr_timeB < 1000) {
    return;
  }
  curr_timeB = millis();
//  Serial.println("calling play");
  playSong(song);
}

void playSong(String song) {
  gateServo.write(160); //Close gate
  blink_led = true;
  Serial.println(song);
  //For each note: move to the correct angle
  for (int i = 0; i < song.length(); i++) {
    int time_elapsed = 0;
    switch (song[i]) {
      case 'C':
        selectorServo.write(C);
        Serial.println("Close");
        break;
      case 'D':
        selectorServo.write(D);
        Serial.println("D");
        break;
      case 'E':
        selectorServo.write(E);
        Serial.println("E");
        break;
      case 'F':
        selectorServo.write(F);
        Serial.println("F");
        break;
      case 'G':
        selectorServo.write(G);
        Serial.println("G");
        break;
      case 'A':
        selectorServo.write(A);
        Serial.println("A");
        break;
      case 'B':
        selectorServo.write(B);
        Serial.println("B");
        break;
      case 'c':
        selectorServo.write(c);
        Serial.println("c");
        break;
      default:
        break;
    }
    delay(500);
    gateServo.write(openGate); //Open
    Serial.println("Open");
    curr_time = millis();
    while (millis() - curr_time <= 45) {
      //hi there
    }
    gateServo.write(closeGate); //Close
    Serial.println("Close");
    delay(500);
  }
  gateServo.write(closeGate); //Close gate
}
