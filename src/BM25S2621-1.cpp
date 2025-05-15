/*****************************************************************
File:        BM25S2621-1.cpp
Author:      BEST MODULES CORP.
Description: Communication and operation function with module
Version:     V1.0.1   -- 2025-05-06
******************************************************************/
#include "BM25S2621-1.h"

/*************************************************
Description: Constructor
Parameters：
             staPin : control pin of rs485
             *theSerial : point of Serial(default : Serial1)

Return: void
Others: None
*************************************************/
BM25S2621_1::BM25S2621_1(uint8_t staPin, HardwareSerial *theSerial)
{
  _hardSerial = theSerial;
  _staPin = staPin;
  _softSerial = NULL;
}
/**********************************************************
Description:  Constructor
Parameters: staPin: control pin of rs485
            rxPin: Receiver signal pin of the UART
            txPin: Send signal pin of UART
Return: None
Others: None
**********************************************************/
BM25S2621_1::BM25S2621_1(uint8_t staPin, uint8_t rxPin, uint8_t txPin)
{
  _hardSerial = NULL;
  _staPin = staPin;
  _rxPin = rxPin;
  _txPin = txPin;
  _softSerial = new SoftwareSerial(_rxPin, _txPin);
}
/*************************************************
Description: Enable the Serial port of the Module
Parameters: void
Return: void
Others: None
*************************************************/
void BM25S2621_1::begin()
{
  if (_softSerial != NULL)
  {
    _softSerial->begin(9600);
    _softSerial->SetTxStatus(ENABLE);
    _softSerial->SetRxStatus(ENABLE);
    _softSerial->setTimeout(20); // set softwareUart boudrate
    }
  else
  {
    _hardSerial->begin(9600);
    _hardSerial->SetTxStatus(ENABLE);
    _hardSerial->SetRxStatus(ENABLE);
    _hardSerial->setTimeout(20); // set hardwareUart timeout
  }
  pinMode(_staPin, OUTPUT);
}

/*************************************************
Description: set module's ID
Parameters: currentID : current ID of module
            newID : new ID of module
            broadcast : choose whether it is a broadcast command(default : false)
Return: Communication status  0:Susses 1:Fail
Others: None
*************************************************/
uint8_t BM25S2621_1::setID(uint8_t currentID, uint8_t newID, bool broadcast)
{

  uint8_t readbuffer[9];
  uint8_t module_id;
  if (broadcast)
    module_id = 0xff;
  else
    module_id = currentID;
  if (RS485_request(module_id, FUNCODE_WRITE, ADDR_ID, newID, readbuffer) == CHECK_OK)
  {
    return CHECK_OK;
  }
  else
    return CHECK_ERROR;
}

/*************************************************
Description: Reset module ID to 1
Parameters: id : current ID of module
            broadcast : choose whether it is a broadcast command(default : false)
Return: Communication status  0:Susses 1:Fail
Others: None
*************************************************/
uint8_t BM25S2621_1::restoreDefault(uint8_t id, bool broadcast)
{
  uint8_t readbuffer[9];
  uint8_t module_id;
  if (broadcast)
    module_id = 0xff;
  else
    module_id = id;
  if (RS485_request(module_id, FUNCODE_WRITE, ADDR_ID, 1, readbuffer) == CHECK_OK)
  {
    delay(5);
    if (RS485_request(module_id, FUNCODE_WRITE, ADDR_TEMPCALI, 0x0000, readbuffer) == CHECK_OK)
    {
      delay(5);
      if (RS485_request(module_id, FUNCODE_WRITE, ADDR_MOISCALI, 0x0000, readbuffer) == CHECK_OK)
      {
        return CHECK_ERROR;
      }
      else
        return CHECK_OK;
    }
    else
      return CHECK_OK;
  }
  else
    return CHECK_OK;
}



/*************************************************
Description: get module's ID
Parameters: none
Return: 1~254:module's ID
        0:communication failure
Others: None
*************************************************/
uint8_t BM25S2621_1::getID()
{
  uint8_t readbuffer[9];
  if (RS485_request(0xff, FUNCODE_READ, ADDR_ID, 1, readbuffer) == CHECK_OK)
  {
    //        this->ID = readbuffer[4];
    return readbuffer[4];
  }
  else
  {
    return CHECK_ERROR;
  }
}

