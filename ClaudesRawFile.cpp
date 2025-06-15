#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <RTClib.h>
#include <EEPROM.h>

// Pin definitions
const int IR_SEND_PIN = 4;
const int STATUS_LED = 2;
const int FAN_RELAY_PIN = 5;

// IR sender
IRsend irsend(IR_SEND_PIN);

// RTC
RTC_DS3231 rtc;

// Web server
WebServer server(80);

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Schedule structure
struct Schedule {
  int dayOfWeek; // 0=Sunday, 1=Monday, etc.
  int hour;
  int minute;
  int temperature;
  bool fanOn;
  bool active;
};

// System modes
enum SystemMode {
  NORMAL,
  AWAY,
  VACATION
};

// Global variables
Schedule schedules[20]; // Support up to 20 schedules
int scheduleCount = 0;
SystemMode currentMode = NORMAL;
int currentTemp = 72;
bool fanState = false;
bool acOn = false;
unsigned long lastScheduleCheck = 0;
const unsigned long SCHEDULE_CHECK_INTERVAL = 60000; // Check every minute

// EEPROM addresses
const int EEPROM_SIZE = 512;
const int SCHEDULE_ADDR = 0;
const int MODE_ADDR = 400;

// IR codes for AC (replace with your AC's codes)
const uint32_t AC_POWER = 0x20DF10EF;
const uint32_t AC_TEMP_UP = 0x20DF40BF;
const uint32_t AC_TEMP_DOWN = 0x20DF00FF;
const uint32_t AC_MODE = 0x20DFC03F;

void setup() {
  Serial.begin(115200);

  // Initialize pins
  pinMode(STATUS_LED, OUTPUT);
  pinMode(FAN_RELAY_PIN, OUTPUT);

  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Initialize IR sender
  irsend.begin();

  // Load schedules from EEPROM
  loadSchedules();
  loadSystemMode();

  // Connect to WiFi
  connectToWiFi();

  // Setup web server routes
  setupWebServer();

  Serial.println("Arduino AC Controller initialized");
  blinkStatusLED(3);
}

