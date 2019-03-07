#include <SPI.h>
#include <TMC26XStepper.h>

/*
 * SPI Info
 * SCK =  orig 20 / PB11   <=> new 6 / PA20
 * SDI "MOSI" = orig 21 / PB10  => new 12 / PA19
 * SDO "MISO" = orig 18 / PA12  => new 10 / PA18
 */

// These values are for h.boson_1.0
// 200 steps per rotation, CS pin 7, dir pin 36, step pin 31 and a current of 300mA

TMC26XStepper stepboi = TMC26XStepper(200,7,36,31,700);

int step_speed =  200;

void setup() {
  Serial.begin(9600);
  //set this according to you stepper
  Serial.println("Configuring stepper driver");
  
  //char constant_off_time, char blank_time, char hysteresis_start, char hysteresis_end, char hysteresis_decrement
  stepboi.setSpreadCycleChopper(2,24,8,6,0);
  stepboi.setRandomOffTime(0);
  
  stepboi.setMicrosteps(32);
  stepboi.setStallGuardThreshold(4,0);
  Serial.println("config finished, starting");
  stepboi.start();
  Serial.println("started");
}

void loop() {
  // First step one direction and print out stall guard.
  stepboi.setSpeed(step_speed);
  stepboi.step(step_speed);
  stepboi.move();
  if (stepboi.getStepsLeft()%100==0) {
    Serial.print("Stall Guard: ");
    Serial.println(stepboi.getCurrentStallGuardReading());
  }
  delay(5000); // delay 5 seconds

  // Next step the opposite direction
  stepboi.step(-step_speed);
  stepboi.move();
  if (stepboi.getStepsLeft()%100==0) {
    Serial.print("Stall Guard: ");
    Serial.println(stepboi.getCurrentStallGuardReading());
  }
  delay(5000); // delay 5 seconds
}
