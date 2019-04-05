/*
  The Beginning (TM)
 */

#include <SPI.h>
#include <SD.h>

//resistor communication variables
const int sd_chipSelect = 4;
const int photores_pin = 24;

// ESP communication variables
const int STAGE = 0x1F; // STAND IN VARIABLE; memory register address for when it's time to stage
const int esp_chipSelect = 3;

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


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(sd_chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  // pull data from resistor
  int photores = analogRead(photores_pin);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
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




// this is the class he sent in the channel :D
// https://github.com/esp8266/Arduino/tree/master/libraries/SPISlave


    uint32_t readStatus() {
      digitalWrite(esp_chipSelect, LOW);
      SPI.transfer(0x04);
      uint32_t status = (SPI.transfer(0) | ((uint32_t)(SPI.transfer(0)) << 8) | ((uint32_t)(SPI.transfer(0)) << 16) | ((uint32_t)(SPI.transfer(0)) << 24));
      digitalWrite(esp_chipSelect, HIGH);
      return status;
    }

    void writeStatus(uint32_t status) {
      digitalWrite(esp_chipSelect, LOW);
      SPI.transfer(0x01);
      SPI.transfer(status & 0xFF);
      SPI.transfer((status >> 8) & 0xFF);
      SPI.transfer((status >> 16) & 0xFF);
      SPI.transfer((status >> 24) & 0xFF);
      digitalWrite(esp_chipSelect, HIGH);
    }

    void readData(uint8_t * data) {
      digitalWrite(esp_chipSelect, LOW);
      SPI.transfer(0x03);
      SPI.transfer(0x00);
      for (uint8_t i = 0; i < 32; i++) {
        data[i] = SPI.transfer(0);
      }
      digitalWrite(esp_chipSelect, HIGH);
    }

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

    String readData() {
      char data[33];
      data[32] = 0;
      readData((uint8_t *)data);
      return String(data);
    }

    void writeData(const char * data) {
      writeData((uint8_t *)data, strlen(data));
    }
