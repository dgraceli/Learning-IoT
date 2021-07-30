
// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <DNSServer.h>          //required for WiFiManager.h
#include <ESP8266WebServer.h>   //required for WiFiManager.h
#include <WiFiManager.h>     
#include <ESP8266mDNS.h>

#define DHTTYPE DHT22   // DHT22  (AM2302), AM2321
const int UpdateInterval = 5 * 60 * 1000000;

//Raspberry Pi hostname.local
const char* mqtt_server = "raspberrypi.local";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

// DHT Sensor
const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

char data[80];

void setup() 
{
  dht.begin();
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...");
 // setup_wifi();
  client.setServer(mqtt_server, 1883);

  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("Client1")) 
    {
    Serial.println("connected");
    } 
    else 
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    float m = analogRead(A0);
    if (!isnan(h) && !isnan(t)) 
    {
      // Computes temperature values in Celsius
    float hic = dht.computeHeatIndex(t, h, false);
    static char temperatureTemp[7];
    dtostrf(hic, 6, 2, temperatureTemp);
  
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);

    static char moistureTemp[7];
    dtostrf(m, 6, 2, moistureTemp);
    
    client.publish("/esp8266/sensorNode1/temperature", temperatureTemp);
    client.publish("/esp8266/sensorNode1/humidity", humidityTemp);
    client.publish("/esp8266/sensorNode1/moisture", moistureTemp);
    while(m>550)
    {
      delay(3000);
      m = analogRead(A0);
      dtostrf(m, 6, 2, moistureTemp);
      client.publish("/esp8266/sensorNode1/moisture", moistureTemp);
      delay(2000);
    }
    Serial.println(String(t) + "," + String(h) + "," + String(m));
    }
    delay(5000);
    ESP.deepSleep(UpdateInterval,WAKE_RF_DEFAULT);
    
}
void loop() 
{
  
}
