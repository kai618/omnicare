#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

String ssid = "GHC_";
String password = "12356789";

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  
  connectWifi(ssid, password);

  WiFiClientSecure client;  
  const int port = 443;
  String host = "api.github.com";
  String url = "/zen";
  String fingerprint = "59 74 61 88 13 CA 12 34 15 4D 11 0A C1 7F E6 67 07 69 42 F5";
  checkFingerprintMatch(client, host, port, fingerprint);
  Serial.println(sendHttpsGet(client, host, port, url));

  String host2 = "heroku-nodejs-api-test.herokuapp.com";
  String url2 = "/arduino";
  String fingerprint2 = "08 3B 71 72 02 43 6E CA ED 42 86 93 BA 7E DF 81 C4 BC 62 30";
  checkFingerprintMatch(client, host2, port, fingerprint2);
  Serial.println(sendHttpsPost(client, host2, port, url2, "Kai"));
}

void loop() {

}

String sendHttpsGet(WiFiClientSecure httpsClient, String host, int port, String url) {  
  if (httpsClient.connect(host.c_str(), port)) {         
    httpsClient.println("GET " + url + " HTTP/1.1");
    httpsClient.println("Host: " + host);
    httpsClient.println("User-Agent: ESP8266/Hieu");
    httpsClient.println("Connection: close\r\n");    
    Serial.println("--Sent GET request to " + host);

    String response = httpsClient.readString();
    int bodypos = response.indexOf("\r\n\r\n") + 4;
    Serial.println("--GET request ended successfully");
    return response.substring(bodypos);
  }
  else {
    Serial.println("--Connection failed!!!");
    return "ERROR";
  }
}

String sendHttpsPost(WiFiClientSecure httpsClient, String host, int port, String url, String data) {
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
    Serial.println("--Sent POST request to " + host);
    
    String response = httpsClient.readString();
    int bodypos =  response.indexOf("\r\n\r\n") + 4;
    
    Serial.println("--POST request ended successfully");
    return response.substring(bodypos);
  }
  else {
    Serial.println("--Connection failed!!!");
    return "ERROR";    
  }
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

void connectWifi(String ssid, String pass) {
  Serial.println("--WiFi - connecting to " + ssid); 
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  
  Serial.println("");
  Serial.print("--WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}
