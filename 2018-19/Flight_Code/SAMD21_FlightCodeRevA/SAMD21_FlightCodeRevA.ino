/*
 * ### The Beginning (TM) ###
 * Staging code for h.boson_1.1 Staging Olympus Rockets SSI 2019
 * Currently has ability to receive data from the photores pin and log it into the microSD card.
 * In development:
 *  - ESP communication (esp wroom 02 is in slave mode)
 *  - Motor Driver
 *  - Controlling boost converter
 * Example class for using ESP8266 in Slave mode:
 *   https://github.com/esp8266/Arduino/tree/master/libraries/SPISlave
 *  
 * ABBREVIATIONS USED
 *   pr   |   Photoresistor
 *   esp  |   ESP WROOM 02
 *   md   |   Motor Driver
 *   sd   |   microSD
 *   bc   |   Boost Converter
 *   en   |   Enable
 */

#include <SPI.h>
#include <SD.h>

// Configuration parameters - will determine whether or not certain methods are run
const bool bool_sd_log = true;
const bool bool_photores_read = true;
const bool bool_esp_comms = false;
const bool bool_drive_motor = false;
const bool bool_boost_enable = false;

// Photoresistor variables
const int pr_pin = 24;        // PA02 on Arduino M0

// microSD communication variables
const int sd_chipSelect = 4;  // PA10 on Arduino M0

// ESP communication variables
const int STAGE = 0x1F; // STAND IN VARIABLE; memory register address for when it's time to stage
const int esp_chipSelect = 3; // PA09 on Arduino M0

// Motor Driver communication variables
const int md_chipSelect = 44; // PA21 on Arduino M0
const int md_step = 31;       // PA27 on Arduino M0
const int md_dir = 36;        // PB23 on Arduino M0
const int md_clk = 17;        // PA23 on Arduino M0
const int md_en = 37;         // PA22 on Arduino M0
const int md_sgtst = 35;      // PB22 on Arduino M0

// Boost converter variables
const int bc_antiEn = 30;     // PB03 on Arduino M0
const int bc_en = 32;         // PA28 on Arduino M0

void setup() {
  // "Hey! We're setting these pins as input/output pins!"
  pinMode(photores_pin, INPUT);
  pinMode(esp_chipSelect, OUTPUT);

  digitalWrite(esp_chipSelect, HIGH);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Start the SD card
  if (bool_sd_log) {
    Serial.print("Initializing SD card...");
    // see if the card is present and can be initialized:
    if (!SD.begin(sd_chipSelect)) {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      while (1);
    }
    Serial.println("card initialized.");
  }
}

void loop() {
  // pull data from resistor
  if (bool_photores_read) {
    int photores = analogRead(photores_pin);
  }

  // Log photores data to SD card
  if (bool_sd_log && bool_photores_read) {
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(photores);
      dataFile.close();
      // print to the serial port too:
      Serial.println(photores);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
  }

  if (bool_esp_comms){
    // Here is where we check the esp to see if it is time to stage
  }

  if (bool_boost_enable){
    // if we get the go from the esp, enable the boost
    if (bool_drive_motor){
      // Here is where we tell the motor to actuate
    }
  }
}

/*
 * The following commands are adapted from the example class in the description
 * for communicating with an esp in slave mode
 */

// ReadStatus asks the esp to send its status so that the mpu can read it
uint32_t readStatus() {
  digitalWrite(esp_chipSelect, LOW);
  SPI.transfer(0x04); // the bit 0x04 is the command to send status
  uint32_t status = (SPI.transfer(0) | ((uint32_t)(SPI.transfer(0)) << 8) | ((uint32_t)(SPI.transfer(0)) << 16) | ((uint32_t)(SPI.transfer(0)) << 24));
  digitalWrite(esp_chipSelect, HIGH);
  return status;
}

// writeStatus sets the status of the esp to what we say it should be
// status is 4 bytes long
void writeStatus(uint32_t status) {
  digitalWrite(esp_chipSelect, LOW);
  SPI.transfer(0x01);
  SPI.transfer(status & 0xFF);
  SPI.transfer((status >> 8) & 0xFF);
  SPI.transfer((status >> 16) & 0xFF);
  SPI.transfer((status >> 24) & 0xFF);
  digitalWrite(esp_chipSelect, HIGH);
}

// readData
void readData(uint8_t * data) {
  digitalWrite(esp_chipSelect, LOW);
  SPI.transfer(0x03);
  SPI.transfer(0x00);
  for (uint8_t i = 0; i < 32; i++) {
    data[i] = SPI.transfer(0);
  }
  digitalWrite(esp_chipSelect, HIGH);
}

// writeData
void writeData(uint8_t * data, size_t len) {
  uint8_t i = 0;
  digitalWrite(esp_chipSelect, LOW);
  SPI.transfer(0x02);
  SPI.transfer(0x00);
  while (len-- && i < 32) {
    SPI.transfer(data[i++]);
  }
  while (i++ < 32) {
    SPI.transfer(0);
  }
  digitalWrite(esp_chipSelect, HIGH);
}

// readData
String readData() {
  char data[33];
  data[32] = 0;
  readData((uint8_t *)data);
  return String(data);
}

// writeData
void writeData(const char * data) {
  writeData((uint8_t *)data, strlen(data));
}
