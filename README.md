# Simple Mars Rover

An ESP32-based robotic rover controlled wirelessly from a phone or laptop browser. The
ESP32 runs a WiFi access point and a web server that serves a responsive control panel for
the drivetrain and a 4-joint robotic arm. A live camera feed is embedded in the page for
remote monitoring.

## Features

- **WiFi access point + web server** — the ESP32 creates its own network (`ESP32_AP`) and
  hosts the control interface on port 80, so no router or internet connection is needed
- **Responsive mobile-first control panel** served entirely from the sketch — works on
  phones, tablets, and desktops
- **Live camera feed** embedded in the page for real-time monitoring
- **D-pad drive controls** with press-and-hold behaviour (commands re-send every 300 ms,
  release sends `stop`) plus arrow-key keyboard support on desktop
- **Four-axis robotic arm control** via independent sliders:
  | Joint | Range | Channel |
  |---|---|---|
  | Base rotation | 0–180° | 0 |
  | Shoulder | 0–140° | 1 |
  | Elbow | 0–125° | 2 |
  | Gripper | 0–45° | 3 |
- **Angle-to-PWM conversion** (`angleToPulse`) maps 0–180° onto the PCA9685 driver's
  150–600 µs pulse range at 60 Hz
- **Serial debug logging** of every command at 115200 baud

## Project Structure

```
code/
├── code.ino   # Complete sketch — HTML/CSS/JS web UI, HTTP routes, motor & arm logic
└── README.md
```

## Hardware

| Component | ESP32 pin / notes |
|---|---|
| L298N — ENA | 13 (PWM, speed) |
| L298N — IN1 / IN2 | 12 / 14 (right motor direction) |
| L298N — ENB | 25 (PWM, speed) |
| L298N — IN3 / IN4 | 26 / 27 (left motor direction) |
| Aux output | 32 (toggles HIGH when driving forward) |
| PCA9685 servo driver | I²C (`Wire.h`) — channels 0–3 for the arm |
| Robot arm servos | 4x, on PCA9685 channels 0–3 |
| DC drive motors | 2x, via L298N outputs |
| Camera | Separate module streaming to `http://192.168.1.50:81/stream` |

## Technologies

| Technology | Purpose |
|---|---|
| ESP32 | Main controller (WiFi + dual core) |
| Arduino framework (C/C++) | Platform |
| `WiFi.h` | Access point mode |
| `WebServer.h` | HTTP server, routes, and POST handling |
| `Wire.h` (I²C) | Communication with the servo driver |
| `Adafruit_PWMServoDriver` | PCA9685 16-channel PWM driver for the servos |
| `L298N` | Dual H-bridge DC motor driver |
| HTML5 / CSS3 / JavaScript | Embedded control panel (served from flash) |
| Font Awesome 6.4 (CDN) | Icons in the control panel |

## Getting Started

### Prerequisites

- Arduino IDE
- ESP32 board support package
- ESP32 dev board, L298N motor driver, PCA9685 servo driver, 4 servos, DC motors

### Required Libraries

Install via the Arduino Library Manager:

| Library | Author |
|---|---|
| `Adafruit PWM Servo Driver Library` | Adafruit |
| `Adafruit BusIO` | Adafruit (dependency) |

`WiFi.h`, `WebServer.h`, and `Wire.h` ship with the ESP32 core.

### Setup

1. Open `code.ino` in the Arduino IDE and select an ESP32 board.
2. Adjust the SSID and password near the top of the sketch if you want different network
   credentials.
3. Update the camera stream URL if your camera's IP differs from `192.168.1.50`.
4. Upload the sketch.
5. Connect your device to the `ESP32_AP` network (password: `12345678` by default).
6. Open `http://192.168.4.1/` in a browser.

## API

The web server exposes the following endpoints, all `POST`:

| Endpoint | Effect |
|---|---|
| `/forward` | Drive forward at PWM 200 |
| `/backward` | Drive backward at PWM 200 |
| `/left` | Turn left |
| `/right` | Turn right |
| `/stop` | Stop both motors |
| `/joint1` – `/joint4` | Set arm joint angle from the `value` form field |

`GET /` returns the control panel HTML.

## Notes

- The WiFi AP password is hardcoded as `12345678` — change it before use in any shared or
  public setting.
- Command handlers do not authenticate requests; anyone on the access point can drive the
  rover.
- The camera stream is served by a separate device on the network, not by the ESP32.