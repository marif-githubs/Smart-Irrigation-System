#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* ssid = "d link*";
const char* password = "9391039251";

#define DHTPIN D2    // DHT sensor connected to digital pin D2
#define DHTTYPE DHT11   // DHT sensor type

#define SOIL_MOISTURE_PIN A0   // Soil moisture sensor connected to analog pin A0
//#define LIGHT_SENSOR_PIN A1   // Light sensor connected to analog pin A1
#define RAIN_SENSOR_PIN D1   // Rain sensor connected to digital pin D1
#define WATERING_PIN D3   // Watering switch connected to digital pin D0

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);

const char index_html[] ={"<!DOCTYPE html><html lang=\"en\"><head>    <meta charset=\"UTF-8\">    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style>        body{    box-sizing: border-box;    background-image: URL(\"sws1.jpg\");}.first-container{    box-sizing: border-box;    opacity: 1;    max-width: 600px;    margin: 0 auto;       margin-top: 25px;    padding-top:5px ;    background-color: #fff;    border-radius: 10px;    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);}.section1{    margin: 20px ;}h1{    font-size: 2em ;    text-align: center;}#date-time{    position: absolute;    margin-left: 20px;    border: dashed gray ;}.watering-switch{     box-sizing: border-box;    background-color: #eeffe6;    border: dotted rgb(155, 210, 228) 5px;    border-style: groove;    height: 150px;    width: 150px;    border-radius: 100%;    margin: auto;    padding: 25px;    text-align: center;    font-size: 1.5em;}.container{    width: 100%;}th div{    box-sizing: border-box;    width: 200px ;    height: 100px;    padding-top:1PX ;    margin:15px;}th p{    text-decoration: underline;    background-color: aliceblue;    margin-left: 35%;    margin-right: 35%;    font-size: 1.2em;    border-radius: 20%;}table{    margin-left: auto;    margin-right: auto;    border-collapse: collapse;    border-color: black;}h3{margin-left: 30px;padding-bottom: 20px;}</style><script>    function displayDateTime() {        var currentDate = new Date();                var day = currentDate.getDate();        var month = currentDate.getMonth() + 1;         var year = currentDate.getFullYear();                var hours = currentDate.getHours();        var minutes = currentDate.getMinutes();        var seconds = currentDate.getSeconds();                var formattedDateTime = day + '/' + month + '/' + year + ' ' + hours + ':' + minutes + ':' + seconds;                document.getElementById('date-time').innerHTML = formattedDateTime;      }       var isOn = false;    var xhr = new XMLHttpRequest();    function toggle() {        isOn = !isOn;        var status = document.getElementById(\"status_text\");        var button = document.getElementById('watering_switch');        if (isOn) {        status.innerHTML = \"Watering is ON\";        button.style.backgroundColor =  \"aliceblue\";        xhr.open('GET', '/toggle_1', true);        xhr.send();    } else {        status.innerHTML = \"Watering is OFF\";        button.style.backgroundColor = \" #eeffe6\";        xhr.open('GET', '/toggle_0', true);        xhr.send();    }     }      function fetchData() {       fetch('/data')          .then(response => response.json())          .then(data => {                        var t_temperature = data.temperature;            var h_humidity = data.humidity;            var s_soilMoisture = data.soilMoisture;            var lr_lastrain = data.rainSensorValue;            t_temperature += \"C'\";            h_humidity += \"%\";            s_soilMoisture += \"%\";            document.getElementById('temperature_').textContent = t_temperature;            document.getElementById('humidity_').textContent = h_humidity;            document.getElementById('soil_moisture').textContent = s_soilMoisture;            document.getElementById('last_rain').textContent = lr_lastrain;          })          .catch(error => {            console.error('Error fetching data:', error);          });       }              document.addEventListener('DOMContentLoaded', () => {        fetchData();      });          setInterval(displayDateTime, 1000);      setInterval(fetchData, 1000);          document.getElementById('watering_switch').addEventListener('change', toggle);</script><title>Smart Watering System</title></head><body onload=\"displayDateTime()\">    <div class=\"first-container\">    <section class=\"section1\">        <h1>Smart Watering System</h1>    </section>    <section class=\"section2\">        <p id=\"date-time\"></p>        <div class=\"watering-switch\" id=\"watering_switch\" onclick=\"toggle()\">            <p class=\"status-text\" id=\"status_text\">Watering is OFF</p>        </div>    </section>    <section class=\"section3\">        <div class=\"container\">    <table>        <tr><th><div class=\"g1\">            <h2>Temperature</h2>            <P class=\"temperature\" id=\"temperature_\">-</P>        </div></th>        <th><div class=\"g1\" id=\"2\">            <h2>Humodity</h2>            <P class=\"humidity\" id=\"humidity_\">-</P>        </div></th></tr>        <tr><th><div>            <h2>Soil moisture</h2>            <P class=\"soil-moisture\" id=\"soil_moisture\">-</P>        </div></th>        <th><div class=\"g2\">            <h2>UV ray</h2>            <P class=\"uv-ray\" id=\"\">-</P>        </div></th></tr>    </table>    </div class=\"container\">      <h3>LAST WATER OR RAIN ON <span class=\"last-rain\" id=\"last_rain\">-</span>     </h3>    </div>     </section></body></html>"};

void handleRoot() {
  server.send(200, "text/html", index_html);
}





void serveData() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  //int lightIntensity = analogRead(LIGHT_SENSOR_PIN);
  int rainSensorValue = digitalRead(RAIN_SENSOR_PIN);
  
  int moisturePercentage = map(soilMoisture, 0, 1023, 100, 0);

  Serial.println("temperature is"+String(temperature));
  Serial.println("humidity is "+String(humidity)+"%");
  Serial.println("soilMoisture is "+String(moisturePercentage)+"%");
 
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  jsonDoc["soilMoisture"] = moisturePercentage;
  jsonDoc["rainSensorValue"] = rainSensorValue;
  // Serialize the JSON object to a string
  String jsonData;
  serializeJson(jsonDoc, jsonData);

  // Set the response headers
  server.sendHeader("Content-Type", "application/json");
  server.sendHeader("Access-Control-Allow-Origin", "*");

  // Send the sensor data as the response
  server.send(200, "application/json", jsonData);
}
   

void switch_on(){
  digitalWrite(WATERING_PIN,0);
  Serial.println("watering switch is on");
  //server.send(200, "text/plain", );
}

void switch_off(){
  digitalWrite(WATERING_PIN,1);
  Serial.println("watering switch is off");
}

void setup() {
  // Previous setup code...()
  Serial.begin(1200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

   dht.begin();
  pinMode(SOIL_MOISTURE_PIN, INPUT);
 // pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(WATERING_PIN, OUTPUT);
  digitalWrite(WATERING_PIN,1);

  // Register request handlers
  server.on("/", handleRoot);
 
  server.on("/data", serveData);
  server.on("/toggle_1",switch_on);
  server.on("/toggle_0",switch_off);
  // Register more handlers for other files

  // Start the server
  server.begin();
  Serial.println("Server started");
}


void loop() {
  server.handleClient();
}