/*************************************************
Description: Read module's temperature measurement value
Parameters: id : current ID of module
            broadcast : choose whether it is a broadcast command(default : false)
Return: Module's temperature measurement value
Others: None
*************************************************/
uint8_t BM25S2621_1::readTemperature(uint8_t id, bool broadcast)
{
  uint8_t readbuffer[9];
  uint8_t module_id;
  if (broadcast)
    module_id = 0xff;
  else
    module_id = id;
  if (RS485_request(module_id, FUNCODE_READ, ADDR_TEMP, 1, readbuffer) == CHECK_OK)
    return (readbuffer[3] << 8) | readbuffer[4];
  else
    return CHECK_ERROR;
}

/*************************************************
Description: Read module's moisture measurement value
Parameters: id : current ID of module
            broadcast : choose whether it is a broadcast command(default : false)
Return: Module's moisture measurement value
Others: None
*************************************************/
uint8_t BM25S2621_1::readMoisture(uint8_t id, bool broadcast)
{
  uint8_t readbuffer[9];
  uint8_t module_id;
  if (broadcast)
    module_id = 0xff;
  else
    module_id = id;
  if (RS485_request(module_id, FUNCODE_READ, ADDR_MOIS, 1, readbuffer) == CHECK_OK)
    return (readbuffer[3] << 8) | readbuffer[4];
  else
    return CHECK_ERROR;
}

/*************************************************
Description: Read module's moisture and temperature measurement value
Parameters: id : current ID of module
            *temp : Address for storing module's temperature measurement value
            *mois : Address for storing module's moisture measurement value
            broadcast : choose whether it is a broadcast command(default : false)
Return: Communication status  0:Susses 1:Fail
Others: None
*************************************************/
uint8_t BM25S2621_1::readTemperatureAndMoisture(uint8_t id, uint8_t *temp, uint8_t *mois, bool broadcast)
{
  uint8_t readbuffer[15];
  uint8_t module_id;
  if (broadcast)
    module_id = 0xff;
  else
    module_id = id;
  if (RS485_request(module_id, FUNCODE_READ, ADDR_MOIS, 2, readbuffer) == CHECK_OK)
  {
    *mois = (readbuffer[3] << 8) | readbuffer[4];
    *temp = (readbuffer[5] << 8) | readbuffer[6];
    return CHECK_OK;
  }
  else
    return CHECK_ERROR;
}

/*************************************************
Description: send specified commands to the module
Parameters: *ptr_command : store the array address of the command
            *ptr_ack : The array address of the storage module's response
Return: Communication status  0:Susses 1:Fail
Others: None
*************************************************/
uint8_t BM25S2621_1::sendCommand(uint8_t *ptr_command, uint8_t *ptr_ack)
{
  uint8_t send_id = ptr_command[0];
  uint8_t send_funcode = ptr_command[1];
  uint16_t send_start_addr = (ptr_command[2] << 8) | ptr_command[3];
  uint16_t send_len = (ptr_command[4] << 8) | ptr_command[5];
  if (RS485_request(send_id, send_funcode, send_start_addr, send_len, ptr_ack) == CHECK_OK)
  {
    return CHECK_OK;
  }
  else
    return CHECK_ERROR;
}

/*************************************************
Description: calibrate module's temperature
Parameters: id : current ID of module
            currentTemp : current soil temperature
            broadcast : choose whether it is a broadcast command(default : false)
Return: Communication status  0:Susses 1:Fail
Others: None
*************************************************/
uint8_t BM25S2621_1::calibrateTemperature(uint8_t id, uint8_t currentTemp, bool broadcast)
{
  uint8_t readbuffer[9];
  uint8_t module_id;
  if (broadcast)
    module_id = 0xff;
  else
    module_id = id;
  if (RS485_request(module_id, FUNCODE_TEMPCALI, 0, currentTemp, readbuffer) == CHECK_OK)
  {
    return CHECK_OK;
  }
  else
    return CHECK_ERROR;
}

/*************************************************
Description: calibrate module's moisture
Parameters: id : current ID of module
            currentMois : current soil moisture
            broadcast : choose whether it is a broadcast command(default : false)
Return: Communication status  0:Susses 1:Fail
Others: None
*************************************************/
uint8_t BM25S2621_1::calibrateMoisture(uint8_t id, uint8_t currentMois, bool broadcast)
{
  uint8_t readbuffer[9];
  uint8_t module_id;
  if (broadcast)
    module_id = 0xff;
  else
    module_id = id;
  if (RS485_request(module_id, FUNCODE_MOISCALI, 0, currentMois, readbuffer) == CHECK_OK)
  {
    return CHECK_OK;
  }
  else
    return CHECK_ERROR;
}

