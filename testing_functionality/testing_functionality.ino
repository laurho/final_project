const int buttonPins[8] = {2, 3, 4, 5, 6, 7, 8, 12};
char notes[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'c'};
String song = "";
const int playButton = A1;
const int clearButton = A2;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 8; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  Serial.begin(19200);

  pinMode(playButton, INPUT);
  pinMode(clearButton, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  boolean val = digitalRead(12);
//  boolean val = analogRead(playButton);
//  int val = analogRead(clearButton);
  Serial.println(val);

  
//  delay(1000);
//    for (int i = 0; i < 8; i++) {
//      Serial.println(notes[i]);
//      int val = digitalRead(3);
//      Serial.println(val);
////      if (val) {
////        song += notes[i];
////      }
//      delay(15);
//    }
//  Serial.println(song);
}
