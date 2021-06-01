
// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     

// DHT22
#define DHTTYPE DHT22  
const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

//15 minutes Deep Sleep in micro seconds
const int UpdateInterval = 15 * 60 * 1000000;

//Raspberry Pi IP address, so it connects to the MQTT broker
const char* mqtt_server = "192.168.1.108";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

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