/*************************************************
Description: empty water calibration
Parameters: id : current ID of module
            broadcast : choose whether it is a broadcast command(default : false)
Return: Communication status  0:Susses 1:Fail
Others: None
*************************************************/
uint8_t BM25S2621_1::emptyWaterCalibrate(uint8_t id, bool broadcast)
{
  uint8_t readbuffer[9];
  uint8_t module_id;
  if (broadcast)
    module_id = 0xff;
  else
    module_id = id;
  if (RS485_request(module_id, FUNCODE_EMPTYWATER, 0, 0, readbuffer) == CHECK_OK)
  {
    return CHECK_OK;
  }
  else
    return CHECK_ERROR;
}
/*************************************************
  Description: Modbus CRC caculate
  Parameters: ptr : data
              len : data length
              crc_H:crc Hight
              crc_L:crc Low
  Return: void
  Others: None
*************************************************/
void BM25S2621_1::Modbus_CRCcaculate(unsigned char *ptr, int len, u8 *crc_H, u8 *crc_L)
{
  unsigned int i;
  unsigned short crc = 0xFFFF;

  while (len--)
  {
    crc ^= *ptr++;
    for (i = 0; i < 8; ++i)
    {
      if (crc & 1)
        crc = (crc >> 1) ^ 0xA001;
      else
        crc = (crc >> 1);
    }
  }
  *crc_L = crc & 0x00ff;
  *crc_H = (crc >> 8) & 0x00ff;
}
/*************************************************
  Description: RS485 request
  Parameters: id : current ID of module
              funcode : choose whether it is a broadcast command(default : false)
              start：Data address
              len：data length
              readbuffer：Stored read data
  Return: Communication status  0:Success 1:Fail
  Others: None
*************************************************/
uint8_t BM25S2621_1::RS485_request(uint8_t id, uint8_t funcode, uint16_t start, uint16_t len, uint8_t *readbuffer)
{
  uint16_t rev_num = 0; // Number of received data
  uint8_t sendbuffer[9];
  uint8_t crc_check_l, crc_check_h;
  memset(sendbuffer, 0x00, sizeof(sendbuffer));
  sendbuffer[0] = id;
  sendbuffer[1] = funcode;
  sendbuffer[2] = start >> 8;
  sendbuffer[3] = start & 0x00ff;
  sendbuffer[4] = len >> 8;
  sendbuffer[5] = len & 0x00ff;
  Modbus_CRCcaculate(sendbuffer, 6, &sendbuffer[7], &sendbuffer[6]);

  digitalWrite(_staPin, HIGH);
  if (_softSerial != NULL)
  {
    for (uint8_t a = 0; a < 8; a++)
    {
      _softSerial->write(sendbuffer[a]);
    }
    _softSerial->flush();
  }
  else
  {
    for (uint8_t a = 0; a < 8; a++)
    {
      _hardSerial->write(sendbuffer[a]);
    }
    _hardSerial->flush();
  }
  digitalWrite(_staPin, LOW);

  if (funcode == FUNCODE_READ)
    rev_num = 5 + len * 2;
  else if (funcode == FUNCODE_WRITE)
    rev_num = 8;
  else if (funcode == FUNCODE_TEMPCALI || funcode == FUNCODE_MOISCALI || funcode == FUNCODE_EMPTYWATER)
    rev_num = 7;

  if (_softSerial != NULL)
  {
    _softSerial->readBytes(readbuffer, rev_num);
  }
  else
  {
    _hardSerial->readBytes(readbuffer, rev_num);
  }

  Modbus_CRCcaculate(readbuffer, rev_num - 2, &crc_check_h, &crc_check_l);

  if (crc_check_h == readbuffer[rev_num - 1] && crc_check_l == readbuffer[rev_num - 2])
  {
    if (funcode == FUNCODE_EMPTYWATER)
    {
      if (readbuffer[4] == 1 || readbuffer[4] == 0xAA)
        return CHECK_OK;
      else
        return CHECK_ERROR;
    }
    else
      return CHECK_OK;
  }
  else
    return CHECK_ERROR;
}
