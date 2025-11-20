#include <DHT.h>
#define DHT11_PIN  4 // ESP32 pin GPIO21 connected to DHT11 sensor

DHT dht11(DHT11_PIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht11.begin(); // initialize the DHT11 sensor
}

void loop() {
  // read humidity
  float humi  = dht11.readHumidity();
  // read temperature in Celsius
  float tempC = dht11.readTemperature();


  // checking if the reading is successful or not
  if ( isnan(tempC)  || isnan(humi)) 
  {
    Serial.println("Failed to read from DHT11 sensor!");
  }
   else 
   {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ");
    delay(5000);
    Serial.print("\n");

  }
}