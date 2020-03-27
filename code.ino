#include <EEPROM.h>               // 
#include <Wire.h>                 // Hardware needed
#include <ESP8266WiFi.h>          //    libraries
#include <MPU6050.h>              // 

   
#include <Firebase.h>             //
#include <FirebaseArduino.h>      // Firebase
#include <FirebaseError.h>        // libraries
#include <FirebaseHttpClient.h>   // 

MPU6050 mpu;                      // Define MPU6050
int SCL_PIN=D1;                   // Pin SCL "clock"
int SDA_PIN=D2;                   // Pin SDA "data"

int ax, ay, az, gx, gy, gz = 0;   // Acceleration and gyro variables

int addr = 0;                     // Memory address (max 512)
int psc = 0;                      // Message position (max 72)

// Firebase configuration
#define FIREBASE_HOST "------------------------"               // Firebase -> Database -> Real-time Database -> (URL under "Data   Rules   Backups  Usage")
#define FIREBASE_AUTH "------------------------"               // Firebase -> Project settings -> Service accounts -> Database secrets -> Secret
#define WIFI_SSID "----------------------------"               // Wifi name      
#define WIFI_PASSWORD "------------------------"               // Wifi password


void setup() 
{
  Wire.setClock(500000L);                     // Transmission velocity to 500kHz
  Serial.begin(115200);                       
  EEPROM.begin(512);                          // EEPROM addresses initialize
  Serial.print("Inicializando");
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);       // Wifi connection begins
    Serial.print("connecting");               
    while (WiFi.status() != WL_CONNECTED) {  
    Serial.print(".");
    delay(500); 
  }
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());                 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);    // Firebase connection begins
  
  delay(1);

  Serial.println("Initialize MPU6050");

  while(!mpu.beginSoftwareI2C(SCL_PIN,SDA_PIN,MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))   // In case connection with MPU6050 fail...
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  mpu.calibrateGyro();          // Gyro calibration
  
  checkSettings();              

}

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");     // MPU6050 availability
  
  Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())                                        // Clock configuration
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:         ");
  switch(mpu.getRange())                                             // Accelerometer calibrated to 2G
  {
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());                              // Ax value
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());                              // Ay value
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());                            // Az value
  
  Serial.println();

  Serial.print(" * Gyroscope:         ");
  switch(mpu.getScale())                                            // Calibrate gyro to 250 degrees/second   
  {
    case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
  } 
  
  Serial.print(" * Gyroscope offsets: ");
  Serial.print(mpu.getGyroOffsetX());                               // Gx value
  Serial.print(" / ");
  Serial.print(mpu.getGyroOffsetY());                               // Gy value
  Serial.print(" / ");
  Serial.println(mpu.getGyroOffsetZ());                             // Gz value
  
  Serial.println();
}

void loop(){
  
  while (addr <= 511){                                              // While addres under the max...
 
  Vector rawAccel = mpu.readRawAccel();                             // Export raw acceleration values
  Vector normAccel = mpu.readNormalizeAccel();                      // Export normalized acceleration values
  
  
  ax = normAccel.YAxis;                                             
  
  ay = normAccel.ZAxis;                                             
 
  az = normAccel.XAxis;                                             

  Vector rawGyro = mpu.readRawGyro();                               // Export raw gyro values
  Vector normGyro = mpu.readNormalizeGyro();                        // Export normalized gyro values

  
  gx = (normGyro.YAxis);                                            
 
  gy = (normGyro.ZAxis);                                            
         
  gz = (normGyro.XAxis);                                           

  /*Here all data is stored in the EEPROM. Data is stored in 7 byte agrupations per MPU6050 lecture
  ------------------------------------
  [ nº - ax - ay - az - gx - gy - gz ]
  ------------------------------------
  */
  
  EEPROM.write(addr, psc);                                          // A number between 0-72 is saved, to save values in order 
  addr = addr + 1;                                                  // Plus one for the next round
  EEPROM.commit();                                                  // Finish EEPROM writing 
  
  psc = psc + 1;                                                    // Plus one for the next round

  if (ax < 0){                                                      // It changes the value sign because
    ax = -(ax);                                                     // EEPROM can only save values between 0-255
    }
 
  EEPROM.write(addr, ax);                                           // Ax value byte 
  addr = addr + 1;
  EEPROM.commit();
  Firebase.setFloat("Ax", ax);                                      // Ax value is send to Firebase

  if (ay < 0){
    ay = -(ay);
    }
  
  EEPROM.write(addr, ay);                                           // Ay value byte
  addr = addr +1;
  EEPROM.commit();
  Firebase.setFloat("Ay", ay);                                      // Ay value is send to Firebase

  if (az < 0){
    az = -(az);
    }
  
  EEPROM.write(addr, az);                                           // Az value byte
  addr = addr + 1;
  EEPROM.commit();
  Firebase.setFloat("Az", az);                                      // Az value is send to Firebase

  if (gx < 0){
    gx = -(gx);
    }
  
  EEPROM.write(addr, gx);                                           // Gx value byte
  addr = addr + 1;
  EEPROM.commit();
  Firebase.setFloat("Gx", gx);                                      // Gx value is send to Firebase

  if (gy < 0){
    gy = -(gy);
    }
  
  EEPROM.write(addr, gy);                                           // Gy value byte
  addr = addr + 1;
  EEPROM.commit();
  Firebase.setFloat("Gy", gy);                                      // Gy value is send to Firebase
   
  if (gz < 0){
    gz = -(gz);
    }
  
  EEPROM.write(addr, gz);                                           // Gz value byte
  addr = addr + 1;
  EEPROM.commit();
  irebase.setFloat("Gz", gz);                                       // Gz value is send to Firebase

  delay(85); 

  }

  emmgdades = 1;
  Firebase.setFloat("Well done, all data has been saved correctly", emmgdades);  // Final message to conclude the procedure
  
}
