#include <SoftwareSerial.h>
SoftwareSerial EEBlue(10, 11); // RX | TX

#define echoPin1 A3
#define triggerPin1 A2

#define echoPin2 A0
#define triggerPin2 A1

#define LeftLED 12
#define RightLED 13

char t;
bool enb_f = true;
bool enb_b = true;
long highPulseDuration;
int calculatedDistanceCm;

void setup() {
pinMode(3,OUTPUT);
pinMode(9,OUTPUT);
pinMode(4,OUTPUT);   //left motors forward
pinMode(5,OUTPUT);   //left motors reverse
pinMode(6,OUTPUT);   //right motors forward
pinMode(7,OUTPUT);   //right motors reverse
pinMode(triggerPin1, OUTPUT); // Sets the trigPin as an OUTPUT
pinMode(echoPin1, INPUT); // Sets the echoPin as an INPUT
pinMode(triggerPin2, OUTPUT); // Sets the trigPin as an OUTPUT
pinMode(echoPin2, INPUT); // Sets the echoPin as an INPUT   
pinMode(LeftLED,OUTPUT);
pinMode(RightLED,OUTPUT);
Serial.begin(9600);
EEBlue.begin(9600);
}

void loop() {
bluetoothControl(); //Bluetooth control function
double fd = front_UltrasonicSensor(); //Front Ultrasonic Sensor
double bd = back_UltrasonicSensor(); //Back Ultrasonic Sensor

if (fd<30.0){
  enb_f = false;
}else{
  enb_f = true;
}

if (bd<30.0){
  enb_b = false;
}else{
  enb_b = true;
}

}

void bluetoothControl() {
  //Get the Bluetooth control remote values
  if(EEBlue.available()){
  t = EEBlue.read();
  Serial.println(t);

    if ((t == 'F')&&(enb_f)) {
      forward();
    } 
    else if ((t == 'B')&&(enb_b)) {
      backward();
    } 
    else if (t == 'L') {
      left();
    } 
    else if (t == 'R') {
      right();
    } 
    else if (t == 'S') {
      Stop();
    }
    else{
      Stop();
    }
  }
}

/******************Motor functions*****************/
void forward() {
  digitalWrite(6,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(7,LOW);
  digitalWrite(5,LOW);
  digitalWrite(LeftLED,HIGH);
  digitalWrite(RightLED,HIGH);
  analogWrite(3,180);
  analogWrite(9,180);
}
void backward() {
  digitalWrite(7,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(4,LOW);
  digitalWrite(LeftLED,HIGH);
  digitalWrite(RightLED,HIGH);
  analogWrite(3,180);
  analogWrite(9,180);
}
void right() {
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(LeftLED,LOW);
  digitalWrite(RightLED,HIGH);
  analogWrite(3,225);
  analogWrite(9,0);
}
void left() {
  digitalWrite(6,HIGH);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(7,LOW);
  digitalWrite(LeftLED,HIGH);
  digitalWrite(RightLED,LOW);
  analogWrite(3,0);
  analogWrite(9,225);
}
void Stop() {
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(LeftLED,LOW);
  digitalWrite(RightLED,LOW);
  analogWrite(3,0);
  analogWrite(9,0);
}

/* -------------------Front Ultrasonic sensor-----------------------*/

double front_UltrasonicSensor()
{
  //Set the trigPin to low, before setting it to high for the pulse
   digitalWrite(triggerPin1, LOW);
   delayMicroseconds(5);

   // Create the 10 seconds pulse on the trig pin
   digitalWrite(triggerPin1, HIGH);
   delayMicroseconds(10);

   // Set the pin to low to end the pulse
   digitalWrite(triggerPin1, LOW);

   // Read the duration of the high pulse on the echo pin
   highPulseDuration = pulseIn(echoPin1, HIGH);

   // Calculating the distance
   calculatedDistanceCm = highPulseDuration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

   Serial.print("Calculated Distance: ");
   Serial.print(calculatedDistanceCm);
   Serial.println(" cm");
   return calculatedDistanceCm;
   // Displays the distance on the Serial Monitor
}

/*-------------------Back Ultrasonic Sensor----------------------*/

double back_UltrasonicSensor()
{
  //Set the trigPin to low, before setting it to high for the pulse
   digitalWrite(triggerPin2, LOW);
   delayMicroseconds(5);

   // Create the 10 seconds pulse on the trig pin
   digitalWrite(triggerPin2, HIGH);
   delayMicroseconds(10);

   // Set the pin to low to end the pulse
   digitalWrite(triggerPin2, LOW);

   // Read the duration of the high pulse on the echo pin
   highPulseDuration = pulseIn(echoPin2, HIGH);

   // Calculating the distance
   calculatedDistanceCm = highPulseDuration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

   Serial.print("Calculated Distance: ");
   Serial.print(calculatedDistanceCm);
   Serial.println(" cm");
   return calculatedDistanceCm;
  // Displays the distance on the Serial Monitor
}