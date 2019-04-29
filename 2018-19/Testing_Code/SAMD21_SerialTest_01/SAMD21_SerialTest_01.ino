
// Serial2 pin and pad definitions (in Arduino files Variant.h & Variant.cpp)
#define PIN_SERIAL2_RX       (34ul)               // Pin description number for PIO_SERCOM on D12
#define PIN_SERIAL2_TX       (36ul)               // Pin description number for PIO_SERCOM on D10
#define PAD_SERIAL2_TX       (UART_TX_PAD_2)      // SERCOM pad 2
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_3)    // SERCOM pad 3

// Instantiate the Serial2 class
Uart Serial2(&sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

void setup()
{
  Serial2.begin(9600);          // Begin Serial2
  SerialUSB.begin(9600);
  while (!SerialUSB){
    ;
  }
  SerialUSB.println("ESP-SAMD21 Communication Test");
}

void loop()
{ 
  if (Serial2.available())        // Check if incoming data is available
  {
    byte byteRead = Serial2.read();    // Read the most recent byte 
    // Serial2.write(byteRead);      // Echo the byte back out on the serial port
    SerialUSB.println(byteRead);
  }
}

void SERCOM1_Handler()    // Interrupt handler for SERCOM1
{
  Serial2.IrqHandler();
}
