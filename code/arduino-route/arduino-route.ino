#include <AltSoftSerial.h>
#include <ArduinoQueue.h>

AltSoftSerial altSerial; // Rx-8 Tx-9
ArduinoQueue<String> dataQueue(100);

bool ableToSend = false;

void setup() {
  Serial.begin(115200);
  altSerial.begin(9600);
}

void loop() {  
  checkPortSerial();
  checkPortRadio();
  checkSignalFromNodeMCU();
}

void checkPortSerial() {
  if (!altSerial.available()) return;
  
  String data = "";
  char c;
  while (altSerial.available() > 0) {
    c = altSerial.read();
    data += c;
    delay(1);  
  }
  dataQueue.enqueue(data);
}

void checkPortRadio() {
  // TODO
}

void checkSignalFromNodeMCU(){
  if (!Serial.available()) return;

  char c = Serial.read();
  delay(1);
  if (c == '1') ableToSend = true;
  
  //remove other bytes to avoid bugs (if any)
  while(Serial.available()) {
    Serial.read(); 
    delay(1);
  }
  
  if (dataQueue.isEmpty()) return;
  if (ableToSend) {
    Serial.print(dataQueue.dequeue());
    ableToSend = false;
  }  
}
