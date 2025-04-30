/*****************************************************************
File:        BMK52T016.cpp
Author:      BEST MODULES CORP.
Description: The Arduino communicates with the BMK52T016 for the IIC and gets the corresponding values  
Version:     V1.0.3   -- 2025-04-30
******************************************************************/
#include "BMK52T016.h"
/**********************************************************
Description: Constructor
Parameters:       intPin  ：INT Output pin connection with Arduino 
                  theWire : Wire object if your board has more than one I2C interface   
Return:      None  
Others:      None    
**********************************************************/
BMK52T016::BMK52T016(uint8_t intPin,TwoWire *theWire)
{
     _intPin = intPin;
     _wire = theWire;     
}
/**********************************************************
Description: Module Initial
Parameters:  i2c_addr : Module IIC address                         
Return:      void   
Others:      None
**********************************************************/
void BMK52T016::begin(uint8_t i2c_addr)
{
        pinMode(_intPin,INPUT);
        _wire->begin();   
        _i2caddr = i2c_addr;
}
/**********************************************************
Description: get FW Ver
Parameters:  void
Return:      Ver  
Others:      None
**********************************************************/
uint16_t BMK52T016::getFWVer()
{
    uint8_t KeyCMD[1]={0X0B};
    uint8_t fwBuff[2]={0};
    writeBytes(KeyCMD,1); 
    delay(5);
    readBytes(fwBuff,2);
    uint16_t fwValue = (fwBuff[0]<<8)+fwBuff[1];
    delay(10);
    return fwValue;
}
/**********************************************************
Description: get Key Status
Parameters:  void     
Return:      Returns the INT state  
             0:INT output low level  press
             1:INT output high level   unpress    
Others:      None
**********************************************************/
uint8_t BMK52T016::getINT()
{
     return (digitalRead(_intPin));
} 
/**********************************************************
Description: read Key Value
Parameters:  void      
Return:      keyValue:Variables for storing KeyValue data
                  0:No key is pressed
                  bit0=1 : key1 is pressed
                  bit1=1 : key2 is pressed
                  ...
                  bit15=1 : key16 is pressed
Others:      None
**********************************************************/
uint16_t BMK52T016::readKeyValue( )
{
    uint8_t KeyCMD[1]={0X0D};
    uint8_t tempL[1],tempH[1];
    writeBytes(KeyCMD,1); 
    delay(5);
    readBytes(tempL,REQUESTLEN1);
    delay(5);
    readBytes(tempH,REQUESTLEN1);
    uint16_t keyValue = (tempH[0]<<8)+tempL[0];
    delay(10);
    return keyValue;
}
/**********************************************************
Description: get 16  key trigger threshold
Parameters:  buff :Store the obtained 16 keystroke trigger threshold
                  buff[0]:Store the acquired KEY1 trigger threshold
                  buff[1]:Store the acquired KEY2 trigger threshold
                  ...........................................
                  buff[15]:Store the acquired KEY16 trigger threshold
Return:      void   
Others:      None
**********************************************************/
void BMK52T016::getThreshold(uint8_t buff[])
{
    writeBytes(TLCMD,1); 
    delay(5); 
    readBytes(buff,16); 
    delay(10);
} 
/**********************************************************
Description: Set the trigger threshold of 16 keys
Parameters:  buff :The triggering thresholds of 16 keys need to be set
                  buff[0]:Store the acquired KEY1 trigger threshold,range: 10-64
                  buff[1]:Store the acquired KEY2 trigger threshold,range: 10-64
                  ...........................................
                  buff[15]:Store the acquired KEY16 trigger threshold,range: 10-64  
Return:      Implementation status:
              0:Success 
              1:Fail
Others:      None
**********************************************************/
int BMK52T016::setThreshold(uint8_t buff[])
{
    uint16_t total = 0;
    uint8_t TXbuff[18] = {0};
    for(uint8_t i=0;i<16;i++)                    //sum
    {
        total = total + buff[i];
        TXbuff[i+1] = buff[i];
    }
    if(160 <= total && total<= 1024)//10~64
    {
        TXbuff[0] = 0XD8;
        TXbuff[17] = (uint8_t)total;
        writeBytes(TXbuff,18);
        delay(10);
        return SUCCESS;
    }
    else
    {
      return FAIL;
    }
}

