import requests 
import time
import paho.mqtt.client as mqtt
server ="0.0.0.0" 
port = 1883
topic ="drink/#"
url = "http://211.23.17.100:5278/api/createinfo"
def onConnect(client ,userdata ,rc):
    client.subscribe([(topic ,1)])

def onMessage(client ,userdata ,message):
    print("Topic: "+ message.topic +"Message: "+ message.payload)
    t = message.topic
    token = t.split("/")
    value = {"PH" : 6,
             "TDS": 1,
             "Filter1" : 1,
             "Filter2" : 2,
             "Filter3" : 3,
             "Filter4" : 4,
             "leak" : 0,
             "flow" : 0,
             "token" : token[1]
             }
    r = requests.post(url ,data = value)
    print r
while True:
    try:
        client = mqtt.Client( client_id = "" ,clean_session = True ,userdata = None ,protocol = "MQTTv31")
        client.on_connect = onConnect
        client.on_message = onMessage
        client.connect(server ,port ,keepalive = 60 ,bind_address = "")
        client.loop_forever()
    except Exception ,e:
        print e
        time.sleep(5)


