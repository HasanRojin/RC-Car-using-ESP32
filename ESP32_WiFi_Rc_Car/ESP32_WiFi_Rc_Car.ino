#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "Barcode_Car";
const char* password = "12345678";

WebServer server(80);

// Motor pins
const int IN1 = 27, IN2 = 26; // Left motor direction
const int IN3 = 25, IN4 = 33; // Right motor direction
const int ENA = 14; // Left motor speed (PWM)
const int ENB = 12; // Right motor speed (PWM)

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  WiFi.softAP(ssid, password);
  Serial.println("WiFi Started. Connect to: Barcode_Car");

  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html><html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        body {
          font-family: sans-serif;
          margin: 0;
          padding: 0;
          background: #d7f9d7;
        }
        header {
          background-color: #2ecc71;
          color: white;
          padding: 15px;
          text-align: center;
          font-size: 22px;
          font-weight: bold;
          box-shadow: 0 2px 5px rgba(0,0,0,0.2);
        }
        .container {
          display: flex;
          height: calc(100vh - 60px);
          justify-content: space-between;
          align-items: center;
          padding: 20px;
        }
        .vertical-slider {
          writing-mode: bt-lr;
          -webkit-appearance: slider-vertical;
          width: 50px;
          height: 300px;
          background-color: #eafbe7;
        }
        .horizontal-slider {
          width: 300px;
          height: 40px;
          background-color: #eafbe7;
        }
        .label {
          font-size: 16px;
          text-align: center;
          margin-bottom: 10px;
          color: #2c3e50;
        }
        .left, .right {
          display: flex;
          flex-direction: column;
          align-items: center;
        }
      </style>
    </head>
    <body>
      <header>Barcode Car controller</header>
      <div class="container">
        <div class="left">
          <div class="label">Forward / Backward</div>
          <input type="range" min="-255" max="255" value="0" class="vertical-slider" id="fbSlider">
        </div>
        <div class="right">
          <div class="label">Left / Right</div>
          <input type="range" min="-255" max="255" value="0" class="horizontal-slider" id="lrSlider">
        </div>
      </div>

      <script>
        let fb = 0, lr = 0;
        setInterval(() => {
          fb = document.getElementById("fbSlider").value;
          lr = document.getElementById("lrSlider").value;
          fetch(`/control?fb=${fb}&lr=${lr}`);
        }, 150);
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleControl() {
  int fb = server.arg("fb").toInt(); // Forward/Backward
  int lr = server.arg("lr").toInt(); // Left/Right

  int leftSpeed = constrain(fb - lr, -255, 255);
  int rightSpeed = constrain(fb + lr, -255, 255);

  setMotor(leftSpeed, rightSpeed);
  server.send(200, "text/plain", "OK");
}

void setMotor(int left, int right) {
  // Left motor direction
  if (left > 0) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  } else if (left < 0) {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  }

  // Right motor direction
  if (right > 0) {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  } else if (right < 0) {
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  }

  analogWrite(ENA, abs(left));
  analogWrite(ENB, abs(right));
}
