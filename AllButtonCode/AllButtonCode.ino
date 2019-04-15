/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Button
*/

// constants won't change. They're used here to set pin numbers:
//const int buttonPin = A0;     // the number of the pushbutton pin
const int buttonPins[8] = {4, 12, 6, 7, 8, 9, 10, 11}; //8 pins, (need to change for motors)
const int playButton = 2;
const int clearButton = 3;

// variables will change:
int buttonState[8] = {0, 0, 0, 0, 0, 0, 0, 0};
char notes[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'c'};
int pattern[10];
String song = "";

void setup() {
  Serial.begin(9600);

  // Initialize the pins
  for (int i = 0; i < 8; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(playButton, INPUT);
  pinMode(clearButton, INPUT);

  // Set up the timer config
  // TODO: note that we will want to change OCR1A value to interrupt faster than 1 second
//  bitSet(TCCR1B, WGM12);        // set WGM1[2] = 1
//  bitSet(TIMSK1, OCH1E1A);      // enable interrupt on OCR1A register
  OCR1A = 15624;                // set Output Compare Register value
  TCCR1B = TCCR1B | 00000101;   // set CS1[2:0] = 101
}

void loop() {
  // read the state of the pushbutton value:
  for (int i = 0; i < 8; i++) {
    buttonState[i] = digitalRead(buttonPins[i]);
    if (buttonState[i] == HIGH) {
      song += notes[i];
    }
  }

  if (digitalRead(clearButton) == HIGH) {
    song = "";
    Serial.println("Song cleared!");
  }
  if (digitalRead(playButton) == HIGH) {
    Serial.println(song);
  }
  
  delay(175);
}

ISR (TIER1_COMPA_vect) {

}
