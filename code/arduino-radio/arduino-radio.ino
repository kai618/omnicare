#include <SPI.h>
#include <RF24.h>

RF24 radio(2, 3); // CE CNS
const byte address[6] = "kaiRF";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
 checkPortSerial();
}

void checkPortSerial() {
  if (!Serial.available()) return;
  
  String data = "";
  char c;
  while (Serial.available() > 0) {
    c = Serial.read();
    data += c;
    delay(5);
  }
  Serial.println(data);
  transmitRF(data);
}

void transmitRF(String data) {  
  int len = data.length() + 1;
  char arr[len];
  data.toCharArray(arr, len);
  radio.write(&arr, sizeof(arr));
}
