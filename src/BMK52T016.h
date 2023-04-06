/*****************************************************************
File:             BMK52T016.h
Author:           BESTMODULES
Description:      Define classes and required variables
History：         
V1.0.1   -- initial version；2023-01-06；Arduino IDE : v1.8.16
******************************************************************/
#ifndef _BMK52T016_H_
#define _BMK52T016_H_

#include <Wire.h>
#include <Arduino.h>

#define   SUCCESS           (0)
#define   FAIL              (1)
#define   BMK52T016_IICADDR (0x70)
#define   REQUESTLEN1       (1)
#define   REQUESTLEN16       (16)
#define   REQUESTLEN18       (18)      


class BMK52T016
{
    public:
    BMK52T016(uint8_t intPin,TwoWire *theWire = &Wire);
    void begin(uint8_t i2c_addr = BMK52T016_IICADDR);
    uint8_t getINT();
    uint16_t readKeyValue();
    void getThreshold(uint8_t buff[]);
    int setThreshold(uint8_t buff[]);
    int setAllThresholdLevel(uint8_t level = 2);
    
    private:
    void writeBytes(uint8_t wbuf[], uint8_t wlen);
    uint8_t readBytes(uint8_t rbuf[], uint8_t rlen);
    TwoWire *_wire;
    uint8_t _intPin;
    uint8_t _i2caddr;
    uint8_t TLCMD[1] = {0xD8};//threshold CMD
};
#endif
