#include <Arduino.h>
#include <WiFi.h>
#include <ESPDMX.h>

void HTMLResponse(WiFiClient);
// WiFi Network
const char * NetworkName = "ssid";
const char * NetworkPwd = "pw";

// Wifi Accesspoint name and Password:
const char * APName ="FFW-FOG";
const char * APPwd ="87654321";

//DMX
DMXESPSerial dmx;

//Variable to store the State of the Button
String Port26Status = "off";
String Port27Status = "off";

//Variable for output Pins
const int output1 = 1;
const int output2 = 2;

//Set Web Server Port Number to 80
WiFiServer server(80);


//Function WIFI Connect
void SetUpAccessPoint(const char * ssid, const char * pwd){
  Serial.println("Setting AP (Access Point).... ");

  //Setting SSID and Password AP
  WiFi.softAP(ssid, pwd,7,0,4);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP adress: ");
  Serial.println(IP);

  server.begin();
}

String ClientResponse(WiFiClient client){
  String header=""; 
  //String for incomming Data
  String currentLine="";
  Serial.println();
  while(client.connected()){
    if(client.available()){
      char c = client.read();
      Serial.write(c);
      currentLine= currentLine+c;
      if(c =='\n' && currentLine ==''){
          HTMLResponse(client);
      }
      else{
        return currentLine;
      }
    }
  }
}

void ClientResponseCheck(String text){
  Serial.println(text);
  char staerke = 5;
  char dauer =5;
  dmx.write(1,staerke); 
  delay(dauer);
  dmx.write(1,0);
}

void HTMLResponse(WiFiClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  client.print("<!DOCTYPE html><html> <head> <meta charset=\"UTF-8\"> <title>Nebelsteuerung Feuerwehr Siegertsbrunn </title> </head> <body> <h1>Feuerwehr Siegetsbrunn</h1> <h3>Nebelmaschinensteuerung</h3> <div class=\"nebelcontainer\"> <form action=\"/\" method=\"get\"> <h4>Nebel St&auml;rke (mindestens 5)</h4> <input type=\"range\" min=\"5\" max=\"100\" value=\"5\" id=\"nebelstaerkeregler\"> <input type=\"text\" id=\"nebelstaerkenanzeige\" value=\"0\"> <input type=\"range\" min=\"1\" max=\"30\" value=\"0\" id=\"nebelzeitregler\"> <input type=\"text\" id=\"nebelzeitanzeige\" value=\"0\">Minuten <input type=\"submit\" value=\"START\"> </form> </div><script>var nebelstaerke=document.getElementById(\"nebelstaerkeregler\"); var nebelanzeige=document.getElementById(\"nebelstaerkenanzeige\"); nebelanzeige.value=nebelstaerke.value; nebelstaerke.oninput=function(){nebelanzeige.value=this.value;};var nebelzeit=document.getElementById(\"nebelzeitregler\"); var zeitanzeige=document.getElementById(\"nebelzeitanzeige\"); zeitanzeige.value=nebelzeitregler.value; nebelzeit.oninput=function(){zeit=nebelzeit.value; zeitanzeige.value=zeit;};</script> </body></html>");

  //The HTTP response ends with another blank line:
  client.println();
  client.stop();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dmx.init();
  delay(50);

  //WIFI
  SetUpAccessPoint(APName, APPwd);
}

void loop(){
  // put your main code here, to run repeatedly:
  //WifiClient client[4];

  WiFiClient client = server.available(); 
  String Header;

  if (client){
    Serial.println("New Client.");
    Header = ClientResponse(client);
    ClientResponseCheck(Header);
    //Serial.println(ClientResponse(client));
    Header="";
  }
}
