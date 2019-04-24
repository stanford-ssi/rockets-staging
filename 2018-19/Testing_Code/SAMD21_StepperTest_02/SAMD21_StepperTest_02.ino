/*
 * This is testing code to drive a stepper motor using the
 * TMC2660 stepper motor driver chip.
 * 
 * T. Jilk 2019
 */

#include <SPI.h>

// Motor Driver communication variables
const int md_chipSelect = 44; // PA21 on Arduino M0
const int md_step = 31;       // PA27 on Arduino M0
const int md_dir = 36;        // PB23 on Arduino M0
const int md_clk = 17;        // PA23 on Arduino M0
const int md_en = 37;         // PA22 on Arduino M0
const int md_sgtst = 35;      // PB22 on Arduino M0

// Motor Driver configuration bytes
byte chopconf = 10010000000110110100; // normal startup
byte sgcsconf = 11010000000000011111; // full current for both SPI and step/dir operation
byte drvconf = 11101111000000010000; // step dir interface enabled
byte drvctrl = 00000000000000001000; // fullstep, pulse interpolation disabled, only rising STEP edge active
byte smarten = 10101000001000000010; // coolStep control register - normal setup

const int enable = 16;
int step_speed =  200;

void setup() {
  SPI.begin();
  Serial.begin(9600);
  
  pinMode(md_en, OUTPUT);
  pinMode(md_chipSelect, OUTPUT);  
  pinMode(md_step, OUTPUT);
  pinMode(md_dir, OUTPUT);
  pinMode(md_clk, OUTPUT);
  
  digitalWrite(md_en, LOW); // enable the output of the chip (low = enabled)
  digitalWrite(md_dir, LOW); // choose a direction to begin with
  digitalWrite(md_clk, LOW); // writing low makes the chip use its on-board oscillator

  Serial.println("Configuring chopper control register");
  
  writeMotor(chopconf);
  writeMotor(sgcsconf);
  writeMotor(drvconf);
  writeMotor(drvctrl);
  writeMotor(smarten);
}

void writeMotor(byte command){
  digitalWrite(md_chipSelect, LOW);
  Serial.println(SPI.transfer(command));
  digitalWrite(md_chipSelect, HIGH);
}

void loop() {
  // supposedly a rising edge in md_step will cause the motor to take a step...
  digitalWrite(md_dir, !digitalRead(md_dir));
  delay(10);
  digitalWrite(md_step, HIGH);
  Serial.println("Stepping...");
  delay(1000); // min delay is 10 ns, so we should be good
  digitalWrite(md_step, LOW);
  delay(1000);
}
