
#include <AltSoftSerial.h>

AltSoftSerial altSerial;

String str;

void setup() {
  Serial.begin(9600);
  Serial.println("Start Arduino Route");
  altSerial.begin(9600);
}

void loop() {
  
  while (altSerial.available() > 0) {
    char c = altSerial.read();
    str += c;    
  }
  while (Serial.available() > 0) {
    char c = Serial.read();
    str += c;    
  }
  Serial.print(str);
  str = "";
}
