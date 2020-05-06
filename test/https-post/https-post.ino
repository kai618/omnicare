#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

String ssid = "GHC_";
String password = "12356789";

String host = "api.github.com";
const int httpsPort = 443;

// SHA1 fingerprint of the certificate
const char* fingerprint = "59 74 61 88 13 CA 12 34 15 4D 11 0A C1 7F E6 67 07 69 42 F5";

void setup() {
  Serial.begin(115200);
  
  connectWifi(ssid, password);

  WiFiClientSecure client;
  client.connect(host, httpsPort);
  checkFingerprintMatch(client, host);
 
  String url = "/zen";  
  Serial.println(sendHttpsGetRequest(client, host, url));
}

void loop() {

}

String sendHttpsGetRequest(WiFiClientSecure httpsClient, String host, String url) {  
  if (httpsClient.connect(host.c_str(), 443)) {   
    Serial.println("--Sent GET request to " + host); 
    
    httpsClient.println("GET " + url + " HTTP/1.1");
    httpsClient.println("Host: " + host);
    httpsClient.println("User-Agent: ESP8266/Hieu");
    httpsClient.println("Connection: close\r\n");    
    
    String response = httpsClient.readString();
    int bodypos = response.indexOf("\r\n\r\n") + 4;
    
    Serial.println("--Connection ended successfully");
    return response.substring(bodypos);
  }
  else {
    Serial.println("--Connection failed!!!");
    return "Error";
  }
}

//String httpsPost(String url, String data) {
//  if (client.connect(host, 443)) {
//    client.println("POST " + url + " HTTP/1.1");
//    client.println("Host: " + (String)host);
//    client.println("User-Agent: ESP8266/1.0");
//    client.println("Connection: close");
//    client.println("Content-Type: application/x-www-form-urlencoded;");
//    client.print("Content-Length: ");
//    client.println(data.length());
//    client.println();
//    client.println(data);
//    delay(10);
//    
//    String response = client.readString();
//    int bodypos =  response.indexOf("\r\n\r\n") + 4;
//    return response.substring(bodypos);
//  }
//  else {
//    return "ERROR";
//  }
//}

void checkFingerprintMatch(WiFiClientSecure httpsClient, String host) {
  Serial.println("--Checking Certificate...");
  if (httpsClient.verify(fingerprint, host.c_str())) {
    Serial.println("--Certificate matches");
  } else {
    Serial.println("--Certificate does not match!!!");
  }
}

void connectWifi(String ssid, String pass) {
  Serial.println("WiFi - connecting to " + ssid); 
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("WiFi connected. ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}
