#include <Arduino.h>

#include "HUSKYLENS.h"

HUSKYLENS huskylens;

void setup_husky()
{
  Serial1.begin(9600);
  if (!huskylens.begin(Serial1))
  {
    Serial.println("could not setup husky");
  }
  
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}


void loop_husky()
{
  if (huskylens.available())
  {
    HUSKYLENSResult result = huskylens.read();
    printResult(result);
  }
}