#include <Chrono.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h" 
#include <Wire.h> 

String nodeName = "TEMP_HUMID";
const char delimiter = '_';

// DTH11
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temp = 0.0;
float humid = 0.0;

// LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Timer
Chrono timer;
const unsigned int interval = 10000; 


void setup() {
  Serial.begin(9600);
  setUpLCD();
  dht.begin();


  processData();
}

void loop() {
  repeat();
}

void setUpLCD() {
  Wire.begin(); // sda-A4 scl-A5
  lcd.begin();
  lcd.home();

  lcd.setCursor(1, 0);
  lcd.print("Temp :       C");
  lcd.setCursor(13, 0);
  lcd.print(char(223));
  lcd.setCursor(1, 1);
  lcd.print("Humid:      %");
}

void repeat() {
  if (!timer.hasPassed(interval)) return;
  processData();
  timer.restart();
}

void processData() {
  getTempHumid();
  displayLCD();
  sendSignal();
}

void getTempHumid() {
  temp = dht.readTemperature();
  humid = dht.readHumidity();
}

void displayLCD() {
  lcd.setCursor(8, 0);
  lcd.print(temp);
  lcd.setCursor(8, 1);
  lcd.print(humid);
}

void sendSignal() {
  String signal = nodeName + " " + String(temp) + delimiter + String(humid);
  Serial.print(signal);
  Serial.flush();
}
