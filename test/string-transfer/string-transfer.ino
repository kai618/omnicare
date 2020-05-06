
char* longStr = "This is a very long string";
char* shortStr = "1";

void setup() {
  Serial.begin(115200); 
}

void loop() {  
  Serial.println(shortStr);
  delay(1000);
  Serial.println(longStr);
  delay(1000);
}
