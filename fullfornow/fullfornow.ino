#include <DHT.h>
#define DHT11_PIN  5
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int trigPin = 4;
const int echoPin = 16;
const int Led1 = 18;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

DHT dht11(DHT11_PIN, DHT11);
float duration, distance;


void setup() 
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(Led1,OUTPUT);


  Serial.begin(9600);
  dht11.begin(); 

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);

}

void loop() 
{

ultra();    //call the hc function 


 float humi  = dht11.readHumidity();

  // read temperature in Celsius
  float tempC = dht11.readTemperature();


  // checking if the reading is successful or not

  if ( isnan(tempC)  || isnan(humi)) 
  {
    lcd.setCursor(0, 0);
    lcd.print("DHT11 FAIL");
  }
   else 
   {
      lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.print(tempC);
    lcd.print("°C     ");

  }

  lcd.setCursor(0, 1);

if(distance < 20)
{
  digitalWrite (Led1, HIGH);
   lcd.print("Motion detected  ");

   Serial.print("Distance: ");
    Serial.println(distance);
}

else
{
    digitalWrite (Led1, LOW);

   if  (!isnan(humi))
   {
    lcd.print("Humidity: ");
    lcd.print(humi);
    lcd.print("%     ");
   }
else
    {
      lcd.print("                "); // clear line
    }
}
delay(300);
}

void ultra()
{
digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;

  delay(50);

//

  
  }