/**********************************************************
Description: set Threshold Level of 16 keys
Parameters:  Level :Storing ThresholdLevel Data,range: 0-6        
Return:      Implementation status:
              0:Success 
              1:Fail   
Others:      threshold  = 10+level*9
**********************************************************/
int BMK52T016::setAllThresholdLevel(uint8_t level)
{
    if(level>6)
    {
        return FAIL;
    }
    uint8_t thr = 10+level*9;
    uint8_t sum = 16*thr;
    uint8_t STLCMD[] = {0xD8,thr,thr,thr,thr,thr,thr,thr,thr,thr,thr,thr,thr,thr,thr,thr,thr,sum};
    writeBytes(STLCMD,REQUESTLEN18);
    delay(10);
    return SUCCESS;
}

/**********************************************************
Description: set Led Mode
Parameters:  mode : 0 - flower key mode
                    1 - CMD Mode
Return:  void 
Others:  none     
**********************************************************/
void BMK52T016::setLedMode(uint8_t mode)
{
  if(mode <= 1)
  {
    uint8_t ledStatusCMD[3] = {0x02,0,0};  //Clear the LED status to avoid the light from turning on when setting the CMD mode
    writeBytes(ledStatusCMD,3); 
    uint8_t ledModeCMD[2] = {0x01,mode};
    writeBytes(ledModeCMD,2);
    delay(10); 
  }
}

/**********************************************************
Description: get Led Mode
Parameters:  void
Return:  Led Mode:
              0:flower key mode 
              1:CMD Mode 
Others:  none     
**********************************************************/
uint8_t BMK52T016::getLedMode()
{
  uint8_t value[1]={0};
  uint8_t ledModeCMD[1]={0X01};
  writeBytes(ledModeCMD,1);
  delay(5);
  readBytes(value,1);
  delay(10); 
  return value[0];
}

/**********************************************************
Description: write Led
Parameters:  data : led status(bit0~bit16：Key1~Key16)
            0: led off
            1: led on
Return:  void 
Others:  none     
**********************************************************/
void BMK52T016::writeLed(uint16_t data)
{
  uint8_t LedCMD[3] = {0x02,(uint8_t)data,(uint8_t)(data>>8)};
  writeBytes(LedCMD,3);
  delay(10); 
}

/**********************************************************
Description: read Led
Parameters:  void
Return:  led status(bit0~bit16：Key1~Key16)
            0: led off
            1: led on
Others:  none     
**********************************************************/
uint16_t BMK52T016::readLed()
{
  uint8_t value[2]={0};
  uint8_t LedCMD[1]={0X02};
  writeBytes(LedCMD,1);
  delay(5);
  readBytes(value,2);
  delay(10); 
  return ((uint16_t)value[1]<<8)+value[0];
}

/**********************************************************
Description: writeBytes
Parameters:  wbuf :the bytes sent
             wlen :the length of the data sent          
Return:      void  
Others:      None
**********************************************************/
void BMK52T016::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
    while (_wire->available() > 0)
    {
      _wire->read();
    }
    _wire->beginTransmission(_i2caddr);
    _wire->write(wbuf, wlen);
    _wire->endTransmission();
}
/**********************************************************
Description: read Bytes
Parameters:  rbuf :the bytes receive
             rlen :the length of the data receive
Return:      Implementation status:
              0:Success 
              1:Fail  
Others:      None
**********************************************************/
uint8_t BMK52T016::readBytes(uint8_t rbuf[], uint8_t rlen)
{
  _wire->requestFrom(_i2caddr, rlen);
  if(_wire->available()==rlen)
  {
      for (uint8_t i = 0; i < rlen; i++)
      {
        rbuf[i] = _wire->read();
      }
      return SUCCESS;
  }
  else
  {
       return FAIL;
  }
}
