#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>
#define DHT11_PIN 5
#include <Wire.h>
#include "rgb_lcd.h"
#include "homepage.h"
#include <time.h>

rgb_lcd lcd;


// Pin definitions
const int TRIG_PIN = 32;
const int ECHO_PIN = 35;
const int Led1 = 18;
const int SW1 = 17;
const int buzz = 13;

const int SW2 = 16;
const int SW3 = 4;
const int stopbuzz = 26;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

float duration, distance;

int mode = 0;
int setField = 0;

bool alarmTriggered = false;
bool alarmActive = false;
bool lastButtonState = HIGH;
int alarmHour = 11;
int alarmMinute = 40;
bool alarmMode = false;

void ultra();
 
// DHT sensor setup
DHT dht11(DHT11_PIN, DHT11);

void clickSound() 
{
  tone(buzz, 1000, 50); // click sound
}

// Wi-Fi credentials
const char *ssid = "dmakuiphone";
const char *password = "daniel123";

// time
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET 3600
#define UTC_OFFSET_DST 0

// Web server on port 80
WebServer server(80);

// Function for the temperature reading
String getTemp() {
  float tempC = dht11.readTemperature();
  if (isnan(tempC)) {
    return "Error reading temperature";
  } else {
    return String(tempC) ;
  }
}

// Function for humidity reading
String getHumidity() {
  float humidity = dht11.readHumidity();
  if (isnan(humidity)) {
    return "Error reading humidity";
  } else {
    return String(humidity);
  }
}


// Function to measure the distance from the ultrasonic sensor
String getDistance() {
  float duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.0343) / 2;
  
  return String(distance);
}

// Handle the root URL ("/") and serve the main page
void handleRoot() {
  String message = homePagePart1;  // Start with the first part of the HTML
  message += "<tr><td>" + getTemp() + "</td><td>" + getHumidity() + "</td><td>" + getDistance() + "</td></tr>";
  message += homePagePart2;  // Append the footer part
  server.send(200, "text/html", message);
}

// Handle AJAX request for real-time data updates
void handleSensorData() {
  String temp = getTemp();
  String humidity = getHumidity();
  String distance = getDistance();
  String timeNow = getTime();
  
  String data = "{\"temperature\": \"" + temp + "\", \"humidity\": \"" + humidity + "\", \"distance\": \"" + distance + "\" , \"time\": \"" + timeNow + "\"}";
  
  server.send(200, "application/json", data);  // Send data
}

// time 
String getTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "No Time";
  }

  char timeStr[20];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
  return String(timeStr);
}

// Handle the 404 error for non-existent routes
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

  // Initialize mDNS for local hostname
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

 // Initialize the DHT11 sensor
  dht11.begin();

// lcd
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);

  // Set up the ultrasonic sensor and LED pin
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(Led1, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(stopbuzz, INPUT_PULLUP);


  // Set up the web routes
  server.on("/", handleRoot);               // Main page route
  server.on("/sensorData", handleSensorData);  // AJAX route for real-time data
  server.onNotFound(handleNotFound);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  delay(100); // Allow the CPU to switch to other tasks


// Detect button press (falling edge)
 bool currentButtonState = digitalRead(SW1);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    mode = (mode + 1) % 3;  // Toggle between 3 modes
    lcd.clear();
    if (!alarmActive) clickSound();
    delay(200);  // debounce
  }

  lastButtonState = currentButtonState;

 if (digitalRead(SW2) == LOW) {
    setField = (setField + 1) % 2;
    if (!alarmActive) clickSound();
    delay(200);
  }

//BUTTON (SW3)
  if (digitalRead(SW3) == LOW) {

    if (mode == 2) { // SET ALARM
      if (setField == 0)
        alarmHour = (alarmHour + 1) % 24;
      else
        alarmMinute = (alarmMinute + 1) % 60;
      if (!alarmActive) clickSound();
    }

    delay(200);
  }


 if (mode == 0) {
    float tempC = dht11.readTemperature();
    float humi = dht11.readHumidity();
    distance = getDistance().toFloat();

    lcd.setCursor(0, 0);
    if (isnan(tempC) || isnan(humi)) {
      lcd.print("DHT11 FAIL");
    } else {
      lcd.print("Temp: ");
      lcd.print(tempC);
      lcd.print(" C");
    }

    lcd.setCursor(0, 1);
    if (distance < 5) {
      digitalWrite(Led1, HIGH);
      lcd.print("Motion detected");
    } else {
      digitalWrite(Led1, LOW);
      if (!isnan(humi)) {
        lcd.print("Humidity: ");
        lcd.print(humi);
        lcd.print("%");
      }
    }
  }
  // Mode 1: Time display
  else if (mode == 1) {
    String currentTime = getTime();
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.print(currentTime);
   lcd.setCursor(0, 1);
    lcd.print("Alarm at:");
    lcd.print(alarmHour);
    lcd.print(":");
    lcd.print(alarmMinute);

  }
  // Mode 2: Alarm settings
  else if (mode == 2) {
    lcd.setCursor(0, 0);
    lcd.print("Set Alarm");
    lcd.setCursor(0, 1);
    lcd.print(alarmHour);
    lcd.print(":");
    if (alarmMinute < 10) lcd.print("0");
    lcd.print(alarmMinute);
  }

  // Alarm check
  distance = getDistance().toFloat();

 struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int currentHour = timeinfo.tm_hour;
    int currentMinute = timeinfo.tm_min;
  


 if (currentHour == alarmHour && currentMinute == alarmMinute && !alarmTriggered) {
    alarmActive = true;
    alarmTriggered = true;
    if (alarmActive) {
    tone(buzz, 1000);
  }
  }



  if (alarmActive && (digitalRead(stopbuzz) == LOW || distance < 5)) {
    alarmActive = false;
    noTone(buzz);
  }

  // Reset when minute changes
  if (currentMinute != alarmMinute) {
    alarmTriggered = false;
  }
  }
}

