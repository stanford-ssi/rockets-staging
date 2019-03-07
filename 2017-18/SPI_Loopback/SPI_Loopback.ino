#include <SPI.h>
void setup() {
  Serial.begin(9600);
  SPI.begin();
}

void loop() {
  int i,errs=0;
  uint8_t buff[32];

  for(i=0;i<sizeof(buff); i++) buff[i]=i;
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(buff,sizeof(buff));
  SPI.endTransaction();
  for(i=0;i<sizeof(buff); i++) if (buff[i]!=i) errs++;
  Serial.print("errs "); Serial.println(errs);
  delay(2000);
}
