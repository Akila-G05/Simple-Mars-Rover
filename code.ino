#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


//---------------------- Create Acces point & WEB Server -------------------------
// Create Access point
const char* ssid = "ESP32_AP";
const char* password = "12345678";  // at least 8 chars
WebServer server(80); // Create WebServer on port 80

// HTML code (use your existing HTML)
const char* webpage = R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
      <style>
          * {
              box-sizing: border-box;
              margin: 0;
              padding: 0;
          }
          
          body {
              font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
              background: linear-gradient(135deg, #1a2a6c, #b21f1f, #fdbb2d);
              color: #333;
              line-height: 1.6;
              padding: 20px;
              min-height: 100vh;
          }
          
          .container {
              max-width: 1200px;
              margin: 0 auto;
              background: rgba(255, 255, 255, 0.95);
              border-radius: 20px;
              box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
              overflow: hidden;
          }
          
          header {
              text-align: center;
              padding: 25px 20px;
              background: #2196F3;
              color: white;
          }
          
          h1 {
              font-size: 2.5rem;
              margin-bottom: 10px;
              text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.3);
          }
          
          .subtitle {
              font-size: 1.2rem;
              opacity: 0.9;
          }
          
          .video-container {
              padding: 20px;
              text-align: center;
              background: #000;
          }
          
          #video {
              width: 100%;
              max-width: 640px;
              border: 4px solid #2196F3;
              border-radius: 12px;
          }
          
          .control-panels {
              display: flex;
              flex-direction: column;
              padding: 20px;
              gap: 30px;
          }
          
          @media (min-width: 992px) {
              .control-panels {
                  flex-direction: row;
              }
              
              .car-controls, .arm-controls {
                  flex: 1;
              }
          }
          
          .panel {
              background: white;
              border-radius: 15px;
              padding: 25px;
              box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
          }
          
          h2 {
              text-align: center;
              margin-bottom: 25px;
              color: #2196F3;
              font-size: 1.8rem;
              display: flex;
              align-items: center;
              justify-content: center;
              gap: 10px;
          }
          
          .d-pad {
              display: flex;
              flex-direction: column;
              align-items: center;
              gap: 10px;
          }
          
          .d-pad-row {
              display: flex;
              justify-content: center;
              gap: 10px;
          }
          
          .control-btn {
              width: 80px;
              height: 80px;
              border-radius: 50%;
              border: none;
              background: #2196F3;
              color: white;
              font-size: 32px;
              cursor: pointer;
              display: flex;
              align-items: center;
              justify-content: center;
              transition: all 0.2s;
              box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
          }
          
          .control-btn:active {
              background: #0b7dda;
              transform: scale(0.95);
              box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
          }
          
          .stop-btn {
              background: #f44336;
              width: 90px;
              height: 90px;
          }
          
          .stop-btn:active {
              background: #d32f2f;
          }
          
          .slider-container {
              margin: 25px 0;
              padding: 15px;
              background: #f9f9f9;
              border-radius: 10px;
              box-shadow: 0 2px 5px rgba(0, 0, 0, 0.05);
          }
          
          .slider-header {
              display: flex;
              justify-content: space-between;
              align-items: center;
              margin-bottom: 10px;
          }
          
          .slider-label {
              font-size: 18px;
              font-weight: bold;
              display: flex;
              align-items: center;
              gap: 8px;
          }
          
          .value-display {
              font-size: 16px;
              font-weight: bold;
              color: #2196F3;
              background: #e3f2fd;
              padding: 4px 12px;
              border-radius: 20px;
              min-width: 50px;
              display: inline-block;
          }
          
          input[type=range] {
              width: 100%;
              height: 10px;
              /* -webkit-appearance: none; */
              background: #ddd;
              border-radius: 5px;
              outline: none;
          }
          
          input[type=range]::-webkit-slider-thumb {
              -webkit-appearance: none;
              width: 24px;
              height: 24px;
              border-radius: 50%;
              background: #2196F3;
              cursor: pointer;
              box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
          }
          
          .status-bar {
              text-align: center;
              padding: 15px;
              margin-top: 20px;
              background: #e3f2fd;
              border-radius: 10px;
              font-weight: bold;
          }
          
          .connection-status {
              display: inline-flex;
              align-items: center;
              gap: 8px;
              padding: 8px 16px;
              border-radius: 20px;
              background: #4caf50;
              color: white;
          }
          
          .connection-status::before {
              content: "";
              width: 12px;
              height: 12px;
              border-radius: 50%;
              background: white;
              animation: pulse 1.5s infinite;
          }
          
          @keyframes pulse {
              0% { opacity: 1; }
              50% { opacity: 0.4; }
              100% { opacity: 1; }
          }
          
          footer {
              text-align: center;
              padding: 20px;
              margin-top: 20px;
              color: white;
              background: #1565C0;
              font-size: 0.9rem;
          }
      </style>
  </head>
  <body>
      <div class="container">
          <header>
              <h1><i class="fas fa-robot"></i> ESP32 Car with Camera & Arm</h1>
              <p class="subtitle">Control your robot car remotely with real-time video feedback</p>
          </header>
          
          <div class="video-container">
              <img id="video" src="http://192.168.1.50:81/stream" alt="Live camera feed">
          </div>
          
          <div class="control-panels">
              <div class="car-controls panel">
                  <h2><i class="fas fa-car"></i> Car Controls</h2>
                  <div class="d-pad">
                      <div class="d-pad-row">
                          <button class="control-btn" id="forwardBtn" onmousedown="startCommand('forward')" ontouchstart="startCommand('forward')" onmouseup="stopCommand()" ontouchend="stopCommand()">
                              ⬆️
                          </button>
                      </div>
                      <div class="d-pad-row">
                          <button class="control-btn" id="leftBtn" onmousedown="startCommand('left')" ontouchstart="startCommand('left')" onmouseup="stopCommand()" ontouchend="stopCommand()">
                              ⬅️
                          </button>
                          <button class="control-btn stop-btn" id="stopBtn" onclick="sendCommand('stop')">
                              ⏹️
                          </button>
                          <button class="control-btn" id="rightBtn" onmousedown="startCommand('right')" ontouchstart="startCommand('right')" onmouseup="stopCommand()" ontouchend="stopCommand()">
                              ➡️
                          </button>
                      </div>
                      <div class="d-pad-row">
                          <button class="control-btn" id="backwardBtn" onmousedown="startCommand('backward')" ontouchstart="startCommand('backward')" onmouseup="stopCommand()" ontouchend="stopCommand()">
                              ⬇️
                          </button>
                      </div>
                  </div>
                  <div class="status-bar">
                      <p>Press and hold buttons to move the car. Release to stop.</p>
                      <p>Current command: <span id="currentCommand">None</span></p>
                  </div>
                  <div class="status-bar">
                      <div class="connection-status">Connected to ESP32</div>
                  </div>
              </div>
              
              <div class="arm-controls panel">
                  <h2><i class="fas fa-hand-point-up"></i> Robot Arm Control</h2>
                  
                  <div class="slider-container">
                      <div class="slider-header">
                          <span class="slider-label"><i class="fas fa-undo"></i> Base Rotation</span>
                          <span class="value-display"><span id="joint1Value">90</span>°</span>
                      </div>
                      <input type="range" min="0" max="180" value="120" id="joint1" oninput="updateJoint('joint1', this.value)">
                  </div>

                  <div class="slider-container">
                      <div class="slider-header">
                          <span class="slider-label"><i class="fas fa-arrows-alt-v"></i> Shoulder</span>
                          <span class="value-display"><span id="joint2Value">90</span>°</span>
                      </div>
                      <input type="range" min="0" max="140" value="0" id="joint2" oninput="updateJoint('joint2', this.value)">
                  </div>

                  <div class="slider-container">
                      <div class="slider-header">
                          <span class="slider-label"><i class="fas fa-arrows-alt-v"></i> Elbow</span>
                          <span class="value-display"><span id="joint3Value">90</span>°</span>
                      </div>
                      <input type="range" min="0" max="125" value="0" id="joint3" oninput="updateJoint('joint3', this.value)">
                  </div>

                  <div class="slider-container">
                      <div class="slider-header">
                          <span class="slider-label"><i class="fas fa-hand-rock"></i> Gripper</span>
                          <span class="value-display"><span id="joint4Value">90</span>°</span>
                      </div>
                      <input type="range" min="0" max="45" value="0" id="joint4" oninput="updateJoint('joint4', this.value)">
                  </div>
              </div>
          </div>
          
          <footer>
              <p>ESP32 Robot Control Interface | Designed for responsiveness and ease of use</p>
          </footer>
      </div>

      <script>
          let currentDirection = null;
          let intervalId = null;
          
          function sendCommand(command) {
              // In a real implementation, this would send a request to the ESP32
              console.log(`Sending command: ${command}`);
              document.getElementById('currentCommand').textContent = command;
              
              // Simulate network request
              fetch(`/${command}`, { method: 'POST' })
                  .catch(err => console.log('Control command error:', err));
          }
          
          function startCommand(direction) {
              if (currentDirection !== direction) {
                  stopCommand(); // Stop any previous movement
                  currentDirection = direction;
                  sendCommand(direction);
                  
                  // Continue sending the command every 300ms while button is pressed
                  intervalId = setInterval(() => {
                      sendCommand(direction);
                  }, 300);
              }
          }
          
          function stopCommand() {
              if (intervalId) {
                  clearInterval(intervalId);
                  intervalId = null;
              }
              
              if (currentDirection) {
                  sendCommand('stop');
                  currentDirection = null;
              }
          }
          
          function updateJoint(joint, value) {
              // Update the value display
              document.getElementById(`${joint}Value`).textContent = value;
              
              // In a real implementation, this would send a request to the ESP32
              console.log(`Setting ${joint} to ${value} degrees`);
              
              // Simulate network request
              fetch(`/${joint}?value=${value}`, { method: 'POST' })
                  .catch(err => console.log('Joint control error:', err));
          }
          
          // Add keyboard controls for better usability
          document.addEventListener('keydown', (e) => {
              if (e.key === 'ArrowUp') startCommand('forward');
              else if (e.key === 'ArrowDown') startCommand('backward');
              else if (e.key === 'ArrowLeft') startCommand('left');
              else if (e.key === 'ArrowRight') startCommand('right');
          });
          
          document.addEventListener('keyup', (e) => {
              if (['ArrowUp', 'ArrowDown', 'ArrowLeft', 'ArrowRight'].includes(e.key)) {
                  stopCommand();
              }
          });
      </script>
  </body>
  </html>
)rawliteral";

