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

////Working signal processing
//int ledPin = 8;
//volatile byte state = LOW;
//
//void setup() {
//  pinMode(ledPin, OUTPUT);
//
//  // enable interrupts
//  interrupts();
//
//  Serial.begin(19200);
//
//  //Timer 2 setup
//  TCCR2A =   (1 << COM2A1) | (1 << COM2A0)   //COM2A: controls OC2A/Arduino pin 11 output mode
//             | (1 << COM2B1) | (1 << COM2B0) //COM2B: controls OC2B/Arduino pin 3 output mode
//             | (0 << WGM21) | (1 << WGM20);  //WGM: Phase correct PWM (doubles period)
//  TCCR2B = (0 << WGM22)
//           | (1 << CS22) | (0 << CS21) | (1 << CS20) ;  //Timer2:prescaler=1024
//  TIMSK2 = (1 << TOIE2);                                //Enable Timer2 Overflow Interrupt
//  TIFR2  = (1 << TOV2);                                 //Clear TOV2 / clear pending interrupts
//
//}
//
//void loop() {
//  Serial.println(state);
//  digitalWrite(ledPin, state);
//}
//// interrupt every 512 cycles (16,000,000/512Hz=31250)
//ISR (TIMER2_OVF_vect)
//{
//  state = !state;
//}
