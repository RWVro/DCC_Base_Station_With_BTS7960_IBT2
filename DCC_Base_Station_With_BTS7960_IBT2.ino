
//==================================== DCC_Base_Station_BT_V3 ======================================
//
//
//===============================  Board = ESP32 Dev Module (NodeMCU ESP32) ===========================
//
//
//====================== ESP32 with BTS7960 PWM H-Bridge motorcontrollermodule (IBT_2) ================

#define idStr "ESP32 Base Station Connected!"
#define LED 2

#define ENABLE 4                // to direction MotorShield pin 3-4 (Enable)
#define RPWM 5                  // to RPWM MotorShield pin 1
#define LPWM 15                 // to LPWM MotorShield pin 2


#include "BluetoothSerial.h"
#include <SPI.h>

#include "General.h"
#include "Speed_28.h"
#include "Speed_128.h"
#include "Functions.h"

//=================================  VARIABLES =================================

BluetoothSerial SerialBT;

String inputNbr;
bool stringComplete = false;

//String outputIdString;

bool speedOnOffSwitch = true;
//================================= Setup =======================================

void printFileName()
{
  Serial.println("");
  Serial.println(__FILE__" ("__DATE__") "__TIME__);
  Serial.println("");
}

void setup()
{
  Serial.begin(500000);
  delay(3000);
  printFileName();

  //=========== BlueTooth ===========
  SerialBT.begin("BASE_STATION");             //Bluetooth device name
  Serial.println("BlueTooth name = BASE_STATION");

  // sets the digital pins as output
  //=====================================
  pinMode(ENABLE, OUTPUT);           // to direction MotorShield pin 3-4 (Enable)
  pinMode(RPWM, OUTPUT);           // to RPWM MotorShield pin 1
  pinMode(LPWM, OUTPUT);          // to LPWM MotorShield pin 2

  digitalWrite(ENABLE, LOW);
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, LOW);

  pinMode(LED, OUTPUT);         // For test   Blue Led
  digitalWrite(LED, HIGH);

  delay(3000);
  Serial.println("BASE_STATION started!");
  digitalWrite(LED, LOW);
}

//===================================== functies om pulsen te maken ===============

/*
void basicPulses()
{
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
}
*/

//============================================ Settings ===================================

void settings()
{
  if (inputString.indexOf("ID?") > -1)          // indexOf is find string in string.
  {
    outputIdString = idStr;
  }
}

//===================================== functies om pulsen te maken ===============

void basicNormalPulses(int value)
{
  for (byte x = 0; x < value; x++)
  {
    dccZeroPulse();
    dccOnePulse();
    dccZeroPulse();
    dccOnePulse();
    dccZeroPulse();
    dccOnePulse();
    dccZeroPulse();
    dccOnePulse();
    dccZeroPulse();
    dccOnePulse();
  }
}

void basicZeroPulses()
{
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
  dccZeroPulse();
}

void SendDriveFunctions()
{
  if (loc128Set)
  {
    for (locArr128RowCnt = 0; locArr128RowCnt <= 20; locArr128RowCnt++)
    {
      locArray128Row = locArr128RowCnt;
      if (loc128Array[locArray128Row][4] == 128)
      {
        addr128Byte = loc128Array[locArray128Row][0];
        spd128InstrByte = loc128Array[locArray128Row][1];
        Spd128SpdStepByte = loc128Array[locArray128Row][2];
        error128Byte = loc128Array[locArray128Row][3];
        speed128Packet();
      }
    }
  }

  if (loc28Set)
  {
    for (locArr28RowCnt = 0; locArr28RowCnt <= 20; locArr28RowCnt++)
    {
      locArray28Row = locArr28RowCnt;
      if (loc28Array[locArray28Row][3] == 28)
      {
        addr28Byte = loc28Array[locArray28Row][0];
        spd28InstrByte  = loc28Array[locArray28Row][1];
        error28Byte = loc28Array[locArray28Row][2] ;
        speed28Packet();
      }
    }
  }
}

void SendFunctions()                      // Update functions
{
  functionGr1Packet();
  dccIdlePackets(1);
  functionGr2Packet();
  dccIdlePackets(1);
  functionCtrlPacket();
  dccIdlePackets(1);
}


//======================================= Main Program ==============================

void loop()
{
  dccIdlePackets(1);    

  SendDriveFunctions();

  SendFunctions();

  while (SerialBT.available())
  {
    char inChar = (char)SerialBT.read();
    if (inChar == '\n')
    {
      stringComplete = true;
      if (stringComplete)
      {
        if (inputString.indexOf("SET-") > -1)
        {
          settings();
          SerialBT.println("LOC:" + outputIdString);
          Serial.println("LOC: " + outputIdString);
          goto next;
        }
        if (inputString.indexOf("FG1") > -1)
        {
          Serial.println("-> " + inputString);
          findGroup1Function();
          SerialBT.println("FUNC:" + outputFunctionString);
          Serial.print("<- ");
          Serial.println("FUNC: " + outputFunctionString);
        }
        if (inputString.indexOf("FG2") > -1)
        {
          Serial.println("-> " + inputString);
          findGroup2Function();
          SerialBT.println("FUNC:" + outputFunctionString);
          Serial.print("<- ");
          Serial.println("FUNC: " + outputFunctionString);
        }
        if (inputString.indexOf("FMAN") > -1)
        {
          Serial.println("-> " + inputString);
          findControlFunction();
          SerialBT.println("FUNC:" + outputFunctionString);
          Serial.print("<- ");
          Serial.println("FUNC: " + outputFunctionString);
        }

        //=========================
        if (inputString.indexOf("SPEED_SWITCH_128") > -1)
        {
          Serial.println("-> " + inputString);
          speedOnOffSwitch = true;
          SerialBT.println("LOC:SPEED_SWITCH_128");
          Serial.println("<-LOC: SPEED_SWITCH_128");
        }
        if (inputString.indexOf("SPEED_SWITCH_28") > -1)
        {
          Serial.println("-> " + inputString);
          SerialBT.println("LOC:SPEED_SWITCH_28");
          Serial.println("<-LOC: SPEED_SWITCH_28");
          speedOnOffSwitch = false;
        }
        //=========================

        if (inputString.indexOf("TBL128") > -1)
        {
          Serial.println("-> " + inputString);
          findTable1128StepsCode();
          SerialBT.println("LOC:" + outputSpeed128String);
          Serial.print("<- ");
          Serial.println("LOC:" + outputSpeed128String);
        }
        if (inputString.indexOf("TBL28") > -1)
        {
          Serial.println("-> " + inputString);
          findTable28StepsCode();
          SerialBT.println("LOC:" + outputSpeed28String);
          Serial.print("<- ");
          Serial.println("LOC:" + outputSpeed28String);
        }

next:;
        SerialBT.println(outputString);
        inputString = "";
        outputString = "";
        stringComplete = false;
        inputNbr = "";
      }
      return;
    }
    inputString += inChar;
  }
}
