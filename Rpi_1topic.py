import paho.mqtt.client as paho
import time


broker="192.168.1.103" #IP of your Raspberry Pi



#define callback

def on_message(client, userdata, message):
    
    sensor_values  =str(message.payload.decode("utf-8"))
    #print("received message =",str(message.payload.decode("utf-8")))
    f= open("data_log_deepSleep.txt","a")
    f.write(time.strftime('%Y-%m-%d,%H:%M:%S,'))
    f.write(sensor_values)
    f.write("\n")
    f.close()
    


client= paho.Client("client-001")

client.on_message=on_message



print("connecting to broker ",broker)

client.connect(broker)

print("subscribed")
print("Writing to log file")
client.subscribe("sensorREAD")

client.loop_forever()
