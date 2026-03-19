#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>
#include <Wire.h>
#include "homepage.h" // Include the homepage.h

// Pin definitions for the sensors and LED
#define DHT11_PIN 5
#define TRIG_PIN 4
#define ECHO_PIN 16
#define LED_PIN 18

 
// DHT sensor setup
DHT dht11(DHT11_PIN, DHT11);


// Wi-Fi credentials
const char *ssid = "dmakuiphone";
const char *password = "daniel123";

// Web server on port 80
WebServer server(80);

// Function for the temperature reading
String getTemp() 
{
  float tempC = dht11.readTemperature();
  if (isnan(tempC)) 
  {
    return "Error reading temperature";
  } else 
  {
    return String(tempC) ;
  }
}

// Function for humidity reading
String getHumidity() 
{
  float humidity = dht11.readHumidity();
  if (isnan(humidity)) 
  {
    return "Error reading humidity";
  } else 
  {
    return String(humidity);
  }
}

// Function to measure the distance from the ultrasonic sensor
String getDistance() 
{
  float duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.0343) / 2; // Convert to cm
  
  if (distance < 20) {
    digitalWrite(LED_PIN, HIGH); 
  } else {
    digitalWrite(LED_PIN, LOW); 
  }
  
  return String(distance);
}

// Handle the root URL ("/") and serve the main page
void handleRoot() {
  String message = homePagePart1;  // Start with the first part of the html
  message += "<tr><td>" + getTemp() + "</td><td>" + getHumidity() + "</td><td>" + getDistance() + "</td></tr>";
  message += homePagePart2;  // Append the footer part
  server.send(200, "text/html", message);
}

// Handle AJAX request for real-time data updates
void handleSensorData() {
  String temp = getTemp();
  String humidity = getHumidity();
  String distance = getDistance();
  
  String data = "{\"temperature\": \"" + temp + "\", \"humidity\": \"" + humidity + "\", \"distance\": \"" + distance + "\"}";
  
  server.send(200, "application/json", data);  // Send data
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

  // Initialize mDNS for local hostname
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

 // Initialize the DHT11 sensor
  dht11.begin();

  // Set up the ultrasonic sensor and LED pin
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);



  // Set up the web routes
  server.on("/", handleRoot); // Main page route
  server.on("/sensorData", handleSensorData); // route for realtime info
  server.onNotFound(handleNotFound);

  // to start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  delay(100); // Allow the CPU to switch to other tasks
}
