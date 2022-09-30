// Mock up of Music Thing Modular Turing Machine with Arduino
// Added MsTimer, moved shiftregister code out of loop

// Includes -------------------------------------------------
#include <MsTimer2.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>

// Declare vars ---------------------------------------------
int ledPin[] = {6, 7, 8, 9, 10, 11, 12, 13};
int cv_out_pin = 5;

byte d = 0b10001101; // starting binary value

int pot_1 = 0; // value read from pot 1 A0 clock speed
int pot_2 = 0; // value read from pot 2 A1 random probablity

unsigned long count = 400;
unsigned long delay_ms = 0;

Adafruit_MCP4725 MCP4725;

// Setup ----------------------------------------------------
void setup() {
  // Testing
  Serial.begin(9600);
  
  // Initialize MCP4725
  MCP4725.begin(0x60);
  
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
    // d = leftRotate(d, 1);
    // d = rightRotate(d, 1);
    
    // If the random value is less than the probability pot value flip the last bit of te byte
    if (r < pot_2) {
      d = (d ^ (1 << 7));
    }
  
    // Display byte
    displayBinary(d);

     printBits(d);

    // Output Value
    // Display value
    analogWrite(cv_out_pin, d);

    // MCP Out
    // int out = d / 255.0 * 4096.0;
    MCP4725.setVoltage((uint16_t) d, false);
    // Serial.print(out);
    // Serial.print(" - ");
    Serial.println((analogRead(A6) * 5.0) / 1024.0); // **** Test Counter 
  }
}

// Set Count -----------------------------------------------
void setCount() {
  // increment count every MS
  count ++;
}

// Left Rotate
int leftRotate(int n, unsigned int d) {
  return (n << d) | (n >> (8 - d));
}

// Right Rotate
int rightRotate(int n, unsigned int d) {
  return (n >> d) | (n << (8 - d));
}

// rotate byte
byte rotateByte(byte value, int amount) {
  // shift the byte and move the last bit to the end
  return (value >> amount) | (value << (8 - amount));
}

// Flip last bit
int flipLast(int n) {
  return n ^= 0b00000001;
}

// Flip first bit
int fliFirst(int n) {
   return n ^= 0b10000000;
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

// Print Bits 
void printBits(byte n) {
  // Loop over LED pins and display the matching bit of the byte
  for (int i = 0; i < 8; i++) {
    if (bitRead(n, i) == 1) {
      Serial.print("1");
    } else {
      Serial.print("0"); 
    }
  }
  Serial.println();
}

// Read Pots
void readPots() {
  // read pots 1 and 2
  pot_1 = analogRead(A0); // Speed
  pot_2 = analogRead(A1); // probability
}
