#include <SPI.h>

// Motor Driver communication variables
const int md_chipSelect = 44; // PA21 on Arduino M0
const int md_step = 31;       // PA27 on Arduino M0
const int md_dir = 36;        // PB23 on Arduino M0
const int md_clk = 17;        // PA23 on Arduino M0
const int md_en = 37;         // PA22 on Arduino M0
const int md_sgtst = 35;      // PB22 on Arduino M0

// Motor Driver configuration bytes
byte chopconf = 10010000000110110100;
byte sgcsconf = 11010000000000011111;
byte drvconf = 11101111000000010000;
byte drvctrl = 00000000000000000000;
byte smarten = 10101000001000000010;

const int enable = 16;
int step_speed =  200;

void setup() {
  SPI.begin();
  Serial.begin(9600);
  
  pinMode(md_en, OUTPUT);
  pinMode(md_chipSelect, OUTPUT);  
  pinMode(md_step, OUTPUT);
  
  digitalWrite(md_en, HIGH); // enable the output of the chip

  Serial.println("Configuring chopper control register");
  writeMotor(chopconf);
  writeMotor(sgcsconf);
  writeMotor(drvconf);
  writeMotor(drvctrl);
  writeMotor(smarten);
  
}

void writeMotor(byte command){
  digitalWrite(md_chipSelect, LOW);
  SPI.transfer(command);
  digitalWrite(md_chipSelect, HIGH);
}

void loop() {
  digitalWrite(md_step, HIGH);
  delay(100);
  digitalWrite(md_step, LOW);
  delay(100);
}
