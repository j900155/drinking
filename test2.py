import paho.mqtt.client as mqtt
import time
#import paho.mqtt.publish as publish
username="user"
password="pw"
vhost="test"
server="192.168.0.102"
port=1883
topic="outTopic"

def on_Message(client, userdata, message):
    print("Received message '" + str(message.payload) + "' on topic '"+ message.topic + "' with QoS " + str(message.qos))

def onConnect(client ,userdata ,rc):
        client.subscribe([(topic,1)])

while True:
    try:
        client=mqtt.Client(client_id="", clean_session=True, userdata=None, protocol="MQTTv31")
        client.username_pw_set(vhost + ":" + username,password)
        #client.message.payload()
        #client.message.qos()
        client.on_connect = onConnect
        client.on_message=on_Message
        client.connect(server, port, keepalive=59, bind_address="")
        client.loop_forever()
        #time.sleep(1)
    except Exception,e:
        print e
        time.sleep(1)
