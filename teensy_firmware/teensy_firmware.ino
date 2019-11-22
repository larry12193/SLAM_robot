#define R_DIR_PIN  2
#define R_STEP_PIN 3
#define L_DIR_PIN  7
#define L_STEP_PIN 6

#define DELAY 500
#define STEP_COUNT 1000

int nstep = 0;
int dir_multi = 1;
char serIn;

void setup() {

  Serial.begin(115200);
  while( Serial.available() > 0) serIn=Serial.read();
  pinMode(R_DIR_PIN,  OUTPUT);
  pinMode(R_STEP_PIN, OUTPUT);
  pinMode(L_DIR_PIN,  OUTPUT);
  pinMode(L_STEP_PIN, OUTPUT);

  digitalWrite(R_DIR_PIN,  LOW);
  digitalWrite(R_STEP_PIN, LOW);
  digitalWrite(L_DIR_PIN,  LOW);
  digitalWrite(L_STEP_PIN, LOW);
}

void loop() {

  if( Serial.available() > ) {
    serIn = Serial.read();
  }

  digitalWrite(R_DIR_PIN, HIGH);
  digitalWrite(L_DIR_PIN, HIGH);
  dir_multi = -1;
  
  for( int i = 0; i < STEP_COUNT; i++ ) {
    digitalWrite(R_STEP_PIN,HIGH);
    digitalWrite(L_STEP_PIN,HIGH);
    delayMicroseconds(DELAY);
    digitalWrite(R_STEP_PIN,LOW);
    digitalWrite(L_STEP_PIN,LOW);
    nstep += dir_multi;
  }

  digitalWrite(R_DIR_PIN, HIGH);
  digitalWrite(L_DIR_PIN, HIGH);
  dir_multi = -1;
  
  for( int i = 0; i < STEP_COUNT; i++ ) {
    digitalWrite(R_STEP_PIN,HIGH);
    digitalWrite(L_STEP_PIN,HIGH);
    delayMicroseconds(DELAY);
    digitalWrite(R_STEP_PIN,LOW);
    digitalWrite(L_STEP_PIN,LOW);
    nstep += dir_multi;
  }
  
}
