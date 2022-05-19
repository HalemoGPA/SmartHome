#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here
ESP8266WebServer server(80);
#define DHT11_PIN D8
#define FLAME_PIN D7
#define GAS_PIN A0
#define BUZZER_PIN D6
float Temperature=0;
float Humidity=0;
bool FlameReading=1;
int GasReading=0;
bool buzzerState=0;

void setup() {
  Serial.begin(115200);
  pinMode(D6,OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  delay(100);
  IPAddress ser_ip=WiFi.softAPIP();
  Serial.println(ser_ip);
  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.on("/buzzeroff",handle_off);
  server.begin();
  Serial.println("HTTP server started");
  pinMode(FLAME_PIN,INPUT);
  pinMode(GAS_PIN,INPUT);
}






void loop() {
  server.handleClient();
  if(buzzerState==HIGH)
  {
    if(FlameReading == LOW || GasReading > 800)
    {
      digitalWrite(BUZZER_PIN,HIGH);
    }
//    else 
//    {
//      digitalWrite(BUZZER_PIN,LOW);
//    }
  }
  else 
  {
    digitalWrite(BUZZER_PIN,LOW);
  }
  if (Humidity  > 65)
  {
    digitalWrite(D5,HIGH);
  }
  else {
    digitalWrite(D5,LOW);
  } 


}



void handle_OnConnect() {
 buzzerState=HIGH;
 DHT.read(DHT11_PIN);
 Temperature = DHT.temperature; // Gets the values of the temperature
 Humidity = DHT.humidity; // Gets the values of the humidity 
 FlameReading = digitalRead(FLAME_PIN); //Gets values of flame sensor readings
 GasReading =analogRead(GAS_PIN);
 
 server.send(200, "text/html", SendHTML(Temperature,Humidity,FlameReading,GasReading,buzzerState)); 
}

void handle_off(){
 buzzerState=LOW;
 DHT.read(DHT11_PIN);
 Temperature = DHT.temperature; // Gets the values of the temperature
 Humidity = DHT.humidity; // Gets the values of the humidity 
 FlameReading = digitalRead(FLAME_PIN); //Gets values of flame sensor readings
 GasReading =analogRead(GAS_PIN);
 
 server.send(200, "text/html", SendHTML(Temperature,Humidity,FlameReading,GasReading,buzzerState));
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}



String SendHTML(float TempCstat,float Humiditystat,bool flamestat,int gasstat,bool buzzerstate){
  String ptr = "<!DOCTYPE html> <html>"
  "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">"
//  "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">"
//  ptr+="<meta http-equiv=\"refresh\" content=\"1\" >"
  "<title>Baka Project</title>"
  "<script>"
  "setInterval(loadDoc,100);"
  "function loadDoc() {"
  "var xhttp = new XMLHttpRequest();"
  "xhttp.onreadystatechange = function() {"
  "if (this.readyState == 4 && this.status == 200) {"
  "document.getElementById(\"webpage\").innerHTML =this.responseText}"
  "};";
  if(buzzerstate==HIGH)
  {
    ptr+="xhttp.open(\"GET\", \"/\", true);"
         "xhttp.send();";
  }
  else 
  {
    ptr+="xhttp.open(\"GET\", \"/buzzeroff\", true);"
         "xhttp.send();";
  }
  
  ptr+="}"
  "</script>"
  "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}"
  "body{margin-top: 50px;}"
  "h1 {margin: 50px auto 30px;}"
  ".side-by-side{display: inline-block;vertical-align: middle;position: relative;}"
  ".humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;}"
  ".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;color:white;}"
  ".humidity{font-weight: 300;font-size: 60px;color: #3498db;}"
  ".temperature-icon{background-color: #ff9c00;width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}"
  ".temperature-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;color:white;}"
  ".temperature{font-weight: 300;font-size: 60px;color: #ff9c00;}"
  ".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}"
  ".data{padding: 10px;}"
  ".centralize{margin-left: 15px;}"
  "p {color: rgb(255, 255, 255);margin: 30px auto 35px;}"
  "a {display: block;width: 80px;height: 30px;border-radius: 20px;border: none;color: white;font-size: 18px;text-decoration: none;margin: 10px auto;text-align: center;cursor: pointer;position: absolute;top: 1px;left: 9px;}"
  ".button-off {background-color: #AAA;}\n"
  ".button-off:active {background-color: #222;}\n"
  ".button-on:active {background-color: #ffb700;}\n"
  ".button-on {background-color: #ffda00;}\n"
  ".button {width: 100px;border: none;color: white;padding: 20px 30px;font-size: 25px;margin: 30px 10px 10px;cursor: pointer;border-radius: 20px;}"
  "p {font-size: 18px;margin-bottom: 10px;}\n"
  ".gas{display: block;margin: 5px 70px;}"
  ".gas-exist {display: block;margin: 5px 48px;}"
  ".flame-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;text-indent: 30px;padding-top:10px;}"
  ".svg-itself{display:block;margin-top:30px;}"
  ".container-svg{background-color:#ffda00;border:2px solid #404040;border-radius:50px;padding:10px;width:200px;margin:auto;margin-top:20px; }"
  ".big-gpa{display: flex;justify-content: center;align-items: center;padding:80px;}"
  ".fired{margin-bottom:-30px;cursor:not-allowed;filter: drop-shadow(2px 2px 5px #ff0000) drop-shadow(-2px -2px 5px #ff8c00) brightness(1.5) drop-shadow(2px -2px 5px #e91313) drop-shadow(5px 2px 10px #ff5900);}"
  "hr {background-color: white;width: 300px;margin: 20px auto;}"
  "html{background-color:#2f3b40}"
  "h1{color:white;padding: 20px;margin:auto;border-radius: 5px;}"
  ".gpa{color:white;margin-top:20px;background-color:#333;border-radius:20px;padding:10px;font-family: Arial, 'Times New Roman', Times, serif;font-size: small;}"
  "</style>"
  "</head>"
  "<body>"
  
   "<div id=\"webpage\">"
   
   "<h1>Baka Project</h1>"
   "<hr>"
   "<div class=\"data\">"
   "<div class=\"side-by-side temperature-icon\">"
   "<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\""
   "width=\"9.915px\" height=\"22px\" viewBox=\"0 0 9.915 22\" enable-background=\"new 0 0 9.915 22\" xml:space=\"preserve\">"
   "<path fill=\"#FFFFFF\" d=\"M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142"
   "c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491"
   "c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463"
   "c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394"
   "c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z\"/>"
   "</svg>"
   "</div>"
   "<div class=\"side-by-side temperature-text\">Temperature</div>"
   "<div class=\"side-by-side temperature\">";
   ptr+=(int)TempCstat;
   ptr+="<span class=\"superscript\">°C</span></div>"
   "</div>"
   "<div class=\"data hu\">"
   "<div class=\"side-by-side humidity-icon\">"
   "<svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\"; width=\"12px\" height=\"17.955px\" viewBox=\"0 0 13 17.955\" enable-background=\"new 0 0 13 17.955\" xml:space=\"preserve\">"
   "<path fill=\"#FFFFFF\" d=\"M1.819,6.217C3.139,4.064,6.5,0,6.5,0s3.363,4.064,4.681,6.217c1.793,2.926,2.133,5.05,1.571,7.057"
   "c-0.438,1.574-2.264,4.681-6.252,4.681c-3.988,0-5.813-3.107-6.252-4.681C-0.313,11.267,0.026,9.143,1.819,6.217\"></path>"
   "</svg>"
   /*
        <svg width="300" height="300" viewBox="0 0 300 300">
    
   
    <g class="flames">
      <path d="M101.138,160.949 C94.916,154.464 53.538,110.17 95.277,71.802 C130.054,39.868 135.137,13.003 123.434,-0 C123.434,-0 211.959,33.692 159.877,111.877 C150.998,125.163 128.702,140.843 140.369,173.129 L101.138,160.949 z" fill="#000000"/>
      <path d="M155.503,171.572 C153.624,165.019 145.142,150.746 171.021,122.303 C184.873,107.172 190.104,84.742 191.308,76.301 C191.308,76.301 237.167,100.662 191.576,160.215 L155.503,171.572 z" fill="#000000"/>
    </g>
   
    <g class="logs">
      <path d="M240.344,255.473 L240.344,216.874 L59.378,160.915 L59.378,199.513 z" fill="#000000"/>
      <path d="M165.259,180.707 L240.321,157.488 L240.321,196.087 L227.627,199.99 z" fill="#000000"/>
      <path d="M134.519,235.715 L59.419,258.9 L59.419,220.301 L72.151,216.433 z" fill="#000000"/>
    </g>
   */
   "</div>"
   "<div class=\"side-by-side humidity-text\">Humidity</div>"
   "<div class=\"side-by-side humidity\">";
   ptr+=(int)Humiditystat;
   ptr+="<span class=\"superscript\">%</span></div>"
   "</div>"
   "<hr>"
   "<div class=\"container-svg\">"
    "<div class=\"side-by-side flame-text\">Check Fire</div>";
    if (flamestat){
        ptr+="<div class=\"svg-itself\"><svg width=\"70\" height=\"70\" viewBox=\"0 0 300 300\">"
        "<path d=\"M101.138,160.949 C94.916,154.464 53.538,110.17 95.277,71.802 C130.054,39.868 135.137,13.003 123.434,-0 C123.434,-0 211.959,33.692 159.877,111.877 C150.998,125.163 128.702,140.843 140.369,173.129 L101.138,160.949 z\" fill=\"#000000\"/>"
        "<path d=\"M155.503,171.572 C153.624,165.019 145.142,150.746 171.021,122.303 C184.873,107.172 190.104,84.742 191.308,76.301 C191.308,76.301 237.167,100.662 191.576,160.215 L155.503,171.572 z\" fill=\"#000000\"/>"
        "</svg></div></div>";
    }else {
      ptr+="<div class=\"svg-itself fired\"><svg width=\"120\" height=\"120\" viewBox=\"0 0 300 300\">"
        "<path d=\"M101.138,160.949 C94.916,154.464 53.538,110.17 95.277,71.802 C130.054,39.868 135.137,13.003 123.434,-0 C123.434,-0 211.959,33.692 159.877,111.877 C150.998,125.163 128.702,140.843 140.369,173.129 L101.138,160.949 z\" fill=\"#FF0000\"/>"
        "<path d=\"M155.503,171.572 C153.624,165.019 145.142,150.746 171.021,122.303 C184.873,107.172 190.104,84.742 191.308,76.301 C191.308,76.301 237.167,100.662 191.576,160.215 L155.503,171.572 z\" fill=\"#FF0000\"/>"
        "</svg></div></div>";
    }
    if (gasstat<400){
      ptr+="<div class=\"container-svg\">"
                "<div class=\"side-by-side flame-text\">Check Gas</div>"
                "<svg class=\"gas\" xmlns=\"http://www.w3.org/2000/svg\" width=\"70\" height=\"70\" viewBox=\"0 0 48 48\">"
                    "<path fill=\"none\" d=\"M0 0h48v48h-48z\" />"
                    "<path d=\"M39.54 14.46l.03-.03-7.45-7.43-2.12 2.12 4.22 4.22c-1.88.72-3.22 2.53-3.22 4.66 0 2.76 2.24 5 5 5 .71 0 1.39-.15 2-.42v14.42c0 1.1-.9 2-2 2s-2-.9-2-2v-9c0-2.21-1.79-4-4-4h-2v-14c0-2.21-1.79-4-4-4h-12c-2.21 0-4 1.79-4 4v32h20v-15h3v10c0 2.76 2.24 5 5 5s5-2.24 5-5v-19c0-1.38-.56-2.63-1.46-3.54zm-15.54 5.54h-12v-10h12v10zm12 0c-1.1 0-2-.9-2-2s.9-2 2-2 2 .9 2 2-.9 2-2 2z\" />"
                "</svg>"
            "</div>";
    }
    else {
      ptr+="<div class=\"container-svg\">"
                "<div class=\"side-by-side flame-text\">Check Gas</div>"
                "<svg class=\"gas-exist\" xmlns=\"http://www.w3.org/2000/svg\" width=\"120\" height=\"120\" viewBox=\"0 0 48 48\">"
                    "<path fill=\"none\" d=\"M0 0h48v48h-48z\"></path>"
                    "<path d=\"M39.54 14.46l.03-.03-7.45-7.43-2.12 2.12 4.22 4.22c-1.88.72-3.22 2.53-3.22 4.66 0 2.76 2.24 5 5 5 .71 0 1.39-.15 2-.42v14.42c0 1.1-.9 2-2 2s-2-.9-2-2v-9c0-2.21-1.79-4-4-4h-2v-14c0-2.21-1.79-4-4-4h-12c-2.21 0-4 1.79-4 4v32h20v-15h3v10c0 2.76 2.24 5 5 5s5-2.24 5-5v-19c0-1.38-.56-2.63-1.46-3.54zm-15.54 5.54h-12v-10h12v10zm12 0c-1.1 0-2-.9-2-2s.9-2 2-2 2 .9 2 2-.9 2-2 2z\" fill=\"#34a853\" class=\"color000000 svgShape\"></path>"
                "</svg>"
            "</div>";
    }
  if (buzzerstate)
  {
    ptr+="<div class=\"centralize\"><p class=\"side-by-side\">Buzzer Status : On</p><button class=\"button button-off side-by-side\"><a href=\"/buzzeroff\">Turn OFF</a></button></div>";
         
  }
  else 
  {
    ptr+="<div class=\"centralize\"><p class=\"side-by-side\">Buzzer Status : OFF</p><button class=\"button button-on side-by-side\" ><a href=\"/\">Turn On</a></button></div>";
  }
  ptr+="<hr><div class=\"big-gpa\">"
   "<div class=\"gpa\">Made by Halemo GPA</div>"
   "</div>"
   "</div>"
   "</body>"
   "</html>";
  return ptr;
}








/*
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 8

void setup(){
  Serial.begin(115200);
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);
}

void loop(){
  DHT.read(DHT11_PIN);
  Serial.print("temp:");
  Serial.print(DHT.temperature);
  Serial.print("  humi:");
  Serial.println(DHT.humidity);
  delay(1000);
}*/
