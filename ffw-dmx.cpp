#include <Arduino.h>
#include <WiFi.h>
#include <ESPDMX.h>
#include <WebServer.h>

//Definitions
void ClientResponseCheck();


/*
* Area with DMX Functions
*/

//DMX definition
DMXESPSerial dmx;

void DMXControl(int fogtime, int fogstrenght)
{
  Serial.println("DMX Control");
  Serial.print("Staerke: ");
  Serial.println(fogstrenght);
  Serial.print("Zeit: ");
  Serial.println(fogtime);
}
//----------------------------------------------------------------

/*
* Area with WIFI Functions
*/

// WiFi Network
const char * NetworkName = "ssid";
const char * NetworkPwd = "pw";

// Wifi Accesspoint name and Password:
const char * APName ="FFW-FOG";
const char * APPwd ="87654321";


//Function WIFI Connect
void SetUpAccessPoint(const char * ssid, const char * pwd){
  Serial.println("Setting AP (Access Point).... ");

  //Setting SSID and Password AP
  WiFi.softAP(ssid, pwd);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP adress: ");
  Serial.println(IP);
}
//----------------------------------------------------------------

/*
* Area with HTML Function
*/
//Set Web Server Port Number to 80
WebServer server(80);

//Definition for HTML Site
String startHTML();

//HTML Server Handling
void handle_HTMLRoot(){
  server.send(200,"text/html",startHTML());
}

//Handle if Website not found
void handle_NotFound(){
  String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

  server.send ( 404, "text/plain", message );
}

// General Actions for Website Calls called in Setup fuction
void htmlserver(){
  server.on("/",handle_HTMLRoot);
  server.on("/werte",ClientResponseCheck);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTML Server Started");
}

// Handling HTML Get Reply for Fogmaschine Control Website
void ClientResponseCheck(){
  Serial.println("ResponseCHECK");
  int zeit = 0;
  int staerke = 0;
  if (server.method() == HTTP_GET){
    Serial.println("ResponseCheck IF = OK");
    for (uint8_t i = 0; i< server.args(); i++){
      if(server.argName( i ) =="staerke"){
        String text = server.arg( i );
        staerke = text.toInt();
        Serial.print("Staerke:____");
        Serial.println(staerke);
      }
      if (server.argName( i ) =="zeit"){
        String text =server.arg( i );
        zeit = text.toInt();
        Serial.print("Zeit:___");
        Serial.println(zeit);
      }
    } 
    DMXControl(zeit,staerke);
    server.send(200,"text(html",startHTML());
  }
  else{
    server.send(404, "text/plain", "Somthing went wrong!!!");
  }
}

//-----------------------------------------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dmx.init();
  delay(50);

  //WIFI
  SetUpAccessPoint(APName, APPwd);

  //HTML Server
htmlserver();
}

void loop(){
  server.handleClient();
}

String startHTML(){
  String htmlptr ="<!DOCTYPE html><html>\n";
  htmlptr += "<head> <meta charset=\"UTF-8\">\n";
  htmlptr += "<title>Nebelsteuerung Feuerwehr Siegertsbrunn </title>\n";
  // --> CSS Style if needed put here
  htmlptr += "</head>\n";
  htmlptr += "<body>\n"; 
  htmlptr += "<h1>Feuerwehr Siegetsbrunn</h1>\n";
  htmlptr += "<h3>Nebelmaschinensteuerung</h3>\n";
  htmlptr += "<div class=\"nebelcontainer\">\n";

  // --> Form for Values
  htmlptr += "<h4>Nebel St&auml;rke (mindestens 5)</h4>\n";
  htmlptr += "<form action=\"werte\" formmethod=\"get\">\n";
  htmlptr += "<input type=\"range\" min=\"5\" max=\"100\" value=\"5\" id=\"nebelstaerkeregler\">\n";
  htmlptr += "<input type=\"text\" name=\"staerke\" id=\"nebelstaerkenanzeige\" value=\"0\">\n";
  htmlptr += "<input type=\"range\" min=\"1\" max=\"30\" value=\"0\" id=\"nebelzeitregler\">\n";
  htmlptr += "<input type=\"text\" name=\"zeit\" id=\"nebelzeitanzeige\" value=\"0\">Minuten\n";
  htmlptr += "<input type=\"submit\" value=\"START\">\n";
  htmlptr += "</form> </div>\n";
  // --> JAVA SCRIPT
  htmlptr += "<script>var nebelstaerke=document.getElementById(\"nebelstaerkeregler\");";
  htmlptr += "var nebelanzeige=document.getElementById(\"nebelstaerkenanzeige\");\n";
  htmlptr += "nebelanzeige.value=nebelstaerke.value;\n";
  htmlptr += "nebelstaerke.oninput=function(){\n";
  htmlptr += "nebelanzeige.value=this.value;};\n";
  htmlptr += "var nebelzeit=document.getElementById(\"nebelzeitregler\");\n";
  htmlptr += "var zeitanzeige=document.getElementById(\"nebelzeitanzeige\");\n";
  htmlptr += "zeitanzeige.value=nebelzeitregler.value;\n";
  htmlptr += "nebelzeit.oninput=function(){\n";
  htmlptr += "zeit=nebelzeit.value; zeitanzeige.value=zeit;};\n";
  htmlptr += "</script>\n";
  htmlptr += "</body></html>";
  return htmlptr;
}
