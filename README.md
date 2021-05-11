# ESP8266 --MQTT-- Rpi

In this project,a DHT22 and a Capacitive Soil Moisture sensor are interfaced to ESP8266 NodeMCU board, the ESP8266(Client) sends the sensor readings through MQTT once in 15 minutes(Using the DeepSleep mode to save Battery power) to Raspberry pi(Broker), the RPi stores the sensor data in text file.

