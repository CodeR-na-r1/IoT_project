#include <ESP8266WiFi.h>

const char* ssid = "BASE"; //“your_wifi_ssid”
const char* password = "00000000"; //"your_wifi_password"
uint8_t retries=0;

const char* ap_ssid = "ESP8266_AP"; //Access Point SSID
const char* ap_password= "12345678"; //Access Point Password
uint8_t max_connections=8;//Maximum Connection Limit for AP
int current_stations=0, new_stations=0;
 
void setup()
{
  Serial.begin(115200);
  // wifi connect

  //Try and Connect to the Network
  WiFi.begin(ssid,password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
 
  //Wait for WiFi to connect for a maximum timeout of 20 seconds
  while(WiFi.status()!=WL_CONNECTED && retries<20)
  {
    Serial.print(".");
    retries++;
    delay(1000);
  }
 
  Serial.println();
  //Inform the user whether the timeout has occured, or the ESP8266 is connected to the internet
  if(retries==20)//Timeout has occured
  {
    Serial.print("Unable to Connect to ");
    Serial.println(ssid);
  }
   
  if(WiFi.status()==WL_CONNECTED)//WiFi has succesfully Connected
  {
    Serial.print("Successfully connected to ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }

  // AP
  //Start the serial communication channel
  Serial.println();
   
  //Setting the AP Mode with SSID, Password, channel, hidden and Max Connection Limit
  if(WiFi.softAP(ap_ssid,ap_password,1,false,max_connections)==true)
  {
    Serial.print("Access Point is Creadted with SSID: ");
    Serial.println(ap_ssid);
    Serial.print("Max Connections Allowed: ");
    Serial.println(max_connections);
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("Unable to Create Access Point");
  }
}
 
void loop()
{
  //WiFiClient client; // connect there to server (fastApi) -> https://techtutorialsx.com/2018/05/17/esp32-arduino-sending-data-with-socket-client/

  //Continuously check how many stations are connected to Soft AP and notify whenever a new station is connected or disconnected
 
  new_stations=WiFi.softAPgetStationNum();
   
  if(current_stations<new_stations)//Device is Connected
  {
    current_stations=new_stations;
    Serial.print("New Device Connected to SoftAP... Total Connections: ");
    Serial.println(current_stations);
  }
   
  if(current_stations>new_stations)//Device is Disconnected
  {
    current_stations=new_stations;
    Serial.print("Device disconnected from SoftAP... Total Connections: ");
    Serial.println(current_stations);
  }
   
}