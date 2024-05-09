/*****************************************************************
File:         setThresholdLevel.ino
Description:  Connect the IIC interface of the BMK52T016 to the Wire 
              of the Arduino and set the stageLeresholdLevel using
              setThresholdLevel,next, use getThresholdLevel to
              obtain the thresholdLevel via the I2C and then
              be displayed on the Serial monitor.
******************************************************************/
#include "BMK52T016.h"  
BMK52T016     BMK52(2,&Wire);//intPin,Please comment out this line of code if you don't use Wire
//BMK52T016     BMK52(22,&Wire1);//Please uncomment out this line of code if you use Wire1 on BMduino
//BMK52T016     BMK52(25,&Wire2);//Please uncomment out this line of code if you use Wire2 on BMduino
uint8_t Threshold[16]={0};
void setup() 
{
    BMK52.begin();
    Serial.begin(9600);
    BMK52.setAllThresholdLevel(5); // Set Thr to 28 (10+2*9)
    BMK52.getThreshold(Threshold);    
    for(uint8_t i=0;i<16;i++)
    {
       Serial.print("THR LEVEL");
       Serial.print(i);
       Serial.print("is:");
       Serial.println(Threshold[i]);  
    }
}
void loop() 
{ 

}
