 /*
 V1.0 - first version
 
 Created by Igor Jarc <igor.jarc1@gmail.com>
 See http://iot-playground.com for details
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
#include <Esp8266EasyIoT.h>
#include <SoftwareSerial.h> 
#include <OneWire.h>
#include <DallasTemperature.h>

#define CHILD_ID_TEMP 1
#define HUMIDITY_SENSOR_DIGITAL_PIN 2

Esp8266EasyIoT esp; 
SoftwareSerial serialEsp(10, 11);

float lastTemp;

Esp8266EasyIoTMsg msgTemp(CHILD_ID_TEMP, V_TEMP);

#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

void setup()
{
  serialEsp.begin(9600);
  Serial.begin(115200);  

  Serial.println("EasyIoTEsp init");


  esp.begin(NULL, 3, &serialEsp, &Serial);
  //esp.begin(NULL, &serialEsp);

  pinMode(13, OUTPUT);

//  Serial.println("present S_TEMP");
  esp.present(CHILD_ID_TEMP, S_TEMP);
  sensors.begin();
}

void loop()
{  
  while(!esp.process());

  sensors.requestTemperatures();
  float temperature = (sensors.getTempCByIndex(0));
  if (isnan(temperature)) {
    Serial.println("Failed reading temperature from DHT");
  } 
  else if (temperature != lastTemp) 
  {
    lastTemp = temperature;
    esp.send(msgTemp.set(temperature, 1));
    Serial.print("T: ");
    Serial.println(temperature);
  }

}




