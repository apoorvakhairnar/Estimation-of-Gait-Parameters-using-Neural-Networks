//testing some things 11/17 5 pm

#include <Wire.h>
#include <Adafruit_BNO08x.h>
#include "SparkFun_BNO080_Arduino_Library.h"
#define FREQ 40
#include <sh2.h>
#include <SD.h>
#include <sh2_SensorValue.h>
#define cardSelect        33
#define POLL_FREQ         40
#define CLK_FREQ          80000000

void error()
{
  for(int j=0;j<10;j++)
  {digitalWrite(13,HIGH);
  delay(500);
  digitalWrite(13,LOW);
  delay(500);}
}
char filename[20];
File dataFile;
struct Data {
  float accelx, accely, accelz;
  float quat_w, quat_x, quat_y, quat_z;
};
unsigned long startTime=0;

Data sensorData;
sh2_SensorValue_t sensorValue;
BNO080 thighIMU;
BNO080 shinIMU;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  pinMode(13,OUTPUT);
  //startTime=millis();
  
  Wire.begin();
  Wire.setClock(400000);
  
  if(!SD.begin(cardSelect)){
    Serial.println("Card init failed");
    error();
    }
    else{
     Serial.println("SD card initialized!");
     }
  strcpy(filename, "/");
  //strcat(filename, board_name);
  strcat(filename, "imu_data00.csv");
  for (uint8_t i = 0; i < 100; i++) {
      filename[9] = '0' + i / 10;
      filename[10] = '0' + i % 10;
      // create if does not exist, do not open existing, write, sync after write
      if (! SD.exists(filename)) {
        Serial.println(filename);
        break;
      }
    }

  dataFile=SD.open(filename,FILE_WRITE);
  if (!thighIMU.begin(0x4A, Wire)) {
    Serial.println("thigh BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    error();
    while (1)
      ;
  }
  if (!shinIMU.begin(0x4B, Wire)) {
    Serial.println("Shin BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    error();
    while (1)
      ;
  }
  Serial.println("Found both sensors:");
  
  thighIMU.enableAccelerometer(5);  //Send data update every 50ms
  thighIMU.enableGyroIntegratedRotationVector(5);
  thighIMU.enableGyro(5);
  shinIMU.enableAccelerometer(5);  //Send data update every 50ms
  shinIMU.enableGyroIntegratedRotationVector(5);
  shinIMU.enableGyro(5);
  Serial.println("Setting desired reports for sensors");
  Serial.println("done");


  xTaskCreate(sensor_data, "Log sensor data", 10000, NULL, 1, NULL);
}

void loop() {
}

void sensor_data(void* parameter) {
  float x,y,z,quatI,quatJ,quatK,quatReal,gyroY,x2,y2,z2,quatI2,quatJ2,quatK2,quatReal2,gyroY2;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(1000 / FREQ);
  Serial.print("loop");
  if (dataFile) {
    dataFile.println("TimeElapsed,QuatReal,QuatI,QuatJ,QuatK,AccelX,AccelY,AccelZ,wx,wy,wz,QuatReal2,QuatI2,QuatJ2,QuatK2,AccelX2,AccelY2,AccelZ2,wx2,wy2,wz2");
  }
  
  else{error();}
  //delay(1000);
  int rssiarray[2];
  for (int jj=0;jj<10;jj++)
  rssiarray[jj]=0;
  int index =0,total=0,var=0;
  int avg =0;
  for (;;) {
    if (thighIMU.dataAvailable() == true) {
      if (dataFile) {
        dataFile.print(millis());
        dataFile.print(",");
        }
       x = thighIMU.getAccelX();
       y = thighIMU.getAccelY();
       z = thighIMU.getAccelZ();

       quatI = thighIMU.getQuatI();
       quatJ = thighIMU.getQuatJ();
       quatK = thighIMU.getQuatK();
       quatReal = thighIMU.getQuatReal();
       //gyroX = thighIMU.getGyroX();
       gyroY = thighIMU.getGyroY();

       //gyroZ = thighIMU.getGyroZ();
      dataFile.print(quatReal);
        dataFile.print(",");
        dataFile.print(quatI);
        dataFile.print(",");
        dataFile.print(quatJ);
        dataFile.print(",");
        dataFile.print(quatK);
        dataFile.print(",");
        dataFile.print(x);
        dataFile.print(",");
        dataFile.print(y);
        dataFile.print(",");
        dataFile.print(z);
        dataFile.print(",");
        //dataFile.print(gyroX);
        //dataFile.print(",");
        dataFile.print(gyroY);
        dataFile.print(",");
        //dataFile.print(gyroZ);
        //dataFile.print(",");
      //  Serial.print("ThighSensor");
      // Serial.print(quatReal);
      // Serial.print(",");
      // Serial.print(quatI);
      // Serial.print(",");
      // Serial.print(quatJ);
      // Serial.print(",");
      // Serial.print(quatK);
      // Serial.print(",");
      // Serial.print(x);
      // Serial.print(",");
      // Serial.print(y);
      // Serial.print(",");
      // Serial.print(z);
      // Serial.print(",");
      // Serial.print(gyroX);
      // Serial.print(",");
      // Serial.print(gyroY);
      // Serial.print(",");
      // Serial.print(gyroZ);
      // Serial.print(",");
     }
    if (shinIMU.dataAvailable() == true) {
      
       x2 = shinIMU.getAccelX();
       y2 = shinIMU.getAccelY();
       z2 = shinIMU.getAccelZ();

       quatI2 = shinIMU.getQuatI();
       quatJ2 = shinIMU.getQuatJ();
       quatK2 = shinIMU.getQuatK();
       quatReal2 = shinIMU.getQuatReal();
       
       gyroY2 = shinIMU.getGyroY();
        dataFile.print(quatReal2);
        dataFile.print(",");
        dataFile.print(quatI2);
        dataFile.print(",");
        dataFile.print(quatJ2);
        dataFile.print(",");
        dataFile.print(quatK2);
        dataFile.print(",");
        dataFile.print(x2);
        dataFile.print(",");
        dataFile.print(y2);
        dataFile.print(",");
        dataFile.print(z2);
        dataFile.print(",");
        // dataFile.print(gyroX2);
        // dataFile.print(",");
        dataFile.println(gyroY2);
        // dataFile.print(",");
        // dataFile.println(gyroZ2);
      //  Serial.print("ShinSensor");
      // Serial.print(quatReal2);
      // Serial.print(",");
      // Serial.print(quatI2);
      // Serial.print(",");
      // Serial.print(quatJ2);
      // Serial.print(",");
      // Serial.print(quatK2);
      // Serial.print(x2);
      // Serial.print(",");
      // Serial.print(y2);
      // Serial.print(",");
      // Serial.print(z2);
      // Serial.print(",");
      // Serial.print(gyroX2);
      // Serial.print(",");
      // Serial.print(gyroY2);
      // Serial.print(",");
      // Serial.print(gyroZ2);
      // Serial.println(",");
     } //float quatRadianAccuracy = myIMU.getQuatRadianAccuracy();
      
       //float quatRadianAccuracy = myIMU.getQuatRadianAccuracy();
      if (dataFile) {
        // dataFile.print(millis() - startTime);
        // dataFile.print(",");
        
       
        //dataFile.print(",");
      }
      
      //Serial.println();
      
      
      dataFile.flush();
    
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
  }

