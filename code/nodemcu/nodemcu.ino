String url = "domain.com/set/";

void setup() {
  Serial.begin(115200);
}


void loop() {
  // check to get a new token
  // check serial, read serial, send back confirm signal
  checkSignalFromArduinoRoute();
}

void checkSignalFromArduinoRoute() {
  String data = "";
  char c;
  while (Serial.available() > 0) {
    c = Serial.read();
    data += c;
    delay(1); // needs this to work, I don't know why :(
    if (Serial.available() > 0) continue;
    sendRequest(data);
  }
}

void sendRequest(String data) {
  Serial.println(url + data);
}
