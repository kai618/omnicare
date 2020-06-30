#include <AltSoftSerial.h>
#include <ArduinoQueue.h>

#include <SPI.h>
#include <RF24.h>

#include <ThreeWire.h>  
#include <RtcDS1302.h>

AltSoftSerial altSerial; // Rx-8 Tx-9
ArduinoQueue<String> dataQueue(50);

bool ableToSend = false;

String serialPin =  "0 ";
String radioPin =   "1 ";

// RF
RF24 radio(2, 3); // CE CSN
const byte address[6] = "kaiRF";

// RTC
ThreeWire myWire(5,4,6); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {  
  Serial.begin(115200);
  altSerial.begin(9600);
//  Serial.println("ArduinoRoute started");
  setupRTC();
  setupRF24();
}

void loop() {  
  checkPortSerial();
  checkPortRadio();
  checkSignalFromNodeMCU();
}

void setupRTC() {
  Rtc.Begin();  
  
//  Serial.println(Rtc.GetDateTime().Epoch32Time());  
//  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);  
//  Serial.println(compiled.Epoch32Time());  
//  Rtc.SetDateTime(compiled);  
//  Serial.println(Rtc.GetDateTime().Epoch32Time());     
}

String getEpochStr() {
  // - 25200 to get UTC+0
   return String(Rtc.GetDateTime().Epoch32Time() - 25140) + " ";
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
  
  data = serialPin + getEpochStr() + data;
//  Serial.println(data); // set baud rate of Serial Monitor to 115200
  if (dataQueue.isFull()) dataQueue.dequeue();
  dataQueue.enqueue(data);
}

void checkPortRadio() {
  if (radio.available()) {
    char sig[32] = "";
    radio.read(&sig, sizeof(sig));    
    String data = String(sig);
    
    data = radioPin + getEpochStr() + data;
//    Serial.println(data); // set baud rate of Serial Monitor to 115200
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