void loop() {
  server.handleClient();

  // Check schedules periodically
  if (millis() - lastScheduleCheck > SCHEDULE_CHECK_INTERVAL) {
    checkSchedules();
    lastScheduleCheck = millis();
  }

  delay(100);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void setupWebServer() {
  // Serve main control page
  server.on("/", HTTP_GET, handleRoot);

  // API endpoints
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/control", HTTP_POST, handleControl);
  server.on("/api/schedule", HTTP_GET, handleGetSchedule);
  server.on("/api/schedule", HTTP_POST, handleSetSchedule);
  server.on("/api/schedule", HTTP_DELETE, handleDeleteSchedule);
  server.on("/api/mode", HTTP_POST, handleSetMode);

  server.begin();
  Serial.println("Web server started");
}

void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>AC Controller</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial; margin: 20px; }
        .container { max-width: 600px; margin: 0 auto; }
        .control-group { margin: 20px 0; padding: 15px; border: 1px solid #ddd; border-radius: 5px; }
        button { padding: 10px 20px; margin: 5px; background: #007bff; color: white; border: none; border-radius: 3px; cursor: pointer; }
        button:hover { background: #0056b3; }
        input, select { padding: 8px; margin: 5px; border: 1px solid #ddd; border-radius: 3px; }
        .status { background: #f8f9fa; padding: 10px; border-radius: 5px; margin: 10px 0; }
        .schedule-item { background: #e9ecef; padding: 10px; margin: 5px 0; border-radius: 3px; }
    </style>
</head>
<body>
    <div class="container">
        <h1>AC & Fan Controller</h1>

        <div class="status" id="status">
            Loading status...
        </div>

        <div class="control-group">
            <h3>Manual Control</h3>
            <button onclick="toggleAC()">Toggle AC</button>
            <button onclick="tempUp()">Temp +</button>
            <button onclick="tempDown()">Temp -</button>
            <button onclick="toggleFan()">Toggle Fan</button>
        </div>

        <div class="control-group">
            <h3>System Mode</h3>
            <select id="modeSelect" onchange="setMode()">
                <option value="0">Normal</option>
                <option value="1">Away</option>
                <option value="2">Vacation</option>
            </select>
        </div>

        <div class="control-group">
            <h3>Add Schedule</h3>
            <select id="daySelect">
                <option value="0">Sunday</option>
                <option value="1">Monday</option>
                <option value="2">Tuesday</option>
                <option value="3">Wednesday</option>
                <option value="4">Thursday</option>
                <option value="5">Friday</option>
                <option value="6">Saturday</option>
                <option value="7">Daily</option>
            </select>
            <input type="time" id="timeInput">
            <input type="number" id="tempInput" placeholder="Temperature" min="60" max="85">
            <label><input type="checkbox" id="fanCheckbox"> Fan On</label>
            <button onclick="addSchedule()">Add Schedule</button>
        </div>

        <div class="control-group">
            <h3>Current Schedules</h3>
            <div id="schedules">Loading schedules...</div>
        </div>
    </div>

    <script>
        function updateStatus() {
            fetch('/api/status')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('status').innerHTML =
                        `Mode: ${['Normal', 'Away', 'Vacation'][data.mode]}<br>
                         AC: ${data.acOn ? 'ON' : 'OFF'}<br>
                         Temperature: ${data.temperature}°F<br>
                         Fan: ${data.fanOn ? 'ON' : 'OFF'}<br>
                         Time: ${data.time}`;
                    document.getElementById('modeSelect').value = data.mode;
                });
        }

        function updateSchedules() {
            fetch('/api/schedule')
                .then(response => response.json())
                .then(data => {
                    let html = '';
                    data.schedules.forEach((schedule, index) => {
                        const days = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Daily'];
                        html += `<div class="schedule-item">
                            ${days[schedule.day]} ${schedule.time} - ${schedule.temp}°F, Fan: ${schedule.fan ? 'ON' : 'OFF'}
                            <button onclick="deleteSchedule(${index})">Delete</button>
                        </div>`;
                    });
                    document.getElementById('schedules').innerHTML = html || 'No schedules';
                });
        }

        function toggleAC() { sendCommand('toggle_ac'); }
        function tempUp() { sendCommand('temp_up'); }
        function tempDown() { sendCommand('temp_down'); }
        function toggleFan() { sendCommand('toggle_fan'); }

        function sendCommand(command) {
            fetch('/api/control', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({command: command})
            }).then(() => setTimeout(updateStatus, 500));
        }

        function setMode() {
            const mode = document.getElementById('modeSelect').value;
            fetch('/api/mode', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({mode: parseInt(mode)})
            }).then(() => updateStatus());
        }

        function addSchedule() {
            const day = document.getElementById('daySelect').value;
            const time = document.getElementById('timeInput').value;
            const temp = document.getElementById('tempInput').value;
            const fan = document.getElementById('fanCheckbox').checked;

            if (!time || !temp) {
                alert('Please fill in all fields');
                return;
            }

            const [hour, minute] = time.split(':');
            fetch('/api/schedule', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({
                    day: parseInt(day),
                    hour: parseInt(hour),
                    minute: parseInt(minute),
                    temperature: parseInt(temp),
                    fan: fan
                })
            }).then(() => {
                updateSchedules();
                document.getElementById('timeInput').value = '';
                document.getElementById('tempInput').value = '';
                document.getElementById('fanCheckbox').checked = false;
            });
        }

        function deleteSchedule(index) {
            fetch('/api/schedule?index=' + index, {method: 'DELETE'})
                .then(() => updateSchedules());
        }

        // Update status and schedules on page load and periodically
        updateStatus();
        updateSchedules();
        setInterval(updateStatus, 10000); // Every 10 seconds
    </script>
</body>
</html>
  )";
  server.send(200, "text/html", html);
}

void handleStatus() {
  DateTime now = rtc.now();

  DynamicJsonDocument doc(1024);
  doc["mode"] = currentMode;
  doc["acOn"] = acOn;
  doc["temperature"] = currentTemp;
  doc["fanOn"] = fanState;
  doc["time"] = String(now.hour()) + ":" + String(now.minute());

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleControl() {
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, server.arg("plain"));

    String command = doc["command"];

    if (command == "toggle_ac") {
      toggleAC();
    } else if (command == "temp_up") {
      adjustTemperature(1);
    } else if (command == "temp_down") {
      adjustTemperature(-1);
    } else if (command == "toggle_fan") {
      toggleFan();
    }

    server.send(200, "application/json", "{\"status\":\"ok\"}");
  }
}

void handleGetSchedule() {
  DynamicJsonDocument doc(2048);
  JsonArray scheduleArray = doc.createNestedArray("schedules");

  for (int i = 0; i < scheduleCount; i++) {
    if (schedules[i].active) {
      JsonObject schedule = scheduleArray.createNestedObject();
      schedule["day"] = schedules[i].dayOfWeek;
      schedule["time"] = String(schedules[i].hour) + ":" +
                        (schedules[i].minute < 10 ? "0" : "") + String(schedules[i].minute);
      schedule["temp"] = schedules[i].temperature;
      schedule["fan"] = schedules[i].fanOn;
    }
  }

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleSetSchedule() {
  if (server.hasArg("plain") && scheduleCount < 20) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, server.arg("plain"));

    schedules[scheduleCount].dayOfWeek = doc["day"];
    schedules[scheduleCount].hour = doc["hour"];
    schedules[scheduleCount].minute = doc["minute"];
    schedules[scheduleCount].temperature = doc["temperature"];
    schedules[scheduleCount].fanOn = doc["fan"];
    schedules[scheduleCount].active = true;

    scheduleCount++;
    saveSchedules();

    server.send(200, "application/json", "{\"status\":\"ok\"}");
  } else {
    server.send(400, "application/json", "{\"error\":\"Schedule limit reached\"}");
  }
}

