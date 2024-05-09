/*****************************************************************
File:         readKeyValue.ino
Description:  Connect the IIC interface of BMK52T016 to Wire of Arduino
              and connect the INT interface to PIN2 at the same time. 
              When a button is pressed, Arduino will get the key status 
              via the I2C and then be displayed on the Serial monitor.
******************************************************************/
#include "BMK52T016.h"
BMK52T016     BMK52(2,&Wire);//intPin,Please comment out this line of code if you don't use Wire
//BMK52T016     BMK52(22,&Wire1);//Please uncomment out this line of code if you use Wire1 on BMduino
//BMK52T016     BMK52(25,&Wire2);//Please uncomment out this line of code if you use Wire2 on BMduino

void setup() 
{
    BMK52.begin();
    Serial.begin(9600);
}

void loop() 
{  
    if(BMK52.getINT() == 0)
    {
         Serial.print("keyValue:");
         Serial.println(BMK52.readKeyValue(),BIN);
    }  
}
