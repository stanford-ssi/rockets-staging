#include <SPI.h>
#include <TMC26XStepper.h>

//we have a stepper motor with 200 steps per rotation, CS pin 10, dir pin 6, step pin 7 and a current of 300mA
const int steps_per_rev = 200;
const int cs_pin = 2;
const int dir_pin = 6;
const int step_pin = 7;

TMC26XStepper tmc26XStepper = TMC26XStepper(steps_per_rev, cs_pin, dir_pin, step_pin, 700);

int curr_step;
int speed =  0;
int speedDirection = 100;
int maxSpeed = 1000;

const int serial_speed = 9600;
int counter = 0;

void setup() {
  startSerial();
  initializePins();
  startMotor();
}

void loop() {
  digitalWrite(dir_pin, HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for (int x = 0; x < 200; x++) {
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(500);
    digitalWrite(step_pin, LOW);
    delayMicroseconds(500);
  }
  delay(1000); // One second delay

  digitalWrite(dir_pin, LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for (int x = 0; x < 400; x++) {
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(500);
    digitalWrite(step_pin, LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}

void startSerial() {
  while (!Serial);
  // Start the serial communication
  Serial.begin(serial_speed);
  // Print the header
  Serial.println("==============================");
  Serial.println("TMC26X Stepper Driver Tester");
  Serial.println("==============================");
}

void initializePins() {
  pinMode(cs_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
}

void startMotor() {
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
