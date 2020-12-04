// FILE: TwoPin_DS18B20.ino
// AUTHOR: Edward PAEZ
// VERSION: 0.1.00
// PURPOSE: two pins for two sensors demo
// DATE: 10-11-2020
// URL: https://github.com/PaezEdward/2020-ECG-Heart-Monitor.git

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into pin 2 on the Arduino , You are free to take another one
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.

DallasTemperature sensors(&oneWire);
void setup(void)
{
// start serial port
Serial.begin(9600);
Serial.println("Dallas Temperature IC Control Library Demo");
// Start up the library
sensors.begin();
}
void loop(void)
{
// call sensors.requestTemperatures() to issue a global temperature
// request to all devices on the bus
Serial.print(" Requesting temperatures...");
sensors.requestTemperatures(); // Send the command to get temperatures
Serial.println("DONE");
Serial.print("Temperature is: ");
Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?
// You can have more than one IC on the same bus.
// 0 refers to the first IC on the wire
delay(1000);
}
