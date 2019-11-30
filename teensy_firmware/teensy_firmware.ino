#define R_DIR_PIN  2
#define R_STEP_PIN 3
#define L_DIR_PIN  7
#define L_STEP_PIN 6

#define DELAY 1000
#define STEP_COUNT 1000

#define AXES_LEFT_JOY_LR  0
#define AXES_LEFT_JOY_UD  1
#define AXES_LEFT_LT      2
#define AXES_RIGHT_JOY_LR 3
#define AXES_RIGHT_JPY_UD 4
#define AXES_RIGHT_RT     5
#define AXES_DIR_PAD_LR   6
#define AXES_DIR_PAD_UD   7

#define BUTTON_A         0
#define BUTTON_B         1
#define BUTTON_X         2
#define BUTTON_Y         3
#define BUTTON_LB        4
#define BUTTON_RB        5
#define BUTTON_BACK      6
#define BUTTON_START     7
#define BUTTON_XBOX      8
#define BUTTON_LEFT_JOY  9
#define BUTTON_RIGHT_JOY 10

#define MAGNITUDE_DEADZONE 0.1

#include <ros.h>
#include <sensor_msgs/Joy.h>

volatile double left_x = 0;
volatile double left_y = 0;

void joy_Callback( const sensor_msgs::Joy& joy ) {

  left_x = joy.axes[AXES_LEFT_JOY_LR];
  left_y = joy.axes[AXES_LEFT_JOY_UD];

  double magnitude = sqrt(pow(left_x,2) + pow(left_y,2));
  if( magnitude < MAGNITUDE_DEADZONE ) {
    left_x = 0;
    left_y = 0;
  }
}

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

  if( left_x > 0 ) {
    digitalWrite(R_DIR_PIN, HIGH);
    digitalWrite(L_DIR_PIN, HIGH);
    move = true;
  } else if( left_x < 0 ) {
    digitalWrite(R_DIR_PIN, LOW);
    digitalWrite(L_DIR_PIN, LOW);
    move = true;
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
