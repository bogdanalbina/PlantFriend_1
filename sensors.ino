//temp sensor init and pin
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 4

//muscle electric
int EMGsensorPin = A3;
int EMGsensorValue = 0;
int electricValue[20];
int i=0;

//current sensor
int currentPin = A1;
int currentValue = 0;

//input pin conductivity touch sensor
int conductivityInputPin = 13;
int conductivitySensor = 0;

//soil sensor pin init
int soilSensorPin = A0;
int soilSensorValue = 0;

//switch
int switchValue = 0;

//piezo pin init - I don't use this sensor
//int piezoPin = 4;

//sound sensor init
int soundSensor = A5;
int soundLevel = 0;

//instead of delay use millis
unsigned long previousMillis = 0UL;
unsigned long previousMillisFast = 0UL;
unsigned long standardInterval = 2000UL;
unsigned long fastInterval = 100UL;

int sw = 1;

//define pins and variables for ultrasonic sensor
const int trigPin = 9;  
const int echoPin = 10; 
float duration, distance;

//setting a min and max for Noise Sensor and Ultrasonic sensor
int maxSound = 0;
float minUltra = 1000;

void setup() {
  // put your setup code here, to run once:
  //ultrasonic setup
  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);
  pinMode(conductivityInputPin,INPUT);
  pinMode(EMGsensorPin,INPUT);
  
  //Serial setup
  Serial.begin(115200);
}

void loop() {
  //temp sensor works on 2s so minimum delay must be 2000
  //piezo works continous so with 2000 is hard to detect normal movement, only for strong vibrations works
  //delay(2000);
  
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillisFast > fastInterval) {
	  // The Arduino executes this code once every fastInterval ms
    
 	  //sound level
    soundLevel = analogRead(soundSensor);
    if (soundLevel > maxSound) {
      maxSound = soundLevel;
    }

    //ultrasonic code
    digitalWrite(trigPin, LOW);  
	  delayMicroseconds(2);  
	  digitalWrite(trigPin, HIGH);  
	  delayMicroseconds(10);  
	  digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = (duration*.0343)/2;
    if (distance < minUltra) {
      minUltra = distance;
    }

    EMGsensorValue = analogRead(EMGsensorPin);
    //Serial.println(EMGsensorValue);
    
    if(i<20){
      electricValue[i] = EMGsensorValue;
      i++;
    }
    //Serial.println(EMGsensorValue);
    
    //To see the electric signal uncomment below and comment all other serials.
    //Serial.println(EMGsensorValue);
 
 	  // Don't forget to update the previousMillis value
 	  previousMillisFast = currentMillis;
  }

  

  if (currentMillis - previousMillis > standardInterval)
  {
    //switch
    switchValue = analogRead(A7);
    //delay(1);

    DHT.read(DHT11_PIN);
    
    if (switchValue < 500) {
      sw = 0;
    } else {sw = 1;}
    
    //float time = micros()/1e6; // time is currentMillis
    float temp = 0;
    
    soilSensorValue = analogRead(soilSensorPin);
    //map values from 255 to 0 (reverse so dry is low, and high is humid)
    soilSensorValue = map(soilSensorValue,0,1024,255,0);
    
    if(DHT.temperature > 200){
      //error
      temp = 666;
    } else {
      //temp error 666
      temp = DHT.temperature;
      }

    
    
    if (sw==0) {
      Serial.println(sw);
    }
    else {

    
    conductivitySensor=digitalRead(conductivityInputPin);
    Serial.print(sw);
    Serial.print("; ");
    Serial.print(currentMillis);
    Serial.print("; ");
    Serial.print(temp);
    Serial.print("; " );
    //no vibroSensor needed
    //Serial.print(vibroSensorState);
    //Serial.print(", ")
    Serial.print(soilSensorValue);
    Serial.print("; ");
    Serial.print(minUltra);
    Serial.print("; ");
    Serial.print(conductivitySensor);
    Serial.print("; ");
    
    for (int j = 0;j< 20; j++){
      if (j<19) {
        Serial.print(electricValue[j]);
        Serial.print(",");
      }
      else{
        Serial.print(electricValue[j]);
      }
      
    }
    
    //print_array(electricValue);
    
    Serial.print("; ");
    currentValue = analogRead(currentPin);
    Serial.print(currentValue);
    Serial.print("; ");
    Serial.println(maxSound);}
  
    maxSound = 0;
    minUltra = 1000;
    i=0;
    
   	previousMillis = currentMillis;
  }

  //vibration sensor state HIGH and LOW - not in use
  //int vibroSensorState = digitalRead(piezoPin);
  
}

void print_array(int array[20]){
  Serial.print("[");
  for(int j = 0; j < 20; j++)
{
  Serial.print(array[j]+",");
}
Serial.print("]");
}
