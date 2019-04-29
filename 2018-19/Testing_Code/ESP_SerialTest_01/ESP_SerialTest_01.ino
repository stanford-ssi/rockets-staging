#include <SoftwareSerial.h>

const int rx = 2; // IO2 on esp8266 = 3rd from bottom left
const int tx = 16; // IO16 on esp8266 = 2nd from top right
SoftwareSerial samdSerial(rx, tx);

int counter = 0;

void setup() {
  samdSerial.begin(9600);
  SerialUSB.begin(9600);
}

void loop() {
  SerialUSB.println("Hello");
  samdSerial.write("Hello");
  samdSerial.write(counter);
  SerialUSB.println(counter);
  counter += 1;
  delay(100);
}