void handleDeleteSchedule() {
  if (server.hasArg("index")) {
    int index = server.arg("index").toInt();
    if (index >= 0 && index < scheduleCount) {
      // Shift schedules down
      for (int i = index; i < scheduleCount - 1; i++) {
        schedules[i] = schedules[i + 1];
      }
      scheduleCount--;
      saveSchedules();
      server.send(200, "application/json", "{\"status\":\"ok\"}");
    }
  }
}

void handleSetMode() {
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, server.arg("plain"));

    currentMode = (SystemMode)doc["mode"].as<int>();
    saveSystemMode();

    server.send(200, "application/json", "{\"status\":\"ok\"}");
  }
}

void checkSchedules() {
  if (currentMode != NORMAL) return; // Skip scheduling in away/vacation mode

  DateTime now = rtc.now();
  int currentDay = now.dayOfTheWeek();
  int currentHour = now.hour();
  int currentMinute = now.minute();

  for (int i = 0; i < scheduleCount; i++) {
    if (!schedules[i].active) continue;

    bool dayMatch = (schedules[i].dayOfWeek == currentDay || schedules[i].dayOfWeek == 7); // 7 = daily
    bool timeMatch = (schedules[i].hour == currentHour && schedules[i].minute == currentMinute);

    if (dayMatch && timeMatch) {
      Serial.println("Executing schedule " + String(i));
      setTemperature(schedules[i].temperature);
      setFan(schedules[i].fanOn);
      break; // Execute only one matching schedule
    }
  }
}

void toggleAC() {
  irsend.sendNEC(AC_POWER);
  acOn = !acOn;
  Serial.println("AC toggled: " + String(acOn ? "ON" : "OFF"));
  blinkStatusLED(1);
}

void adjustTemperature(int change) {
  if (!acOn) return;

  currentTemp += change;
  if (currentTemp < 60) currentTemp = 60;
  if (currentTemp > 85) currentTemp = 85;

  if (change > 0) {
    irsend.sendNEC(AC_TEMP_UP);
  } else {
    irsend.sendNEC(AC_TEMP_DOWN);
  }

  Serial.println("Temperature adjusted to: " + String(currentTemp));
  blinkStatusLED(1);
}

void setTemperature(int temp) {
  if (!acOn) {
    toggleAC();
    delay(2000); // Wait for AC to initialize
  }

  while (currentTemp != temp) {
    if (currentTemp < temp) {
      adjustTemperature(1);
    } else {
      adjustTemperature(-1);
    }
    delay(500); // Small delay between commands
  }
}

void toggleFan() {
  fanState = !fanState;
  digitalWrite(FAN_RELAY_PIN, fanState ? HIGH : LOW);
  Serial.println("Fan toggled: " + String(fanState ? "ON" : "OFF"));
  blinkStatusLED(1);
}

void setFan(bool state) {
  fanState = state;
  digitalWrite(FAN_RELAY_PIN, fanState ? HIGH : LOW);
  Serial.println("Fan set to: " + String(fanState ? "ON" : "OFF"));
}

void saveSchedules() {
  EEPROM.put(SCHEDULE_ADDR, schedules);
  EEPROM.put(SCHEDULE_ADDR + sizeof(schedules), scheduleCount);
  EEPROM.commit();
}

void loadSchedules() {
  EEPROM.get(SCHEDULE_ADDR, schedules);
  EEPROM.get(SCHEDULE_ADDR + sizeof(schedules), scheduleCount);

  // Validate loaded data
  if (scheduleCount < 0 || scheduleCount > 20) {
    scheduleCount = 0;
  }
}

void saveSystemMode() {
  EEPROM.put(MODE_ADDR, currentMode);
  EEPROM.commit();
}

void loadSystemMode() {
  EEPROM.get(MODE_ADDR, currentMode);

  // Validate loaded data
  if (currentMode < 0 || currentMode > 2) {
    currentMode = NORMAL;
  }
}

void blinkStatusLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(200);
    digitalWrite(STATUS_LED, LOW);
    delay(200);
  }
}