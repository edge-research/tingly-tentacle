
Datalogging temp and pressure (depth). 

# Adafruit Feather M0 Express

- [Adafruit Feather M0 Express Website](https://www.adafruit.com/product/3403) -- links and info.

# Blue Robotics TSYS01 'Fast Response' temp sensor

- [BR TSYS01 Website](https://www.bluerobotics.com/store/electronics/celsius-sensor-r1/) -- links and info.
- [BR TSYS01 Arduino Library](https://github.com/bluerobotics/BlueRobotics_TSYS01_Library)

# Blue Robotics MS5837 'Bar 30' pressure sensor
- [BR MS5837 Website](http://docs.bluerobotics.com/bar30/) -- links and info.
- [BR MS5837 Arduino Library](https://github.com/bluerobotics/BlueRobotics_MS5837_Library)

# Quick Notes on Setup

- We're going to be programming the device using the Arduino IDE, but fetching the data off using CircuitPython. This allows us to utilize the on-board SPI Flash chip, which is lower-power and (likely) more reliable than an SD card.

- The Blue Robotics sensors should be connected to the Feather via i2c lines.  Follow the wiring diagrams on their respective web pages.  The SDA and SCL lines will each need a 10K 'pullup' -- i.e. they each need to be connected via a 10K resistor to VCC (3.3 V).

- Install the above Arduino libraries for the temp sensor and the pressure sensor in the 'libraries' folder for your Arduino installation.

- Set up your Arduino IDE to allow for interacting with Feather M0 Express boards as per instructions here.

1.  Initially programming the board

- Double tap the 'reset' button to enter bootloader mode.
 
- Upload the 'feather_m0_express_br_temp_br_pressure_logger.ino' in this repo to the board.

- Once uploaded, double tap the 'reset' button again to enter bootloader mode.  Copy the '.UF2' file off the 'FEATHERBOOT' drive and store it.  This is your 'datalogger program', and will be needed after each time you collect data.
 
2. Collecting the data

- Double tap the 'reset' button to enter bootloader mode.

- Drag and drop the 'adafruit-circuitpython-feather_m0_express-2.0.0-beta.1.uf2' file onto the FEATHEROOT drive. 

- This will automatically reboot the board, and then mount as a 'CIRCUITPYTHON' drive. You can then drag-drop off the 'data.txt' file.

3.  Reprogramming the board to return to datalogging

- Double tap the 'reset' button to enter bootloader mode.

- Drag and drop your '.UF2' file (saved in step 1.) back onto the FEATHERBOOT drive.  This will automatically reboot the system and start running the Arduino datalogging program (may require a power cycle).

