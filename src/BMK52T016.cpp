/*****************************************************************
File:        BMK52T016.cpp
Author:      BESTMODULES
Description: The Arduino communicates with the BMK52T016 for the 
	         IIC and gets the corresponding values  
History:         
V1.0.1   -- initial version；2023-01-06；Arduino IDE : v1.8.16
******************************************************************/
#include "BMK52T016.h"
/**********************************************************
Description: Constructor
Parameters:       intPin  ：INT Output pin connection with Arduino 
                  theWire : Wire object if your board has more than one I2C interface   
Return:          
Others:      
**********************************************************/
BMK52T016::BMK52T016(uint8_t intPin,TwoWire *theWire)
{
     _intPin = intPin;
     _wire = theWire;     
}
/**********************************************************
Description: Module Initial
Parameters:   i2c_addr : Module IIC address                         
Return:          
Others:      
**********************************************************/
void BMK52T016::begin(uint8_t i2c_addr)
{
        pinMode(_intPin,INPUT);
        _wire->begin();   
        _i2caddr = i2c_addr;
}
/**********************************************************
Description: get Key Status
Parameters:       
Return:      Returns the INT state  
             0:INT output low level  press
             1:INT output high level   unpress    
Others:      
**********************************************************/
uint8_t BMK52T016::getINT()
{
     return (digitalRead(_intPin));
} 
/**********************************************************
Description: read Key Value
Parameters:        
Return:      keyValue:Variables for storing KeyValue data
                  0:No key is pressed
                  bit0=1 : key1 is pressed
                  bit1=1 : key2 is pressed
                  ...
                  bit15=1 : key16 is pressed
Others:      
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
Description: get Threshold
Parameters:  buff :Storing 16 Threshold and Checksum Data         
Return:         
Others:      
**********************************************************/
void BMK52T016::getThreshold(uint8_t buff[])
{
    writeBytes(TLCMD,1); 
    delay(5); 
    readBytes(buff,16); 
    delay(10);
} 
/**********************************************************
Description: set Threshold
Parameters:  Buff :Storing Threshold Data,10~64  
Return:        
Others:      
**********************************************************/
int BMK52T016::setThreshold(uint8_t buff[])
{
    uint16_t total = 0;
    for(uint8_t i=1;i<17;i++)                    //sum
    {
        total = total + buff[i];
    }
    if(160 <= total && total<= 1024)//10~64
    {
        buff[0] = 0XD8;
        buff[17] = (uint8_t)total;
        writeBytes(buff,18);
        delay(10);
        return SUCCESS;
    }
    else
    {
      return FAIL;
    }
}
/**********************************************************
Description: set Threshold Level
Parameters:  Level :Storing ThresholdLevel Data,threshold 
                             = 10+thresholdlevel*9,recommended range: 0-6,         
Return:      0:Success 1:Fail   
Others:      
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
Description: writeBytes
Parameters:  wbuf :the bytes sent
             wlen :the length of the data sent          
Return:        
Others:      
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
Return:        
Others: 
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
