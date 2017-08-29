

#define VBATPIN A7


#include <RTCZero.h>

RTCZero rtc;
int AlarmTime;


#include <SPI.h>
#include <Adafruit_SPIFlash.h>
#include <Adafruit_SPIFlash_FatFs.h>


#define SLEEP_TIME 5

#define FLASH_TYPE     SPIFLASHTYPE_W25Q16BV  // Flash chip type.
                                              // If you change this be
                                              // sure to change the fatfs
                                              // object type below to match.

#define FLASH_SS       SS1                    // Flash chip SS pin.
#define FLASH_SPI_PORT SPI1                   // What SPI port is Flash on?

Adafruit_SPIFlash flash(FLASH_SS, &FLASH_SPI_PORT);     // Use hardware SPI 

// Alternatively you can define and use non-SPI pins!
//Adafruit_SPIFlash flash(SCK1, MISO1, MOSI1, FLASH_SS);

// Finally create an Adafruit_M0_Express_CircuitPython object which gives
// an SD card-like interface to interacting with files stored in CircuitPython's
// flash filesystem.
Adafruit_M0_Express_CircuitPython pythonfs(flash);

#include <Wire.h>

#include "TSYS01.h"
#include "MS5837.h"

TSYS01 temp_sensor;
MS5837 pressure_sensor;

void setup() {
  // Initialize serial port and wait for it to open before continuing.

   pinMode(LED_BUILTIN, OUTPUT);

   rtc.begin();
   
   Wire.begin();

  temp_sensor.init();

  pressure_sensor.init();
  pressure_sensor.setModel(MS5837::MS5837_30BA);
  pressure_sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  
  // Initialize flash library and check its chip ID.
  if (!flash.begin(FLASH_TYPE)) {
   // Serial.println("Error, failed to initialize flash chip!");

    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
  
      while(1);
  
  }
  
  if (!pythonfs.begin()) {
    
 digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
  
    while(1);
  }

}

void loop() {

temp_sensor.read();
pressure_sensor.read();

float temp = temp_sensor.temperature(); //  Blue Robotics TSYS01 'Fast response' temp sensor

float pressure = pressure_sensor.pressure();  // Blue Robotics MS5837 'Bar30' pressure sensor

float MS_temp = pressure_sensor.temperature();  // Blue Robotics MS5837 'Bar30'temperature

float depth = pressure_sensor.depth(); // Blue Robotics MS5837 'Bar30' depth estimate 


float measuredvbat = analogRead(VBATPIN);
measuredvbat *= 2;    // we divided by 2, so multiply back
measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
measuredvbat /= 1024; // convert to voltage


  // Create or append to a data.txt file and add a new line
  // to the end of it.  CircuitPython code can later open and
  // see this file too!
  File data = pythonfs.open("data.txt", FILE_WRITE);
  if (data) {
    // Write a new line to the file:
    data.print(temp); //  Blue Robotics TSYS01 'Fast response' temp sensor
    data.print(",");
    data.print(MS_temp);  // Blue Robotics MS5837 'Bar30' temperature
    data.print(",");
    data.print(pressure);  // Blue Robotics MS5837 'Bar30' pressure sensor
    data.print(",");
    data.println(depth); // Blue Robotics MS5837 'Bar30' depth estimate 

    
    
    data.close();
    // See the other fatfs examples like fatfs_full_usage and fatfs_datalogging
    // for more examples of interacting with files.

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(100);  

  AlarmTime += SLEEP_TIME; // Adds 5 seconds to alarm time
  AlarmTime = AlarmTime % 60; // checks for roll over 60 seconds and corrects
  rtc.setAlarmSeconds(AlarmTime); // Wakes at next alarm time, i.e. every 5 secs
  
  rtc.enableAlarm(rtc.MATCH_SS); // Match seconds only
  rtc.attachInterrupt(alarmMatch); // Attach function to interupt
  rtc.standbyMode();    // Sleep until next alarm match
  
  
  }
  else {
    
 digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);    }

  
}


void alarmMatch() // Do something when interrupt called
{
  
}
