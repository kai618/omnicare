

void setup() {
  Serial.begin(115200); 
}

void loop() {    
  Serial.println(readSerial());
  delay(1000);
}

String readSerial() {
  String str = "";
  while(Serial.available()) {
    str += (char) Serial.read();
    delay(1);
  }
  return str;
}
