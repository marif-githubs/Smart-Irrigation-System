#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <string.h>
#include <DHT.h>
#include <dummy.h>
#include <EEPROM.h>

const char* ssid = "d link*";
const char* password = "9391039251";

#define DHTPIN D2      // DHT sensor connected to digital pin D2
#define DHTTYPE DHT11  // DHT sensor type

#define SOIL_MOISTURE_PIN A0  // Soil moisture sensor connected to analog pin A0
//#define LIGHT_SENSOR_PIN A1   // Light sensor connected to analog pin A1
#define RAIN_SENSOR_PIN D1  // Rain sensor connected to digital pin D1
#define WATERING_PIN D3     // Watering switch connected to digital pin D0

//int onHour ;
//int onMinute ;
//int Interval;
boolean flag;
int day;

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);

const char index_html[] = {"<!DOCTYPE html><html lang=\"en\"><head>    <meta charset=\"UTF-8\">    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">    <style>        body {            box-sizing: border-box;            background-image: URL(\"https://images.unsplash.com/photo-1601654253194-260e0b6984f9?ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D&auto=format&fit=crop&w=1699&q=80\");        }        .first-container {            box-sizing: border-box;            opacity: 1;            max-width: 600px;            min-width: fit-content;            margin: 0 auto;            margin-top: 25px;            padding-top: 5px;            background-color: #fff;            border-radius: 10px;            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);        }        .section1 {            margin: 20px;            margin-bottom: 35px;        }        h1 {            font-size: 2em;            text-align: center;        }        .section2 {            position: relative;        }        #date-time {            position: absolute;            margin-left: 20px;            top: -25px;        }        .watering-switch {            box-sizing: border-box;            background-color: #eeffe6;            border: groove rgb(76, 207, 251) 5px;            /* border-style: groove; */            height: 150px;            width: 150px;            border-radius: 100%;            margin: auto;            padding: 25px;            text-align: center;            font-size: 1.5em;            cursor: pointer;        }        .but {            position: absolute;            background-color: hsl(101, 100%, 95%);            border: groove rgb(76, 207, 251) 3px;            font-size: 1.14em;            cursor: pointer;            right:50px;            border-radius: 100%;        }                .box {            display: none;            position: fixed;            top: 50%;            left: 50%;            transform: translate(-50%, -50%);            background-color: #f9f9f9;            padding: 20px;            border-radius: 5px;            box-shadow: 0px 2px 8px rgba(0, 0, 0, 0.15);            z-index: 9999;        }        .overlay {            display: none;            position: fixed;            top: 0;            left: 0;            width: 100%;            height: 100%;            background-color: rgba(0, 0, 0, 0.5);            z-index: 9998;        }        .container {            width: 100%;        }        th div {            box-sizing: border-box;            width: 200px;            height: 100px;            padding-top: 1PX;            margin: 15px;            background-color: rgb(230, 247, 253);            border-radius: 25px;            margin-top: 30px;        }        th p {            margin-left: 35%;            margin-right: 35%;            font-size: 1.2em;            border-radius: 20%;        }        table {            margin-top: 15px;            margin-left: auto;            margin-right: auto;            border-collapse: collapse;        }        h3 {            margin-left: 30px;            padding-bottom: 20px;        }    </style>    <script>        function fun1() {            document.getElementById(\"over\").style.display = \"block\";            document.getElementById(\"con\").style.display = \"block\"        }        function fun2() {            document.getElementById(\"over\").style.display = \"none\";            document.getElementById(\"con\").style.display = \"none\"        }        var xhr1 = new XMLHttpRequest();        function sendData() {            var form = document.getElementById(\"sch_form\");            var formData = new FormData(form);            var queryString = new URLSearchParams(formData).toString();            xhr1.open(\"GET\", \"/submit?\" + queryString, true);            xhr1.send();        }        function manual() {            var form_interval = document.getElementById(\"sch_form\").elements[2].value;            if (form_interval == 0){                document.getElementById(\"button\").textContent = \"M\";            }else{                document.getElementById(\"button\").innerHTML = \"A\";            }        }        function scheduleLoader(){	        fetch('/S_loader')		    .then(response => response.json())		    .then(data => {		    	var onHour = data.onHour;                var onMinute = data.onMinute;                var Interval = data.Interval;		    	var inputElement1 = document.getElementById(\"hour\");		    	var inputElement2 = document.getElementById(\"minute\");		    	var inputElement3 = document.getElementById(\"interval\");		    	inputElement1.value = onHour;		    	inputElement2.value = onMinute;		    	inputElement3.value = Interval;		    })                    .catch(error => {                        console.error('Error fetching data:', error);                    });        }        var formattedDateTime;        var formattedTime;        function displayDateTime() {            var currentDate = new Date();            var day = currentDate.getDate();            var month = currentDate.getMonth() + 1;            var year = currentDate.getFullYear();            var hours = currentDate.getHours();            var minutes = currentDate.getMinutes();            var seconds = currentDate.getSeconds();            formattedTime = hours + ':' + minutes;            formattedDateTime = day + '/' + month + '/' + year + ' ' + hours + ':' + minutes + ':' + seconds;            document.getElementById('date-time').innerHTML = formattedDateTime;        }        var isOn = false;        var xhr = new XMLHttpRequest();        function toggle() {            isOn = !isOn;            var status = document.getElementById(\"status_text\");            var button = document.getElementById('watering_switch');            if (isOn) {                status.innerHTML = \"Watering is ON\";                button.style.backgroundColor = \"#bde0ff\";                xhr.open('GET', '/toggle_1', true);                xhr.send();            } else {                status.innerHTML = \"Watering is OFF\";                button.style.backgroundColor = \"#cbffb3\";                xhr.open('GET', '/toggle_0', true);                xhr.send();            }        }        function fetchData() {            fetch('/data')                .then(response => response.json())                .then(data => {                    var t_temperature = data.temperature;                    var h_humidity = data.humidity;                    var s_soilMoisture = data.soilMoisture;                    var lr_lastrain = data.rainSensorValue;                    t_temperature += \"C'\";                    h_humidity += \"%\";                    s_soilMoisture += \"%\";                    document.getElementById('temperature_').textContent = t_temperature;                    document.getElementById('humidity_').textContent = h_humidity;                    document.getElementById('soil_moisture').textContent = s_soilMoisture;                    if (lr_lastrain == \"0\") {                        document.getElementById('last_rain').textContent = formattedDateTime;                    }                })                .catch(error => {                    console.error('Error fetching data:', error);                });        }        document.addEventListener('DOMContentLoaded', () => {            fetchData();            scheduleLoader();        manual();        });        setInterval(displayDateTime, 1000);        setInterval(fetchData, 1000);      </script>    <title>Smart Watering System</title></head><body onload=\"displayDateTime()\">    <div class=\"first-container\">        <section class=\"section1\">            <h1>Smart Watering System</h1>        </section>        <section class=\"section2\">            <p id=\"date-time\"></p>            <div class=\"watering-switch\" id=\"watering_switch\" onclick=\"toggle()\">                <p class=\"status-text\" id=\"status_text\">Watering is OFF</p>            </div>            <button class=\"but\" id=\"button\" onclick=\"fun1()\">M</button>            <div class=\"overlay\" id=\"over\"></div>            <div class=\"box\" id=\"con\">                <h2>Schedule</h2>                <form action=\"\" id=\"sch_form\">                    <label for=\"hour\">Hour:   </label>                    <input type=\"text\" id=\"hour\" name=\"S_hour\" placeholder=\"'hh'\"/><br><br>                    <label for=\"minute\">Minutes:</label>                    <input type=\"text\" id=\"minute\" name=\"S_minute\" placeholder=\"'mm'\"/><br><br>                    <label for=\"interval\">Interval:</label>                    <input type=\"text\" id=\"interval\" name=\"S_interval\" placeholder=\"'mm'\"/><br><br>                </form>                <input type=\"button\" id=\"submitButton\" value=\"save\" onclick=\"fun2(); sendData(); manual();\">                <input type=\"button\" id=\"closeButton\" value=\"close\" onclick=\"fun2()\">            </div>        </section>        <section class=\"section3\">            <div class=\"container\">                <table>                    <tr>                        <th>                            <div class=\"g1\">                                <h2>Temperature</h2>                                <P class=\"temperature\" id   =\"temperature_\">-</P>                            </div>                        </th>                        <th>                            <div class=\"g1\" id=\"2\">                                <h2>Humodity</h2>                                <P class=\"humidity\" id=\"humidity_\">-</P>                            </div>                        </th>                    </tr>                    <tr>                        <th>                            <div>                                <h2>Soil moisture</h2>                                <P class=\"soil-moisture\" id=\"soil_moisture\">-</P>                            </div>                        </th>                        <th>                            <div class=\"g2\">                                <h2>UV ray</h2>                                <P class=\"uv-ray\" id=\"\">-</P>                            </div>                        </th>                    </tr>                </table>            </div>            <h3>LAST WATER OR RAIN ON <span class=\"last-rain\" id=\"last_rain\">-</span>            </h3>    </div>    </section></body></html>"};

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

  //Serial.println("temperature is"+String(temperature));
  //Serial.println("humidity is "+String(humidity)+"%");
  //Serial.println("soilMoisture is "+String(moisturePercentage)+"%");

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


void switch_on() {
  digitalWrite(WATERING_PIN, 0);
  Serial.println("watering switch is on");
}

void switch_off() {
  digitalWrite(WATERING_PIN, 1);
  Serial.println("watering switch is off");
}

void loadSchedule(){
  int addr = 0;
	int onHour=EEPROM.read(addr);
  addr += sizeof(onHour);
  int onMinute=EEPROM.read(addr);
  addr += sizeof(onMinute);
  int Interval=EEPROM.read(addr);
	Serial.println("loadScheduler is running");
	StaticJsonDocument<100> jsonTime;
	jsonTime["onHour"]=onHour;
	jsonTime["onMinute"]=onMinute;
	jsonTime["Interval"]=Interval;

	String jsonTimeData;
	serializeJson(jsonTime,jsonTimeData);

	server.send(200, "application/json",jsonTimeData);
}
	
void handleFormSubmit() {
  String S_hour = server.arg("S_hour");
  String S_minute = server.arg("S_minute");
  String S_interval = server.arg("S_interval");

  int onHour = S_hour.toInt();
  int onMinute = S_minute.toInt();
  int Interval = S_interval.toInt();
  flag = false;
  int addr = 0;
  //Serial.println(onHour);
  //Serial.println(onMinute);
  //Serial.println(Interval);
  // Do something with the data (e.g., store in EEPROM, send a response)
  EEPROM.write(addr,onHour);
  addr += sizeof(onHour);
  EEPROM.write(addr,onMinute);
  addr += sizeof(onMinute);
  EEPROM.write(addr,Interval);

  server.send(200, "text/plain", "Data received");
}

unsigned long minutesToMilliseconds(int minutes) {
  return (unsigned long)minutes * 60000;
}

 void schedule() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  int time = timeinfo->tm_hour;
  int minute = timeinfo->tm_min;
  int weekday = timeinfo->tm_wday;
  int addr = 0;
  int onHour=EEPROM.read(addr);
  addr += sizeof(onHour);
  int onMinute=EEPROM.read(addr);
  addr += sizeof(onMinute);
  int Interval=EEPROM.read(addr);
  //Serial.println(addr);
  //Serial.println(onHour);
  //Serial.println(addr);
  //Serial.println(onMinute); 
  //Serial.println(addr);
  //Serial.println(Interval);

  if(weekday =! day){
    flag = false;
    day = weekday;
  }
  if(!flag) {
    if (onHour >= 0) {
      if (time >= onHour && minute >= onMinute) {
        switch_on();
        delay(minutesToMilliseconds(Interval));
        switch_off();    
        flag = true;
      }
    }
  }
}

void wifi_Set() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, 1);
    delay(1000);
    Serial.println("Connecting to WiFi...");
    digitalWrite(LED_BUILTIN, 0);
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, 0);
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  // pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(WATERING_PIN, OUTPUT);
  digitalWrite(WATERING_PIN, 1);

  wifi_Set();


  // Register request handlers
  server.on("/", handleRoot);
  server.on("/data", serveData);
  server.on("/toggle_1", switch_on);
  server.on("/toggle_0", switch_off);
  server.on("/submit", HTTP_GET, handleFormSubmit);
  server.on("/S_loader",loadSchedule);
  // Register more handlers for other files

  configTime(5.5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }

  // Start the server
  server.begin();
  Serial.println("Server started");
}


void loop() {
  server.handleClient();
  
  schedule();
  

}
