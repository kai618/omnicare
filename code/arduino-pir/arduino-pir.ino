#include <Chrono.h>

//Interval
const unsigned int interval = 10000;

String nodeName = "LIGHT";

// --------------------------------- Components

String OnMode = "TRUE";
String OffMode = "FALSE";
String nowMode = OffMode;

Chrono txTimer;

int ledPin = 2;
int inputPin = 3;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  sendSignal(nowMode);
}

void loop() {
  observeMotion();
  repeatSignal();
}

void sendSignal(String mode) {  
  String signal = nodeName + " " + mode;
  Serial.print(signal);
  Serial.flush();
  txTimer.restart();
  delay(50);
}

void repeatSignal() {
  if(txTimer.hasPassed(interval)) sendSignal(nowMode);
}

void observeMotion() {
  if (digitalRead(inputPin) == HIGH && nowMode != OnMode) {
    digitalWrite(ledPin, HIGH);
    nowMode = OnMode;
    sendSignal(nowMode);
  } else if (digitalRead(inputPin) == LOW && nowMode != OffMode) {
    digitalWrite(ledPin, LOW);
    nowMode = OffMode;
    sendSignal(nowMode);
  }
}