//---------------------- Motor Driver Pins (L298N) -------------------------
#define ENA 13
#define IN1 12
#define IN2 14
#define ENB 25
#define IN3 26
#define IN4 27

//---------------------- Robot Arm -------------------------
Adafruit_PWMServoDriver srituhobby = Adafruit_PWMServoDriver();
// Servo channels
#define servo1 0   // Base
#define servo2 1   // Shoulder
#define servo3 2   // Elbow
#define servo4 3   // Gripper

// Function declarations
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

// Handle root page
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", webpage);
}

// Handle car control commands
void handleCarCommand() {
  String command = server.uri();
  command.remove(0, 1); // Remove the leading slash
  
  if (command == "forward") {
    Serial.println("Car is moving forward");
    digitalWrite(32, HIGH);
    moveForward(200);
  } 
  else if (command == "backward") {
    Serial.println("Car is moving backward");
    digitalWrite(32, LOW);
    moveBackward(200);
  } 
  else if (command == "left") {
    Serial.println("Car is turning left");
    turnLeft(200);
  } 
  else if (command == "right") {
    Serial.println("Car is turning right");
    turnRight(200);
  } 
  else if (command == "stop") {
    Serial.println("Car has stopped");
    stopMotors();
  }
  
  server.send(200, "text/plain", "OK");
}

