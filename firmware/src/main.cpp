#include <Arduino.h>
#include <Servo.h>
//HAL - Hardware Abstraction Layer
const int NUM_MOTORS = 6;
const int NUM_SERVOS = 4;
//index order -> 0: front left, 1: front right, 2: middle left, 3: middle right, 4: back left, 5: back right

//TODO --> wiring diagram for motor connections to the Teensy 4.1

//motor pin numbers (PWM pins)
const uint8_t MOTOR_PWM[NUM_MOTORS] = {255, 255, 255, 255, 255, 255};
//uint8_t is an unsigned 8-bit integer type, which can hold values from 0 to 255. 
//This is suitable for representing motor speeds, as it allows for a range of speeds from 0 (stopped) to 255 (full speed).
bool motorInvert[NUM_MOTORS] = { false, false, false, false, false, false };

//motor direction pin numbers
const uint8_t MOTOR_DIR[NUM_MOTORS] = {255, 255, 255, 255, 255, 255};

//servo pin array
const uint8_t SERVO_PIN[NUM_SERVOS] = {255, 255, 255, 255};


const uint8_t ESTOP_PIN = 255; //TODO: need real pin from wiring diagram
//i is the wheel index
//speed is ranged from -255 -> 255
void setMotor(uint8_t i, int speed){
  if(i>= NUM_MOTORS) return;

  speed = constrain(speed, -255, 255);

  bool forward = (speed>0);
  if (motorInvert[i]) forward = !forward;

  int duty = abs(speed);

  digitalWrite(MOTOR_DIR[i], forward); //forward = low and reverse = high
  analogWrite(MOTOR_PWM[i], forward ? duty : (255 - duty));
}

void stopMotors(){
  for(int i = 0; i < NUM_MOTORS; i++){
    setMotor(i, 0);
  }
}
//SERVOS

//for servos instead of writing them using analogWrite, we will declare a servo object that we can control
//we declare them as instances of the Servo class so that we can use the functions available
Servo steer[NUM_SERVOS];
int servoTrimUs[NUM_SERVOS] = {0, 0, 0, 0};
const int SERVO_CENTER_US = 1500;
const float STEER_LIMIT_DEG = 30.0f;   // conservative; raise after measuring real linkage travel
const float US_PER_DEG = 5.5f;

void setSteer(uint8_t i, float deg){
  if (i >= NUM_SERVOS) return;
//servos only understand pulse width in microseconds
//we are taking microseconds and translating them into human friendly degrees
//1500us -> 0 degrees or straight
//range -> 1000us to 2000us
//roughly 5.5us difference between degrees
//degree of rotation = (1500) + degree * scale
deg = constrain(deg, -STEER_LIMIT_DEG, STEER_LIMIT_DEG);
  int us = SERVO_CENTER_US + servoTrimUs[i] + (deg * US_PER_DEG);
  steer[i].writeMicroseconds(us);
}

void centerSteer(){
  for(int i = 0; i < NUM_SERVOS; i++){
    setSteer(i, 0);
  }
}

const bool ESTOP_ACTIVE_HIGH = true;  // TODO: confirm against sense circuit

bool readEstop(){
  return digitalRead(ESTOP_PIN) == (ESTOP_ACTIVE_HIGH ? HIGH : LOW);
}


void setup(){

}

void loop(){

}