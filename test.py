import paho.mqtt.client as mqtt

user="user"
password="pw"
vhost="192.168.0.102"
port=1883
topic="sub_topic"

client=mqtt.Client(client_id="", clean_session=True, userdata=None, protocol="MQTTv31" )
#client.username_pw_set(user,password)
client.connect(vhost, port, keepalive=60)
client.publish(topic, payload="send", qos=1, retain=False)
client.disconnect()

