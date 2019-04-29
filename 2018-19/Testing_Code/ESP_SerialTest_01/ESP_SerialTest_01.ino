#include <SoftwareSerial.h>

const int rx = 10;
const int tx = 11;
SoftwareSerial samdSerial(rx, tx);

int counter = 0;

void setup() {
  samdSerial.begin(9600);
  
}

void loop() {
  samdSerial.write("Hello");
  samdSerial.write(counter);
  counter += 1;
  delay(100);
}
