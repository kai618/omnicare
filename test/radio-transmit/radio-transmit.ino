#include <SPI.h>
#include <RF24.h>

RF24 radio(2, 3); // CE CNS
const byte address[6] = "kaiRF";

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  delay(1000);
}
