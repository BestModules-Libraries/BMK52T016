/*****************************************************************
File:         writeLED.ino
Description:  (1)Set the LED mode to CMD mode
              (2)Display LED flow light
              (3)Set the LED mode to key mode
******************************************************************/
#include "BMK52T016.h"
BMK52T016     BMK52(2,&Wire);//intPin,Please comment out this line of code if you don't use Wire
//BMK52T016     BMK52(22,&Wire1);//Please uncomment out this line of code if you use Wire1 on BMduino
//BMK52T016     BMK52(25,&Wire2);//Please uncomment out this line of code if you use Wire2 on BMduino
uint16_t led_data = 0;
void setup() 
{
  Serial.begin(9600);
  BMK52.begin();

  BMK52.setLedMode(1);   //Set the LED mode to CMD mode
  if(BMK52.getLedMode() == 1)
  {
    Serial.println("Set the LED mode to CMD mode : SUCCESS");
  }

  led_data = 0x0001;
  for(uint8_t i=0;i<16;i++) //LED flow light
  {
    BMK52.writeLed(led_data);
    led_data = led_data<<1;
    delay(100);
  }

  BMK52.setLedMode(0);   //Set the LED mode to Key mode
  if(BMK52.getLedMode() == 0)
  {
    Serial.println("Set the LED mode to Key mode : SUCCESS");
  }
}

void loop() 
{
  if(BMK52.getINT() == 0)
  {
      Serial.print("keyValue:");
      Serial.println(BMK52.readKeyValue(),BIN);
  }
}
