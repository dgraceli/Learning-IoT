
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <DHT.h>
#include <PubSubClient.h>
 
#define DHTPIN 5 // Digital pin connected to the DHT sensor GPIO 5 = D1
 
#define DHTTYPE DHT22 // DHT 22 

const int UpdateInterval = 15 * 60 * 1000000;// Deep Sleep is microseconds so for 15 minutes
DHT dht(DHTPIN, DHTTYPE);

 
unsigned long previousMillis = 0;
unsigned long interval = 10000;
 
const char* mqttServer = "192.168.X.X"; //IP Address of RPi
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
 //This code runs once after each Deep Sleep
  Serial.begin(115200);
 //Connecting to the available network without hardcoding the ssid and password
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");

  Serial.println("connected...");
  dht.begin();
  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client")) {
 
      Serial.println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }

  float t = dht.readTemperature();
    float h = dht.readHumidity();
    float m = analogRead(A0);
    if(!isnan(h)&&!isnan(t)){
      Serial.println(String(t) + "," + String(h) + "," + String(m));      
      String toSend = String(t) + "," + String(h) + "," + String(m);
      client.publish("sensorREAD",toSend.c_str());
      Serial.println("Sent data to mqtt");
      }
      delay(5000);
      ESP.deepSleep(UpdateInterval,WAKE_RF_DEFAULT);
      delay(2000);
      
}
void loop() {

}
