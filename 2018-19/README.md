# This is a list of things that we have to do with the code / pcb before the launch, in this order.

- Check pcb for shorts
- Assemble pcb and reflow
- Solder on larger components
- Double check that everything is connected properly
- Turn on power supplies and see if we actually get 3.3V from buck1
- Flash Arduino bootloader onto the Samd21
- Flash code onto the esp

## Note that all code is currently FlightCodeRevA

===========================
## TEST 1: SAMD21 Talks
===========================
- Blink sketch
- Write to SD card and log from photoresistor
- Controls boost converter
- Drives motor

===========================
TEST 2: ESP Talks
===========================
- OTA Blink Sketch (FlightCodeRevA)
- Wireless 'arming' blink sketch
- ESP changes memory register based on WiFi signal
- Controlling from mobile device for assembly

===========================
TEST 3: Put it together
===========================
- SAMD21 running motor if told, etc.
- Records data to SD card the whole time (or if a different esp memory register is true)
- SAMD21 receives commands from ESP and can send things back (again, via memory registers)
- Low power mode for ESP?