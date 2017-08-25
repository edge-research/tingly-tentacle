// Adafruit M0 Express CircuitPython Flash Example
// Author: Tony DiCola
//
// This is an example of reading and writing data from Arduino
// to the M0 Express flash filesystem used by CircuitPython.
// You can create, update, and read files on the CircuitPython
// filesystem in an Arduino sketch and then later load CircuitPython
// to interact with the same files.  This example will print out
// the contents of boot.py and main.py (if found) and add a line 
// to a data.txt file on CircuitPython's filesystem.
//
// Note before you use this sketch you must load CircuitPython
// on your M0 Express.  This will create the filesystem and
// initialize it, then you can load this example and read/write
// files on the board.
//
// Usage:
// - Modify the pins and type of fatfs object in the config
//   section below if necessary (usually not necessary).
// - Upload this sketch to your M0 express board.
// - Open the serial monitor at 115200 baud.  You should see the
//   example start to run and messages printed to the monitor.
//   If you don't see anything close the serial monitor, press
//   the board reset buttton, wait a few seconds, then open the
//   serial monitor again.


#define VBATPIN A7


#include <RTCZero.h>

RTCZero rtc;
int AlarmTime;


#include <SPI.h>
#include <Adafruit_SPIFlash.h>
#include <Adafruit_SPIFlash_FatFs.h>


#define SLEEP_TIME 5

// Configuration of the flash chip pins and flash fatfs object.
// You don't normally need to change these if using a Feather/Metro
// M0 express board.
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

float temp = temp_sensor.temperature();
float pressure = pressure_sensor.pressure();

float MS_temp = pressure_sensor.temperature();

float depth = pressure_sensor.depth();


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
    data.print(temp);
    data.print(",");
    data.print(MS_temp);
    data.print(",");
    data.print(pressure);
    data.print(",");
    data.println(depth);
    
    
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
