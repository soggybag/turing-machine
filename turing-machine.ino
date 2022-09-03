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
  randomSeed(analogRead(A5)); // Seed with random value from unused analog pin

  // Init d
  d = random(0, 255); // generate initial random value
  
  // initialize LED byte pins
  for (int i = 0; i < 8; i++) {
    pinMode(ledPin[i], OUTPUT);
  }

  // initialize LED value pin
  pinMode(cv_out_pin, OUTPUT);
  analogWrite(cv_out_pin, d);
  
  // Testing
  Serial.begin(9600);

  // setup timer
  MsTimer2::set(1, setCount); // Call setCount every MS
  MsTimer2::start(); // Start the timer
}

// main ----------------------------------------------------
void loop() {
  // Read Pots
  readPots();

  // map values 
  delay_ms = map(pot_1, 0, 1023, 0, 1000);

  // Check count interval
  if (count > delay_ms) {
    // generate a random value
    long r = random(0, 1000);

    // reset the counter
    count = 0;

    // rotate the byte
    d = rotateByte(d, 1);
    
    // If the random value is less than the probability pot value flip the last bit of te byte
    if (r < pot_2) {
      d = (d ^ (1 << 7));
    }
  
    // Display byte
    displayBinary(d);
    
    // Display value
    analogWrite(cv_out_pin, d);
  }
}

// Set Count -----------------------------------------------
void setCount() {
  // increment count every MS
  count ++;
}

// rotate byte
byte rotateByte(byte value, int amount) {
  // shift the byte and move the last bit to the end
  return (value >> amount) | (value << (8 - amount));
}

// Display byte with LEDs on pins 6 to 13
void displayBinary(byte numToShow) {
  // Loop over LED pins and display the matching bit of the byte
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
  // read pots 1 and 2
  pot_1 = analogRead(A0); // Speed
  pot_2 = analogRead(A1); // probability
}
