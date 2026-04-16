#include <DHT.h>
#define DHT11_PIN  5
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int trigPin = 32;
const int echoPin = 35;
const int Led1 = 18;
const int buzz = 13;


const int SW1 = 17;
const int SW2 = 16;
const int SW3 = 4;
const int stopbuzz = 26;



const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

int mode = 0;
int setField = 0;

//bool alarmMode = false;
bool lastButtonState = HIGH;

//time
int hours = 12;
int minutes = 0;

// Alarm
int alarmHour = 6;
int alarmMinute = 0;
bool alarmActive = false;

DHT dht11(DHT11_PIN, DHT11);
float duration, distance;

void setup() 
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(Led1,OUTPUT);
  pinMode(buzz, OUTPUT);

  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(stopbuzz, INPUT_PULLUP);

  Serial.begin(9600);
  dht11.begin(); 

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);

}

void loop() 
{
  bool currentButtonState = digitalRead(SW1);

if (lastButtonState == HIGH && currentButtonState == LOW) {
    mode = (mode + 1) % 3; // 3 modes
    lcd.clear();
    delay(200);
  }

lastButtonState = currentButtonState;
 //BUTTON (SW2)
  if (digitalRead(SW2) == LOW) {
    setField = (setField + 1) % 2;
    delay(200);
  }

//BUTTON (SW3)
  if (digitalRead(SW3) == LOW) {

    if (mode == 2) { // SET ALARM
      if (setField == 0)
        alarmHour = (alarmHour + 1) % 24;
      else
        alarmMinute = (alarmMinute + 1) % 60;
    }

    delay(200);
  }
// clock
  static unsigned long lastMillis = 0;
  if (millis() - lastMillis >= 60000) {
    lastMillis = millis();
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      hours = (hours + 1) % 24;
    }
  }

// normal sensor displays
if (mode == 0)
{


ultra();

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
if(distance < 5)
{
  digitalWrite (Led1, HIGH);
   lcd.print("Motion detected  ");
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
}

// display time TIME
  else if (mode == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("Time: ");

    lcd.print(hours);
    lcd.print(":");
    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);

    lcd.setCursor(0, 1);
    lcd.print("Alarm: ");
    lcd.print(alarmHour);
    lcd.print(":");
    if (alarmMinute < 10) lcd.print("0");
    lcd.print(alarmMinute);
  }

  // SET ALARM
  else if (mode == 2)
  {
    lcd.setCursor(0, 0);
    lcd.print("Set Alarm      ");

    lcd.setCursor(0, 1);
    lcd.print(alarmHour);
    lcd.print(":");
    if (alarmMinute < 10) lcd.print("0");
    lcd.print(alarmMinute);

    // optional cursor indicator
    if (setField == 0)
      lcd.setCursor(0, 1);
    else
      lcd.setCursor(3, 1);
  }

  // alarm set off
if (hours == alarmHour && minutes == alarmMinute) {
  alarmActive = true; // start alarm
}

if (alarmActive) {
  tone(buzz, 1000);
  
  // Check if STOP button is pressed
  if (digitalRead(stopbuzz) == LOW) {
    alarmActive = false;  // stop the alarm
    noTone(buzz);         // turn off buzzer
  }
}

  delay(200);
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

  }


