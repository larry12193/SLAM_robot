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

#define MIN_STEP_DELAY_US 500
#define MAX_STEP_DELAY_US 2000
#define DIFF_STEP_DELAY_US MIN_STEP_DELAY_US - MAX_STEP_DELAY_US

#include <ros.h>
#include <sensor_msgs/Joy.h>

volatile double left_x = 0;
volatile double left_y = 0;
volatile double left_delay  = MAX_STEP_DELAY_US;
volatile double right_delay = MAX_STEP_DELAY_US; 

int nstep = 0;
int dir_multi = 1;
char serIn;
bool move = false;

IntervalTimer left_motor;
IntervalTimer right_motor;


ros::NodeHandle nh;

void joy_Callback( const sensor_msgs::Joy& joy ) {

  left_x = joy.axes[AXES_LEFT_JOY_LR];
  left_y = joy.axes[AXES_LEFT_JOY_UD];

  double magnitude = sqrt(pow(left_x,2) + pow(left_y,2));

  double vel = DIFF_STEP_DELAY_US*abs(left_y) + MAX_STEP_DELAY_US;

  double differential = vel*left_x;

  int left_vel = (int)vel + (int)differential;
  int right_vel = (int)vel - (int)differential;

  if( left_vel < MIN_STEP_DELAY_US ) left_vel = MIN_STEP_DELAY_US;
  if( right_vel < MIN_STEP_DELAY_US ) right_vel = MIN_STEP_DELAY_US;
  
  left_motor.update(left_vel);
  right_motor.update(right_vel);
  
  if( magnitude < MAGNITUDE_DEADZONE ) {
    left_x = 0;
    left_y = 0;
  }
}

ros::Subscriber<sensor_msgs::Joy> joy_sub("/joy", &joy_Callback);


void left_motor_Interrupt() {
  static bool state = false;
  if( move ) {
    if( state ) digitalWrite(L_STEP_PIN,HIGH);
    else        digitalWrite(L_STEP_PIN,LOW);
    state = !state;
  }
}

void right_motor_Interrupt() {
  static bool state = false;
  if( move ) {
    if( state ) digitalWrite(R_STEP_PIN,HIGH);
    else        digitalWrite(R_STEP_PIN,LOW);
    state = !state;
  }
}

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

  nh.initNode();
  nh.subscribe(joy_sub);

  left_motor.begin(left_motor_Interrupt, MAX_STEP_DELAY_US);
  right_motor.begin(right_motor_Interrupt, MAX_STEP_DELAY_US);
}

void loop() {

  nh.spinOnce();

  noInterrupts();

  move = false;

  if( left_y > 0 ) {
    digitalWrite(R_DIR_PIN, HIGH);
    digitalWrite(L_DIR_PIN, HIGH);
    move = true;
  } else if( left_y < 0 ) {
    digitalWrite(R_DIR_PIN, LOW);
    digitalWrite(L_DIR_PIN, LOW);
    move = true;
  }

  interrupts();   
}
