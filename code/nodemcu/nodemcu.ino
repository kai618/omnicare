String url = "domain.com/set/";

void setup() {
  Serial.begin(115200);
  
  lightTheSun();
  delay(3000);
  shadowTheSun();
  Serial.println("Start NodeMCU");
  sendYesBackToArduinoRoute();
}


void loop() {
  // check to get a new token
  // check serial, read serial, send back confirm signal
  checkSignalFromArduinoRoute();
}

void checkSignalFromArduinoRoute() {
  if (!Serial.available()) return;
  String data = "";
  char c;
  while (Serial.available() > 0) {
    c = Serial.read();
    data += c;
    delay(5); // needs this to work, I don't know why :(
  }
  sendRequest(data);
}

void sendRequest(String data) {
  lightTheSun();
  delay(500);
  shadowTheSun();
  sendYesBackToArduinoRoute();
}

void sendYesBackToArduinoRoute() {
  Serial.print('y');
}

void lightTheSun() {
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
}

void shadowTheSun() {
  digitalWrite(D0, HIGH);
}
