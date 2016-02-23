#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const char *ssid = "Esp8266";
const char *password = "8266";

void Scan()
{
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
      Serial.print(" : ");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" : ");
      Serial.print( (WiFi.encryptionType(i)== ENC_TYPE_NONE)? "" : "*");
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

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Scan();
  delay(100);

  Serial.println("set");

}

void loop() {
  

}
