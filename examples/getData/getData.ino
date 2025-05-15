/*************************************************
File:           getData.ino
Description:    Read the temperature and humidity measurement values of the specified module
Note:           Before reading module data, different IDs need to be assigned to the module
**************************************************/
#include "BM25S2621-1.h"
#define STATUS_PIN 2
#define RX_PIN 4
#define TX_PIN 5
BM25S2621_1 module(STATUS_PIN, RX_PIN, TX_PIN); // status: D2, Softeware serial: RX:D4 TX:D5
// BM25S2621_1 module(STATUS_PIN, &Serial); // status:D2, Hardware serial: Serial
// BM25S2621_1 module(22, &Serial1); // status:D22(STATUS1), Hardware serial: Serial1
// BM25S2621_1 module(25, &Seria2); // status:D25(STATUS2), Hardware serial: Serial2

uint8_t mois1, mois2, mois3; // Soil moisture value
uint8_t temp1, temp2, temp3; // Soil temperature value
uint8_t module1_id = 2;      // Module 1 ID
uint8_t module2_id = 3;      // Module 2 ID
uint8_t module3_id = 4;      // Module 3 ID

void setup()
{
  Serial.begin(115200);
  module.begin();
  /* Uncomment when setting the ID, only one module can be connected at a time.
  After all settings are completed, comment out the command and then attach all modules to the RS485 bus.*/
  /*if (module.setID(1, module1_id, true) == 0)
  {
    Serial.print("curID:");
    Serial.println(module1_id);
  }
  else
  {
    Serial.println("setID error");
  }
  delay(300);
  */
}
void loop()
{
  if (module.readTemperatureAndMoisture(module1_id, &temp1, &mois1, false) == 0)
  {
    Serial.print("curTemp1:");
    Serial.println(temp1);
    Serial.print("curMois1:");
    Serial.println(mois1);
  }
  else
  {
    Serial.println("module1 get temperature and moisture error");
  }
  Serial.println(); 
  delay(10);

  if (module.readTemperatureAndMoisture(module2_id, &temp2, &mois2, false) == 0)
  {
    Serial.print("curTemp2:");
    Serial.println(temp2);
    Serial.print("curMois2:");
    Serial.println(mois2);
  }
  else
  {
    Serial.println("module2 get temperature and moisture error");
  }
  Serial.println(); 
  delay(10);

  if (module.readTemperatureAndMoisture(module3_id, &temp3, &mois3, false) == 0)
  {
    Serial.print("curTemp3:");
    Serial.println(temp3);
    Serial.print("curMois3:");
    Serial.println(mois3);
  }
  else
  {
    Serial.println("module3 get temperature and moisture error");
  }
  Serial.println(); 
  delay(1000);
}
