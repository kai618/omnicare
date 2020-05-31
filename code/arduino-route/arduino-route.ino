#include <AltSoftSerial.h>
#include <ArduinoQueue.h>

AltSoftSerial altSerial; // Rx-8 Tx-9
ArduinoQueue<String> dataQueue(50);

bool ableToSend = false;

String serialPin =  "0 ";
String radioPin =   "1 ";

void setup() {  
  Serial.begin(115200);
  altSerial.begin(9600);
//  Serial.println("ArduinoRoute started");
}

void loop() {  
  checkPortSerial();
  checkPortRadio();
  checkSignalFromNodeMCU();
}

int a = 0;
void checkPortSerial() {
  if (!altSerial.available()) return;
  
  String data = "";
  char c;
  while (altSerial.available() > 0) {
    c = altSerial.read();
    data += c;
    delay(5);
  }
  if (dataQueue.isFull()) dataQueue.dequeue();
  data = serialPin + data;
  dataQueue.enqueue(data);

////  for testing only
//  if (dataQueue.isFull()) dataQueue.dequeue();
//  dataQueue.enqueue(String(a++));
//  delay(2000);
}

void checkPortRadio() {
  // TODO
}

void checkSignalFromNodeMCU(){
  if (Serial.available()) {
    ableToSend = true;
    //remove other bytes (if any) to avoid bugs
    while(Serial.available()) {
      Serial.read();
      delay(5);
    }
  }
  
  if (dataQueue.isEmpty()) return;
  if (ableToSend) {
    Serial.print(dataQueue.dequeue());
    Serial.flush();
    ableToSend = false;
  }  
}
