import paho.mqtt.client as mqtt

#Channel Topic
sensors = "Distance Sensor and Reed Switch"
LED = "LED"

#ip of localhost
mqtt_broker= "127.0.0.1"
mqtt_port = 1883

#Set up MQTT Client object (access to MQTT functions in the library) 
client = mqtt.Client()
print "MQTT client object is set up"

#Define functions

def msg_rcv(client, user_data, msg):	#Interpret Msgs (Loops)
	if (msg.payload == 1):
		LED_color = "On"
	elif (msg.payload == 0):
		LED_color = "Off"
	elif (msg.payload == "red" or msg.payload == "green" or msg.payload == "yellow")
		LED_color = msg.payload
	client.publish (LED, LED_color)	#(channel, msg to publish)

def run_broker(client, user_data, rc):			#Subscribe to topics (Once)
	client.subscribe(sensors)			#Listen to the Sensors channel
	print "Subscribed to Sensors Channel"

#When message is received, run msg_rcv function
client.on_message = msg_rcv

#When connected to Broker, run run_broker function
client.on_connect = run_broker

#Begin connection to MQTT Broker
client.connect(mqtt_broker,mqtt_port)
print "connection to broker started"

#Predefined functions
client.loop_forever()	#Client will keep itself alive
client.disconnect()	#Disconnect before dying (cntrl C or kill)
