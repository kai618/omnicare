#include <Chrono.h>
#include <Servo.h>

//Interval
const unsigned int interval = 10000;

String nodeName = "FIRE";

// --------------------------------- Components

// pin input
#define button 2
#define servo 8
#define gasSen A5
#define flameSen A0

#define grnLed 6
#define bluLed 7
#define redLed 5
#define buzzer 4

Chrono alertGas;
bool alertStateGas = true;
unsigned long spacingGas = 300;

Chrono alertFire;
bool alertStateFire = true;
unsigned long spacingFire = 200;

// Threshold
const int gasLimit = 200;
const int flameLimit = 150;

// State
bool mainState = false;  //true = on, false = off
bool butState = false;  //true = pressed, false = unpressed
unsigned long pressTime = 0;
const unsigned long waitTime = 500;

// Servo

Servo sv;
int angle = 0;
int svStep = 1;

//Mode
String OffMode = "OFF";
String SafeMode = "SAFE";
String GasMode = "SMOKE";
String FireMode = "FIRE";

String nowMode = OffMode;

Chrono txTimer;

// --------------------------------- Main

void setup() {
  Serial.begin(9600);
  delay(100);

  setupServo();
  setupLEDpin();
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  sendSignal(nowMode);
}

void loop() {
  checkButton();
  if (mainState) setOnMode();
  else setOffMode();

  changeOutputMode();
  repeatSignal();
}

// --------------------------------- Input Sensors buttons Servo

void sendSignal(String mode) {  
  String signal = nodeName + " " + mode;
  Serial.print(signal);
  txTimer.restart();
}

void repeatSignal() {
  if(txTimer.hasPassed(interval)) sendSignal(nowMode);
  
}

void checkButton() {
  if (mainState == false) {
    if (isButtonPressed()) setSafeModeOutput();
  }
  else {
    if (isButtonHeld()) setOffModeOutput();
  }
}
bool isButtonPressed() {
  if (digitalRead(button) == LOW) butState = true;
  if (butState && digitalRead(button) == HIGH) {
    butState = false;
    return true;
  }
  return false;
}
bool isButtonHeld() {
  if (digitalRead(button) == HIGH) pressTime = 0;
  if (digitalRead(button) == LOW && pressTime == 0) pressTime = millis();
  if (pressTime != 0 && digitalRead(button) == LOW && millis() - pressTime >= waitTime) butState = true;
  if (butState && digitalRead(button) == HIGH) {
    butState = false;
    pressTime = 0;
    return true;
  }
  return false;
}

void setOnMode() {
  setDetectMode(chooseDetectMode());
}

void setOffMode() {
  if (nowMode != OffMode) setMode(OffMode);
  getHandBack();
}

void setDetectMode(int i) {
  switch (i) {
    case 1: setSafeMode(); break;
    case 2: setGasMode(); break;
    case 3: setFireMode(); break;
  }
}

int chooseDetectMode() {
  if (detectFlame()) return 3;
  else if (detectGas()) return 2;
  else return 1;
}

void setSafeMode() {
  if (nowMode != SafeMode) setMode(SafeMode);
  scoutArea(35);
}

void setGasMode() {
  if (nowMode != GasMode) setMode(GasMode);
  scoutArea(5);
}

void setFireMode() {
  if (nowMode != FireMode) setMode(FireMode);  
  delay(20);
}

void setMode(String mode) {
  nowMode = mode;
  sendSignal(mode);
}

void setupServo() {
  sv.attach(servo);
  angle = sv.read();
}

void scoutArea(int slowness) {
  angle += svStep;
  sv.write(angle);
  if (angle < 0 || angle > 180) svStep = -svStep;
  delay(slowness);
}

void getHandBack() {
  if (angle > 0) {
    sv.write(--angle);
    delay(20);
  }
}

bool detectGas() {
//   Serial.println(analogRead(gasSen));
//  if (analogRead(gasSen) > gasLimit) return true;
  return false;
}

bool detectFlame() {
  // Serial.println(analogRead(flameSen));
  if (analogRead(flameSen) < flameLimit) return true;
  return false;
}

// --------------------------------- Output LED Buzzer

void changeOutputMode() {
  if (nowMode == OffMode) setOffModeOutput();
  else if (nowMode == SafeMode) setSafeModeOutput();
  else if (nowMode == GasMode) setGasModeOutput();
  else if (nowMode == FireMode) setFireModeOutput();   
}
void setOffModeOutput() {
  setColor(0, 0, 0);
  if (mainState) {
    mainState = false;
    beepOff();
  }
}
void setSafeModeOutput() {
  setColor(0, 255, 0);
  if (!mainState) {
    mainState = true;
    beepOn();
  }
}
void setGasModeOutput() {
  delay(15);
  if (alertGas.hasPassed(spacingGas)) {
    alertGas.restart();
    switchAlertGas();
  }
}
void setFireModeOutput() {
  delay(15);
  if (alertFire.hasPassed(spacingFire)) {
    alertFire.restart();
    switchAlertFire();
  }
}
void switchAlertGas() {
  if (alertStateGas) setAlertONGas();
  else setAlertOFF();
  alertStateGas = !alertStateGas;
}
void switchAlertFire() {
  if (alertStateFire) setAlertONFire();
  else setAlertOFF();
  alertStateFire = !alertStateFire;
}
void setAlertONGas() {
  setColor(255, 255, 0);
  tone(buzzer, 500, spacingGas);
}
void setAlertONFire() {
  setColor(255, 0, 0);
  tone(buzzer, 1000, spacingFire);
}
void setAlertOFF() {
  setColor(0, 0, 0);
}
void beepOn() {
  tone(buzzer, 700, 100);
}
void beepOff() {
  tone(buzzer, 200, 500);
}
void setColor(int redVal, int grnVal, int bluVal) {
  analogWrite(redLed, redVal);
  analogWrite(grnLed, grnVal);
  analogWrite(bluLed, bluVal);
}
void setupLEDpin() {
  pinMode(redLed, OUTPUT);
  pinMode(grnLed, OUTPUT);
  pinMode(bluLed, OUTPUT);
}
