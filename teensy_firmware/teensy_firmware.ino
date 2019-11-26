#define R_DIR_PIN  2
#define R_STEP_PIN 3
#define L_DIR_PIN  7
#define L_STEP_PIN 6

#define DELAY 1000
#define STEP_COUNT 1000

int nstep = 0;
int dir_multi = 1;
char serIn;
bool move = false;

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


  move = false;

  if( Serial.available() > 0 ) {
    serIn = Serial.read();
    
    if( serIn == 'W' ) {
      digitalWrite(R_DIR_PIN, HIGH);
      digitalWrite(L_DIR_PIN, HIGH);
      move = true;
    } else if( serIn == 'S' ) {
      digitalWrite(R_DIR_PIN, LOW);
      digitalWrite(L_DIR_PIN, LOW);
      move = true;
    } else if( serIn == 'A' ) {
      digitalWrite(R_DIR_PIN, HIGH);
      digitalWrite(L_DIR_PIN, LOW);
      move = true;
    } else if( serIn == 'D' ) {
      digitalWrite(R_DIR_PIN, LOW);
      digitalWrite(L_DIR_PIN, HIGH);
      move = true;
    }
  }
  if ( move ) {
    for(int i = 0; i < 50; i++ ){
      digitalWrite(R_STEP_PIN,HIGH);
      digitalWrite(L_STEP_PIN,HIGH);
      delayMicroseconds(DELAY);
      digitalWrite(R_STEP_PIN,LOW);
      digitalWrite(L_STEP_PIN,LOW);
    }
    
  }
}