// ---- Function: Convert angle (0–180) into PWM pulse (150–600) ----
int angleToPulse(int ang) {
  int pulse_min = 150;   // ~0 degree
  int pulse_max = 600;   // ~180 degree
  return map(ang, 0, 180, pulse_min, pulse_max);
}

// Handle arm control commands for individual joints
void handleJoint1() {
  if (server.method() == HTTP_POST) {
    String value = server.arg("value");
    int intvalue = value.toInt();
    Serial.println("Joint1 (Base rotation) set to: " + value + "°");
    srituhobby.setPWM(servo1, 0, angleToPulse(intvalue));
    delay(20);
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void handleJoint2() {
  if (server.method() == HTTP_POST) {
    String value = server.arg("value");
    int intvalue = value.toInt();
    Serial.println("Joint2 (Shoulder) set to: " + value + "°");
    srituhobby.setPWM(servo2, 0, angleToPulse(intvalue));
    delay(20);
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void handleJoint3() {
  if (server.method() == HTTP_POST) {
    String value = server.arg("value");
    int intvalue = value.toInt();
    Serial.println("Joint3 (Elbow) set to: " + value + "°");
    srituhobby.setPWM(servo3, 0, angleToPulse(intvalue));
    delay(20);
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void handleJoint4() {
  if (server.method() == HTTP_POST) {
    String value = server.arg("value");
    int intvalue = value.toInt();
    Serial.println("Joint4 (Gripper) set to: " + value + "°");
    srituhobby.setPWM(servo4, 0, angleToPulse(intvalue));
    delay(20);
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void moveForward(int speed) {
  // Right motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
  
  // Left motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void moveBackward(int speed) {
  // Right motor backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);
  
  // Left motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

void turnLeft(int speed) {
  // Right motor forward (full speed)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
  
  // Left motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed); // Reduced speed for sharper turn
}

void turnRight(int speed) {
  // Right motor backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed); // Reduced speed for sharper turn
  
  // Left motor forward (full speed)
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void stopMotors() {
  // Stop both motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void setup() {
  // Motor pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);   

  pinMode(32, OUTPUT);

  Serial.begin(115200);

  //Robot Arm
  srituhobby.begin();
  srituhobby.setPWMFreq(60);
  // Initial positions
  srituhobby.setPWM(servo1, 0, angleToPulse(120));   // Base center
  srituhobby.setPWM(servo2, 0, angleToPulse(45));   // Shoulder down
  srituhobby.setPWM(servo3, 0, angleToPulse(60));   // Elbow bent
  srituhobby.setPWM(servo4, 0, angleToPulse(100));  // Gripper half open
  delay(2000);

  // Start WiFi in Access Point mode
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());  // Default: 192.168.4.1

  // Setup server routes
  server.on("/", handleRoot);
  server.on("/forward", HTTP_POST, handleCarCommand);
  server.on("/backward", HTTP_POST, handleCarCommand);
  server.on("/left", HTTP_POST, handleCarCommand);
  server.on("/right", HTTP_POST, handleCarCommand);
  server.on("/stop", HTTP_POST, handleCarCommand);
  
  // Fixed arm control endpoints - each joint has its own handler
  server.on("/joint1", HTTP_POST, handleJoint1);
  server.on("/joint2", HTTP_POST, handleJoint2);
  server.on("/joint3", HTTP_POST, handleJoint3);
  server.on("/joint4", HTTP_POST, handleJoint4);

  server.begin(); // Start server
  Serial.println("HTTP server started");

}

void loop() {
  server.handleClient();
}