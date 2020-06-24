#include <AltSoftSerial.h>
#include <ArduinoQueue.h>

#include <SPI.h>
#include <RF24.h>

AltSoftSerial altSerial; // Rx-8 Tx-9
ArduinoQueue<String> dataQueue(50);

bool ableToSend = false;

String serialPin =  "0 ";
String radioPin =   "1 ";

// RF
RF24 radio(2, 3); // CE CSN
const byte address[6] = "kaiRF";

void setup() {  
  Serial.begin(115200);
  altSerial.begin(9600);
//  Serial.println("ArduinoRoute started");

  setupRF24();
}

void loop() {  
  checkPortSerial();
  checkPortRadio();
  checkSignalFromNodeMCU();
}

void setupRF24() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void checkPortSerial() {
  if (!altSerial.available()) return;
  
  String data = "";
  char c;
  while (altSerial.available() > 0) {
    c = altSerial.read();
    data += c;
    delay(5);
  }
  
  data = serialPin + data;
  if (dataQueue.isFull()) dataQueue.dequeue();
  dataQueue.enqueue(data);
}

void checkPortRadio() {
  if (radio.available()) {
    char sig[32] = "";
    radio.read(&sig, sizeof(sig));    
    String data = String(sig);
    data = radioPin + data;
    
    if (dataQueue.isFull()) dataQueue.dequeue();
    dataQueue.enqueue(data);
  }
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
