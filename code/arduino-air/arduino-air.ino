void setup() {
  Serial.begin(9600);
}
int i = 10;
String str = "Arduino Air ";

void loop() {
  i++;
  String data = str + String(i);
  Serial.println(data);
  Serial.flush();
  delay(2000);
}
