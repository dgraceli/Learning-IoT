// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   

// Raspberry Pi IP address, so it connects to the MQTT broker
const char* mqtt_server = "raspberrypi.local";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

int RELAY   = D1;
int ECHO    = D2;
int TRIGGER = D3;
float tankdata;
int min_thresh=5;
int water=0;
float max_height=18.00;
float height;

// This functions is executed when some device publishes a message to a topic that this ESP8266 is subscribed to
float ultrasonicRead()
{
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2);  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER, LOW);
  long duration = pulseIn(ECHO, HIGH);
  tankdata = (duration/2) / 29.09;
  height= max_height-tankdata;
  if (height<0)
    return 0;
  else 
    return height;
}

void callback(String topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  water=ultrasonicRead();
  delay(2000);
  static char levelTemp[7];
  dtostrf(water, 6, 2, levelTemp);
    
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
    if((messageTemp > "550") && (water > min_thresh))
      {
        Serial.print("Dry Soil!! Watering");
        digitalWrite(RELAY, HIGH);
        client.publish("/esp8266/ControlNode/relay", "1"); 
        water=ultrasonicRead();
        delay(2000);
        static char levelTemp[7];
        dtostrf(water, 6, 2,levelTemp);
        client.publish("/esp8266/ControlNode/waterLevel",levelTemp);
        Serial.println(" On");
        Serial.println(water);
        delay(5000);
      }
    else 
      {
        digitalWrite(RELAY, LOW);
        client.publish("/esp8266/ControlNode/relay", "0");
        water=ultrasonicRead();
        delay(2000);
        static char levelTemp[7];
        dtostrf(water, 6, 2, levelTemp);
        client.publish("/esp8266/ControlNode/waterLevel",levelTemp);
        Serial.print("Off");
        Serial.println(water);
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
      client.subscribe("/esp8266/+/moisture");
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
  pinMode(RELAY,OUTPUT);
  pinMode(TRIGGER,OUTPUT); 
  pinMode(ECHO,INPUT); 
  delay(1000);
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP_CN");
  Serial.println("connected...");
  //setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  if(!client.loop())
    client.connect("Client3");
  }
