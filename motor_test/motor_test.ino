#define STEP_PIN 0
#define DIR_PIN  1

#define DELAY 100
#define STEP_COUNT 1000

int nstep = 0;
int dir_multi = 1;

void setup() {

  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN,  OUTPUT);
  digitalWrite(STEP_PIN,LOW);
  digitalWrite(DIR_PIN, LOW);
}

void loop() {

  digitalWrite(DIR_PIN, LOW);
  dir_multi = -1;
  
  for( int i = 0; i < STEP_COUNT; i++ ) {
    digitalWrite(STEP_PIN,HIGH);
    delayMicroseconds(DELAY);
    digitalWrite(STEP_PIN,LOW);
    nstep += dir_multi;
  }

  digitalWrite(DIR_PIN, HIGH);
  dir_multi = 1;
  
  for( int i = 0; i < STEP_COUNT; i++ ) {
    digitalWrite(STEP_PIN,HIGH);
    delayMicroseconds(DELAY);
    digitalWrite(STEP_PIN,LOW);
    nstep += dir_multi;
  }
  
}
