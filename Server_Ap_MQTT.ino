#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

const char *APssid = "Esp8266";
const char *APpassword = "8266";
const char* mqtt_server = "192.168.0.102";
const char* pub_topic = "drink/1602000001";
const char* sub_topic = "in/1602000001";

String Cssid="";
String Cpw="";
String all_ssid[15];
String all_RSSI[15];
int state = 0;

ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
int send_pub = 0;

void handleRoot() {
  String ap="",input ="";
  ap = "<table><tr><td>ssid</td><td>RSSI</td></tr>";
  for(int i=0 ;i<10; i++)
  {
    ap= ap +"<tr><td>"+all_ssid[i]+"</td><td>"+all_RSSI[i]+"</td></tr>";
             
  }
  ap=ap+"</table>";
  input = "<form action='send' method= 'get' >";
  input +="SSID: <input type='text', name='ssid' ><br>";
  input += "PW: <input type='text', name='pw' ><br>";
  input += "<input type='submit' name='submit' ,value='send'>";
  input += "</form>";
  server.send(200, "text/html", ap+input);
}
void Send()
{
  Cssid = server.arg("ssid");
  Cpw = server.arg("pw");
  Serial.println(Cssid);
  Serial.println(Cpw);
  String a = Cssid+"  "+Cpw;
  server.send(200, "text/html", a);
  delay(1000);
  state = 1;
 
}
void connectwifi()
{
  char bufssid[Cssid.length()+1];
  char bufpw[Cpw.length()+1];
  Cssid.toCharArray(bufssid,Cssid.length()+1);
  Cpw.toCharArray(bufpw,Cpw.length()+1);
  Serial.print("buf");
  Serial.println(bufssid);
  Serial.println(bufpw);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);
  Serial.println("STA mode");
  WiFi.begin(bufssid, bufpw); 
 // WiFi.begin("IMAC_Lab-2.4G" ,"imacuser");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
}
void Scan()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println("start");
  int n = WiFi.scanNetworks();
  if(n > 0)
  {
    Serial.println("there are "+String(n)+" APs");
    for(int i =0; i<n ;i++)
    {
      Serial.print(i+1);
      Serial.print(" : ");
      Serial.print(WiFi.SSID(i));
      all_ssid[i]= WiFi.SSID(i);
      Serial.print(" : ");
      Serial.print(WiFi.RSSI(i));
      all_RSSI[i] = WiFi.RSSI(i);
      Serial.print(" : ");
      
      Serial.println("");
      delay(10);
    }
  }
  else
  {
    Serial.print("there are no ap") ;
    delay(1000);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) 
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(pub_topic, "hello world");
      // ... and resubscribe
      client.subscribe(sub_topic);
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      Serial.println(mqtt_server);
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void set_mqtt()
{
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}


void callback(char* topic, byte* payload, unsigned int length) {
  String s = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    s = s+(char)payload[i];
  }
  Serial.println();
  
  if(s == "send")
  {
    send_pub = 1;  
  }
  

}

void setup() {
  delay(1000);
  Serial.begin(115200);

  Scan();
   
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(APssid, APpassword);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/send", Send);
  server.begin();
  Serial.println("HTTP server started");
 
  delay(100);

  Serial.println("OK");

}
int old_state = 10;
void loop() 
{
  
  if(state==0)
  {
    server.handleClient();  
    
  }
  else if(state ==1)
  {
    connectwifi();
    state =3;
    set_mqtt();
  }
  else if(state ==3 )
  {
     if (!client.connected()) 
    {
      reconnect();
    }
    client.loop();

    
    if (send_pub == 1) 
    {
      
      ++value;
      snprintf (msg, 75, "hello world #%ld", value);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish(outTopic, msg);  
    }  
    send_pub = 0;
  }
  
  if(state != old_state)
  {
    Serial.println(state);
    old_state = state;
  }

}
