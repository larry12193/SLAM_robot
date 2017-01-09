// Define pinout
int leftStepPin = 7;
int leftDirPin = 8;
int rightStepPin = 9;
int rightDirPin = 10;
int curDir = 0;

char inChar;

void setup() {
  // Setup serial coms on TX/RX to get commands from raspi
  Serial1.begin(115200);
 
  // Configure right motor pins 
  pinMode(rightDirPin, OUTPUT);
  pinMode(rightStepPin, OUTPUT);
  digitalWrite(rightDirPin, HIGH);

  // Configure left motor pins
  pinMode(leftDirPin, OUTPUT);
  pinMode(leftStepPin, OUTPUT);
  digitalWrite(leftDirPin, HIGH);
}

void loop() {
  // Check if serial data available
  if(Serial1.available()) {
    inChar = (char)Serial1.read();
    // Process data
    switch(inChar) {
      case 'w':
        goForward();
        break;
      case 's':
        goBackward();
        break;
      case 'd':
        turnRight();
        break;
      case 'a':
        turnLeft();
        break;
    }
  }
}

void goForward() {
  // Set both motors for forward rotation
  digitalWrite(rightDirPin, LOW);
  digitalWrite(leftDirPin, LOW);
  // Rotate 1 revolution
  quarterRotation();
}

void goBackward() {
  // Set both motors for backward rotation
  digitalWrite(rightDirPin, HIGH);
  digitalWrite(leftDirPin, HIGH);
  // Rotate 1 revolution
  quarterRotation();
}

void turnRight() {
  // Set left motor forward, right backward
  digitalWrite(rightDirPin, HIGH);
  digitalWrite(leftDirPin, LOW);
  // Rotate 1 revolution
  quarterRotation();
}

void turnLeft() {
  // Set right motor forward, left backward
  digitalWrite(rightDirPin, LOW);
  digitalWrite(leftDirPin, HIGH);
  // Rotate 1 revolution
  quarterRotation();
}

void quarterRotation() {
  for( int i = 0; i < 800; i++) {
    digitalWrite(rightStepPin,LOW);
    digitalWrite(leftStepPin,LOW);
    delay(1);
    digitalWrite(rightStepPin,HIGH);
    digitalWrite(leftStepPin,HIGH);
    delay(1);
  }
}

