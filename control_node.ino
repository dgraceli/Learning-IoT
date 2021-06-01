// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   

// Raspberry Pi IP address, so it connects to the MQTT broker
const char* mqtt_server = "192.168.1.108";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

const int relay = 5;

// This functions is executed when some device publishes a message to a topic that this ESP8266 is subscribed to
void callback(String topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  if(topic=="esp8266/+/moisture")
  {
    if(messageTemp > "600")
      {
        Serial.print("Dry Soil!! Watering");
        digitalWrite(relay, HIGH);
        client.publish("esp8266/ControlNode/relay", "ON"); 
        Serial.println(" On");
        delay(5000);
      }
    else 
      {
        digitalWrite(relay, LOW);
        client.publish("esp8266/ControlNode/relay", "OFF");
        Serial.print("Off");
      }
  }
  Serial.println();
  delay(1000);
}

// This functions reconnects your ESP8266 to your MQTT broker
void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Client3")) 
    {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      client.subscribe("/esp8266/sensorNode2/moisture");
      client.subscribe("/esp8266/sensorNode1/moisture");
    }
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() 
{
  pinMode(relay, OUTPUT);
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...");
  //setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if (!client.connected()) 
  {
    reconnect();
  }
  if(!client.loop())
    client.connect("Client3");
}

void loop() 
{
  }
