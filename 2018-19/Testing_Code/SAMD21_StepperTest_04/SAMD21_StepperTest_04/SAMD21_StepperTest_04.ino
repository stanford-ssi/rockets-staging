/*
 * With TMC2660 BOB.
 * 
 * Gabe Alvarez, Tylor Jilk 2019
 */

#include <SPI.h>

// Motor Driver communication variables

const int md_chipSelect = 6;
const int md_step = 7;   //11    
const int md_dir = 4;    //12    
const int md_clk = 10;    //5    
const int md_en = 5;    //10     
const int md_sgtst = 9;  //9    

// Motor Driver configuration bytes
// byte chopconf = 10010000000110110100; // normal startup
// byte sgcsconf = 11010000000000011111; // full current for both SPI and step/dir operation
// byte drvconf = 11101111000000010000; // step dir interface enabled
// byte drvctrl = 00000000000000001000; // fullstep, pulse interpolation disabled, only rising STEP edge active
// byte smarten = 10101000001000000010; // coolStep control register - normal setup

unsigned long chopconf = 0x901B4ul;
unsigned long sgcsconf = 0xD001Ful;
unsigned long drvconf = 0xEF010ul;
unsigned long drvctrl = 0x8ul;
unsigned long smarten = 0xA8202ul;

const int enable = 16;
int step_speed =  200;

void setup() {
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  //SPI.setBitOrder(MSBFIRST); 14000000
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.begin();
  SerialUSB.begin(9600);
  while(!SerialUSB){
    ;
  }
  
  pinMode(md_en, OUTPUT);
  pinMode(md_chipSelect, OUTPUT);  
  pinMode(md_step, OUTPUT);
  pinMode(md_dir, OUTPUT);
  pinMode(md_clk, OUTPUT);
  
  digitalWrite(md_en, LOW); // enable the output of the chip (low = enabled)
  digitalWrite(md_dir, LOW); // choose a direction to begin with
  digitalWrite(md_clk, LOW); // writing low makes the chip use its on-board oscillator

  SerialUSB.println("Configuring chopper control register");
  
  writeMotor(chopconf);
  writeMotor(sgcsconf);
  writeMotor(drvconf);
  writeMotor(drvctrl);
  writeMotor(smarten);
}

void writeMotor(unsigned long command){
  digitalWrite(md_chipSelect, LOW);
  unsigned long response = 0xFFFFul;
  response = SPI.transfer((command >> 16) & 0xff);
  response <<= 8;
  response |= SPI.transfer((command >>  8) & 0xff);
  response <<= 8;
  response |= SPI.transfer((command) & 0xff);
  response >>= 4;
  digitalWrite(md_chipSelect, HIGH);
  SerialUSB.print("Response: ");
  SerialUSB.println(response, BIN);
}

void loop() {
  // supposedly a rising edge in md_step will cause the motor to take a step...
  digitalWrite(md_dir, !digitalRead(md_dir));
  delay(10);
  digitalWrite(md_step, HIGH);
  SerialUSB.println("Stepping...");
  delay(1000); // min delay is 10 ns, so we should be good
  digitalWrite(md_step, LOW);
  delay(1000);
  writeMotor(drvctrl);
}


/*

// Pins edited to fit h.boson_1.1/2

#include <TMCStepper.h>

//const int md_chipSelect = SS;
//const int md_step = 11;       
//const int md_dir = 12;        
//const int md_clk = SCK;        
//const int md_en = 10;         
//const int md_sgtst = 9; 

#define EN_PIN    10  // Enable
#define DIR_PIN   12  // Direction
#define STEP_PIN  11  // Step
#define CS_PIN    SS  // Chip select
// #define SW_MOSI   66  // Software Master Out Slave In (MOSI)
// #define SW_MISO   44  // Software Master In Slave Out (MISO)
// #define SW_SCK    64  // Software Slave Clock (SCK)
// #define SW_RX     63  // TMC2208/TMC2224 SoftwareSerial receive pin
// #define SW_TX     40  // TMC2208/TMC2224 SoftwareSerial transmit pin

#define R_SENSE 0.1  // Match to your driver
                     // SilentStepStick series use 0.11
                     // UltiMachine Einsy and Archim2 boards use 0.2
                     // Panucatt BSD2660 uses 0.1

// Select your stepper driver type
//TMC2130Stepper driver = TMC2130Stepper(CS_PIN, R_SENSE); // Hardware SPI
//TMC2130Stepper driver = TMC2130Stepper(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK); // Software SPI
//TMC2208Stepper driver = TMC2208Stepper(&Serial, R_SENSE); // Hardware Serial0
//TMC2208Stepper driver = TMC2208Stepper(SW_RX, SW_TX, R_SENSE); // Software serial
TMC2660Stepper driver = TMC2660Stepper(EN_PIN, R_SENSE); // Hardware SPI
//TMC2660Stepper driver = TMC2660Stepper(EN_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK); // Software SPI
//TMC5160Stepper driver = TMC5160Stepper(CS_PIN, R_SENSE);

void setup() {
  SPI.begin();
  pinMode(EN_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);

  driver.begin();
  driver.toff(2);
  driver.tbl(24);
  driver.microsteps(16);
  driver.rms_current(800);
}

void loop() {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(160);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(160);
}

*/
