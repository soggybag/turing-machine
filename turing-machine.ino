
// http://www.multiwingspan.co.uk/arduino.php?page=led5

int ledPin[] = {6, 7, 8, 9, 10, 11, 12, 13};

byte d = 0b00101101; // starting binary value

int pot_1 = 0; // value read from pot 1 A0 clock speed
int pot_2 = 0; // value read from pot 2 A1 random probablity

// Init
void setup() {
  // Random
  randomSeed(millis());

  // Init d
  d = random(0, 255);
  
  // bytes
  for (int i = 0; i < 8; i++) {
    pinMode(ledPin[i], OUTPUT);
  }

  // value 
  pinMode(3, OUTPUT);
  
  // Testing
  Serial.begin(9600);
}

// main
void loop() {
  // Read Pots
  readPots();
  long delay_ms = map(pot_1, 0, 1023, 0, 1000);
  
  //  rotate
  d = rotateByte(d, 1);

  // flip last bit maybe
  if (random(0, 1000) < pot_2) {
    d = (d ^ (1 << 7));
    Serial.println(int(d));
  }

  // Display byte
  displayBinary(d);

  // Display value
  analogWrite(3, d);

  delay(delay_ms);

  // Testing 
  // Serial.print("byte int: ");
  // Serial.println(int(d));
  // Serial.println(pot_2);
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
