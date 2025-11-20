// last year proj code - change stuff around

const int trigPin = 4;
const int echoPin = 16;
const int Led1 = 18;

float duration, distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  pinMode(Led1,OUTPUT);
}

void loop() 
{

ultra();

if(distance < 20)
{
  digitalWrite (Led1, HIGH);
}

else
{
    digitalWrite (Led1, LOW);
}


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
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);

}