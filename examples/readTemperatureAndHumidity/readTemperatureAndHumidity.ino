/*************************************************
File:           readTemperatureAndHumidity.ino             
Description:    read the temperature and humidity measurement values of the module. 
Note:               
**************************************************/
#include "BM25S2621-1.h"
#define STATUS_PIN 2
#define RX_PIN 4
#define TX_PIN 5
BM25S2621_1 module(STATUS_PIN, RX_PIN, TX_PIN); // status: D2, Softeware serial: RX:D4 TX:D5
// BM25S2621_1 module(STATUS_PIN, &Serial); // status:D2, Hardware serial: Serial
// BM25S2621_1 module(22, &Serial1); // status:D22(STATUS1), Hardware serial: Serial1
// BM25S2621_1 module(25, &Seria2); // status:D25(STATUS2), Hardware serial: Serial2

uint8_t Target_ID = 3; //Target ID
uint8_t mois = 0; //Soil moisture value
uint8_t temp = 0; //Soil temperature value

void setup() 
{   
  Serial.begin(115200);
  module.begin();
  delay(100);
  if(module.setID(1,Target_ID,true) == 0)
  {
    Serial.print("setID succeed!  curID:");
    Serial.println(Target_ID);  
  }
  else 
  {
    Serial.println("setID fail!");
  }
}

void loop() 
{
  if (module.readTemperatureAndMoisture(Target_ID, &temp, &mois, false) == 0)
  {
    Serial.print("Temperature: ");
    Serial.print(temp);  
    Serial.print(" ℃");
    Serial.print("    Moisture: ");
    Serial.print(mois); 
    Serial.println(" %RH");
  }
  else 
  {
    Serial.println("get temperature and moisture fail!");
  }
  delay(1000);
}
