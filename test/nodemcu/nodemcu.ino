#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Chrono.h>

// ---------------------------------- Wifi
String retrieverSSID = "Sam";
String retrieverPw = "";
String ssidUrl = "http://192.168.43.1:8080/ssid";
String pwUrl = "http://192.168.43.1:8080/password";

String ssid = "GHC_";
String password = "12356789";

// ---------------------------------- HTTPS API
WiFiClientSecure sClient; // to create HTTPS requests
String host = "omnicare-a0c59.an.r.appspot.com";
const int port = 443;
String createModuleUrl = "/api/v1/module/create/";
String getTokenUrl = "/api/v1/module/token/";
String updateDataUrl = "/api/v1/module/";
String token = "";

// ---------------------------------- Serial
Chrono timer;
const unsigned int interval = 10000;


void setup() {
  Serial.begin(115200);
  connectWifi(ssid, password);
  lightTheSun();

  getTokenFromFlash();
  if (token == "") createModuleRequest();
  persistTokenToFlash();
    
  sendYesBackToArduinoRoute();
}


void loop() {
  // check to get a new token
  checkSignalFromArduinoRoute();
}

void checkSignalFromArduinoRoute() {
  if (!Serial.available()) {
    if (timer.hasPassed(interval)) sendYesBackToArduinoRoute();    
    return;
  }
  String data = "";
  char c;
  while (Serial.available() > 0) {
    c = Serial.read();
    data += c;
    delay(5); // needs this to work, I don't know why :(
  }
  updateDataRequest(data);
}


String createModuleRequest() {
  String url =  createModuleUrl + WiFi.macAddress();
  while (token == "") {
    token = sendHttpsGet(sClient, host, port, url);
    delay(2000);
  }
}

void getNewTokenRequest() {
  String url = getTokenUrl + WiFi.macAddress();
  token = sendHttpsGet(sClient, host, port, url);
}

void updateDataRequest(String data) {
  String url = updateDataUrl + WiFi.macAddress();
  String fullData = token + " " + data;

  Serial.println(fullData);
  lightTheMoon();
  bool result = sendHttpsPost(sClient, host, port, url, fullData);
  shadowTheMoon();  
  if(result) sendYesBackToArduinoRoute();
}

void sendYesBackToArduinoRoute() {
  Serial.print('y');
  timer.restart();
}

String sendHttpsGet(WiFiClientSecure httpsClient, String host, int port, String url) {  
  if (httpsClient.connect(host.c_str(), port)) {         
    httpsClient.println("GET " + url + " HTTP/1.1");
    httpsClient.println("Host: " + host);
    httpsClient.println("User-Agent: ESP8266/Hieu");
    httpsClient.println("Connection: close\r\n");   
//    Serial.println("--Sent GET request to " + host);

    String response = httpsClient.readString();
    int bodypos = response.indexOf("\r\n\r\n") + 4;
//    Serial.println("--GET request ended successfully");
    return response.substring(bodypos);
  }
  else {
//    Serial.println("--Connection failed!!!");
    return "";
  }
}

bool sendHttpsPost(WiFiClientSecure httpsClient, String host, int port, String url, String data) {
  if (httpsClient.connect(host, port)) {    
    httpsClient.println("POST " + url + " HTTP/1.1");
    httpsClient.println("Host: " + host);
    httpsClient.println("User-Agent: ESP8266/Hieu");
    httpsClient.println("Connection: close");
    httpsClient.println("Content-Type: text/plain; charset=UTF-8");
    httpsClient.print("Content-Length: ");
    httpsClient.println(data.length());
    httpsClient.println();
    httpsClient.println(data);
    delay(10);    
    return true;
    
//    Serial.println("--Sent POST request to " + host);
    
//    String response = httpsClient.readString();
//    int bodypos =  response.indexOf("\r\n\r\n") + 4;
    
//    Serial.println("--POST request ended successfully");
//    return response.substring(bodypos);
  }
  else { 
    return false;
    
//    Serial.println("--Connection failed!!!");
//    return "ERROR";    
  }
}

void connectWifi(String ssid, String pass) {
  WiFi.disconnect(true);
  WiFi.setAutoConnect(false);
  WiFi.setPhyMode(WIFI_PHY_MODE_11G);
  
//  Serial.println("--WiFi - connecting to " + ssid); 
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.print(".");
  }
//  Serial.println("");
//  Serial.print("--WiFi connected. IP address: ");
//  Serial.println(WiFi.localIP());
//  Serial.println("");
}
  
void checkFingerprintMatch(WiFiClientSecure httpsClient, String host, int port, String fingerprint) {
  httpsClient.connect(host, port);
  Serial.println("--Checking Certificate...");
  if (httpsClient.verify(fingerprint.c_str(), host.c_str())) {
    Serial.println("--Certificate matches");
  } else {
    Serial.println("--Certificate does not match!!!");
  }
}

// ------------- Token Read/Write
void getTokenFromFlash() {
  // My Flash storage has been broken since THCNTT2
}

void persistTokenToFlash() {
  // My Flash storage has been broken since THCNTT2
}


// ------------- LED indicators
void lightTheSun() {
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
}

void shadowTheSun() {
  digitalWrite(D0, HIGH);
}

void lightTheMoon() {
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);
}

void shadowTheMoon() {
  digitalWrite(D4, HIGH);
}
