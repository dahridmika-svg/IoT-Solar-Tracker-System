#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Dialog 4G";
const char* password = "J9RD6BM8E38";

WebServer server(80);

String direction = "CENTER";
String angle = "90";
String statusText = "Balanced";

void handleRoot() {

  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>

<meta http-equiv="refresh" content="1">
<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

body{
  margin:0;
  padding:20px;
  font-family:Arial,sans-serif;
  background:linear-gradient(135deg,#0f172a,#1e293b);
  color:white;
  text-align:center;
}

.title{
  font-size:32px;
  font-weight:bold;
  margin-top:20px;
  margin-bottom:25px;
}

.card{
  background:#1e293b;
  border-radius:20px;
  padding:20px;
  margin:15px auto;
  width:320px;
  box-shadow:0px 0px 15px rgba(0,255,255,0.25);
}

.label{
  font-size:16px;
  color:#94a3b8;
}

.value{
  font-size:30px;
  font-weight:bold;
  margin-top:10px;
}

.online{
  color:#22c55e;
}

.tracking{
  color:#38bdf8;
}

.footer{
  margin-top:25px;
  color:#94a3b8;
}

</style>

</head>

<body>

<div class="title">
IoT Solar Tracker
</div>

<div class="card">
<div class="label">Direction</div>
<div class="value">
)rawliteral";

  page += direction;

  page += R"rawliteral(
</div>
</div>

<div class="card">
<div class="label">Servo Angle</div>
<div class="value">
)rawliteral";

  page += angle;

  page += R"rawliteral(
°
</div>
</div>

<div class="card">
<div class="label">Wi-Fi Status</div>
<div class="value online">
ONLINE
</div>
</div>

<div class="card">
<div class="label">System Status</div>
<div class="value tracking">
)rawliteral";

  page += statusText;

  page += R"rawliteral(
</div>
</div>

<div class="footer">
Arduino UNO + ESP32 Web Dashboard
</div>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", page);
}

void setup() {

  Serial.begin(115200);

  // RX = GPIO17, TX = GPIO16
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {

  server.handleClient();

  if (Serial2.available()) {

    String msg = Serial2.readStringUntil('\n');
    msg.trim();

    Serial.print("Received: ");
    Serial.println(msg);

    int commaIndex = msg.indexOf(',');

    if (commaIndex > 0) {

      direction = msg.substring(0, commaIndex);
      angle = msg.substring(commaIndex + 1);

      if (direction == "CENTER")
        statusText = "Balanced";

      else if (direction == "LEFT")
        statusText = "Tracking Left";

      else if (direction == "RIGHT")
        statusText = "Tracking Right";
    }
  }
}