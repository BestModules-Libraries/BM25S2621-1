/*****************************************************************
File:        BM25S2621-1.h
Author:      BEST MODULES CORP.
Description: Define classes and required variables
Version:     V1.0.1   -- 2025-05-06
******************************************************************/
#ifndef __BM25S2621_1_H
#define __BM25S2621_1_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#define CHECK_OK 0
#define CHECK_ERROR 1
typedef enum // Data storage address
{
  ADDR_ID = 0x00,
  ADDR_MOIS = 0x01,
  ADDR_TEMP = 0x02,
  ADDR_TEMPCALI = 0x03,
  ADDR_MOISCALI = 0x04,
  ADDR_EMPTYWATER = 0X05
} DATA_ADDR;

typedef enum // Function code
{
  FUNCODE_READ = 0x03,
  FUNCODE_WRITE = 0x06,
  FUNCODE_TEMPCALI = 0x0A,
  FUNCODE_MOISCALI = 0X0B,
  FUNCODE_EMPTYWATER = 0X0C
} FUNCODE;

class BM25S2621_1
{
public:
  BM25S2621_1(uint8_t staPin, HardwareSerial *theSerial = &Serial);
  BM25S2621_1(uint8_t staPin, uint8_t rxPin, uint8_t txPin);
  void begin();
  uint8_t setID(uint8_t currentID, uint8_t newID, bool broadcast = false);
  uint8_t restoreDefault(uint8_t id, bool broadcast = false);
  uint8_t getID();
  uint8_t readTemperature(uint8_t id, bool broadcast = false);
  uint8_t readMoisture(uint8_t id, bool broadcast = false);
  uint8_t readTemperatureAndMoisture(uint8_t id, uint8_t *temp, uint8_t *mois, bool broadcast = false);

private:
  uint8_t sendCommand(uint8_t *ptr_command, uint8_t *ptr_ack);
  uint8_t calibrateTemperature(uint8_t id, uint8_t currentTemp, bool broadcast = false);
  uint8_t calibrateMoisture(uint8_t id, uint8_t currentMois, bool broadcast = false);
  uint8_t emptyWaterCalibrate(uint8_t id, bool broadcast = false);
  void Modbus_CRCcaculate(unsigned char *ptr, int len, u8 *crc_H, u8 *crc_L);
  uint8_t RS485_request(uint8_t id, uint8_t funcode, uint16_t start, uint16_t len, uint8_t *readbuffer);
  
  uint8_t _rxPin;
  uint8_t _txPin;
  uint8_t _staPin;
  HardwareSerial *_hardSerial;
  SoftwareSerial *_softSerial;
};

#endif
