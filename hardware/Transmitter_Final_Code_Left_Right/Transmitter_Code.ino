#include "BluetoothSerial.h"

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

BluetoothSerial SerialBT;
Adafruit_MPU6050 mpu;

String MACadd = "98:D3:32:11:31:37";
uint8_t address[6]  = {0x98, 0xD3, 0x32, 0x11, 0x31, 0x37};
//uint8_t address[6]  = {0x11, 0x1D, 0xA5, 0x02, 0xC3, 0x22};
String name = "HC-05";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;

void setup() {
  pinMode(18,INPUT_PULLUP);
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);

  SerialBT.begin("ESP32test", true); 
  SerialBT.setPin(pin);
  Serial.println("The device started in master mode, make sure remote BT device is on!"); 

  connected = SerialBT.connect(name);

  if(connected) {
    Serial.println("Connected Succesfully!");
  } 
  
  else 
  {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }
  SerialBT.connect();
}

void loop() {
   /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if(digitalRead(18)==LOW){
    SerialBT.write('S');
    Serial.println("stop");
  }
  else if((a.acceleration.x > -2 && a.acceleration.x < 2) && (a.acceleration.y > -3 && a.acceleration.y < 3)) {
    SerialBT.write('S');
    Serial.println("stop");
  }  
  else if (a.acceleration.x < -2) {
    SerialBT.write('F');
    Serial.println("move forward");
  }
  else if (a.acceleration.x > 2) {
    SerialBT.write('B');
    Serial.println("move backward");
  }
  else if (a.acceleration.y < -3) {
    SerialBT.write('L');
    Serial.println("move left");
  }
  else if (a.acceleration.y > 3) {
    SerialBT.write('R');
    Serial.println("move right");    
  }

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  // Serial.print(", Z: ");
  // Serial.print(a.acceleration.z);
  // Serial.println(" m/s^2");

  Serial.println("");
  delay(500);
}
