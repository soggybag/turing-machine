// Mock up of Music Thing Modular Turing Machine with Arduino
// Added MsTimer, moved shiftregister code out of loop


// Includes -------------------------------------------------
#include <MsTimer2.h>


// Declare vars ---------------------------------------------
int ledPin[] = {6, 7, 8, 9, 10, 11, 12, 13};
int cv_out_pin = 5;

byte d = 0b10001101; // starting binary value

int pot_1 = 0; // value read from pot 1 A0 clock speed
int pot_2 = 0; // value read from pot 2 A1 random probablity

unsigned long count = 400;
unsigned long delay_ms = 0;


// Setup ----------------------------------------------------
void setup() {
  // Random
  randomSeed(millis());

  // Init d
//  d = random(0, 255);
  
  // bytes
  for (int i = 0; i < 8; i++) {
    pinMode(ledPin[i], OUTPUT);
  }

  // value 
  pinMode(cv_out_pin, OUTPUT);
  analogWrite(cv_out_pin, d);
  
  // Testing
  Serial.begin(9600);

  // setup timer
  
  MsTimer2::set(1, setCount);
  MsTimer2::start();
}

// main ----------------------------------------------------
void loop() {
  // Read Pots
  readPots();
  delay_ms = map(pot_1, 0, 1023, 0, 1000);

  if (count > delay_ms) {
//    Serial.print("pot_1:");
//    Serial.print(pot_1);
//    Serial.print(" delay ms:");
//    Serial.print(delay_ms);
//    Serial.println();   

    long r = random(0, 1000);
    
//    Serial.print("pot_2: ");
//    Serial.print(pot_2);
//    Serial.print(" r:");
//    Serial.println(r);
    
    count = 0;

    //  rotate
    d = rotateByte(d, 1);
    
    // flip last bit maybe
    if (r < pot_2) {
      d = (d ^ (1 << 7));
    }
  
    // Display byte
    displayBinary(d);
    
    // Display value
    Serial.print("D:");
    Serial.println(d);
    
    analogWrite(cv_out_pin, d);
  }
}

// Set Count -----------------------------------------------
void setCount() {
  count ++;
}

// rotate byte
byte rotateByte(byte value, int amount) {
  return (value >> amount) | (value << (8 - amount));
}

// Display byte with LEDs on pins 6 to 13
void displayBinary(byte numToShow) {
  for (int i = 0; i < 8; i++) {
    if (bitRead(numToShow, i) == 1) {
      digitalWrite(ledPin[i], HIGH); 
    } else {
      digitalWrite(ledPin[i], LOW); 
    }
  }
}

// Read Pots
void readPots() {
  pot_1 = analogRead(A0); // Speed
  pot_2 = analogRead(A1); // probability
}
