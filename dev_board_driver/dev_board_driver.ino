#include <SPI.h>
#include <TMC26XStepper.h>

//we have a stepper motor with 200 steps per rotation, CS pin 2, dir pin 6, step pin 7 and a current of 300mA
const int steps_per_rev = 200;
const int cs_pin = 4;
const int dir_pin = 6;
const int step_pin = 5;
TMC26XStepper tmc26XStepper = TMC26XStepper(steps_per_rev, cs_pin, dir_pin, step_pin, 700);

int curr_step;
int speed =  0;
int speedDirection = 100;
int maxSpeed = 1000;

const int right_pin = 1;
const int left_pin = 2;
const int run_pin = 3;
const int stop_pin = 7;
const int led_pin = 13;

const int serial_speed = 9600;

// 0 = stopped; 1 = run; 2 = right; 3 = left
int motor_state = 0;

void setup() {
  startSerial();
  startMotor();
  initializePins();
  initializeInterrupts();
}

void loop() {
  driveMotor();
}

void startSerial(){
  // Start the serial communication
  Serial.begin(serial_speed);
  // Print the header
  Serial.println("==============================");
  Serial.println("TMC26X Stepper Driver Tester");
  Serial.println("==============================");
}

void startMotor(){
  Serial.println("Configuring stepper driver");
  //char constant_off_time, char blank_time, char hysteresis_start, char hysteresis_end, char hysteresis_decrement
  tmc26XStepper.setSpreadCycleChopper(2, 24, 8, 6, 0);
  tmc26XStepper.setRandomOffTime(0);

  tmc26XStepper.setMicrosteps(32);
  //sgThreshold, sgFilter
  tmc26XStepper.setStallGuardThreshold(4, 0);
  Serial.println("config finished, starting");
  tmc26XStepper.start();
  tmc26XStepper.setEnabled(true);
  Serial.println("started");
}

void initializePins(){
  pinMode(cs_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(right_pin, INPUT);
  pinMode(left_pin, INPUT);
  pinMode(run_pin, INPUT);
  pinMode(stop_pin, INPUT);
}

void initializeInterrupts(){
  attachInterrupt(digitalPinToInterrupt(right_pin), RightMotor, RISING);
  attachInterrupt(digitalPinToInterrupt(left_pin), LeftMotor, RISING);
  attachInterrupt(digitalPinToInterrupt(run_pin), RunMotor, RISING);
  attachInterrupt(digitalPinToInterrupt(stop_pin), StopMotor, RISING);
}

void RightMotor(){
  motor_state = 2;
}

void LeftMotor(){
  motor_state = 3;
}

void RunMotor(){
  motor_state = 1;
}

void StopMotor(){
  motor_state = 0;
}

void driveMotor(){
  switch (motor_state){
    case 0: // Stopped
      digitalWrite(led_pin, LOW);
      tmc26XStepper.stop();
      break;
    case 1: // Run back and forth
      
      break;
    case 2: // Right Turning
      digitalWrite(led_pin, HIGH);
      if (!tmc26XStepper.isMoving()) {
        speed += speedDirection;
        if (speed > maxSpeed) {
          speed = maxSpeed;
          speedDirection = -speedDirection;
        } else if (speed < 0) {
          speedDirection = -speedDirection;
          speed = speedDirection;
        }
        //setting the speed
        Serial.print("setting speed to ");
        Serial.println(speed);
        tmc26XStepper.setSpeed(speed);
        //we want some kind of constant running time - so the length is just a product of speed
        Serial.print("Going ");
        Serial.print(10 * speed);
        Serial.println(" steps");
        tmc26XStepper.step(10 * speed);
      } else {
        //we put out the status every 100 steps
        if (tmc26XStepper.getStepsLeft() % 100 == 0) {
          Serial.print("Stall Guard: ");
          Serial.println(tmc26XStepper.getCurrentStallGuardReading());
        }
      }
      tmc26XStepper.move();
      break;
    case 3: // Left Turning
      
      break;
  }
}
