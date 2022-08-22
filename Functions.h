
//============================== Function Instruction ================================
//
//                                    // bit 7, 6, 5  = 100 or 101 function instruction
//
//                                    // bit 7, 6, 5 = 100 function instruction F1 - F4
//                                    // bit 7, 6, 5 = 101 function instruction F5 - F12
//                                    // bit 4 = 1 F5 - F8, bit4 = 0 F9 - F12

byte functGrp1Byte = 128;             // (1,0,0,0,0,0,0,0)   // bit0 = f0, bit1 = f1, ...
byte functGrp2Byte = 160;             // (1,0,1,0,0,0,0,0)

byte functCtrlByte = 222;             //(1,1,0,1,1,1,1,0) F13 - F20 (2 bytes instruction, next byte defined F13 - F20)
byte functDataByte = 0;                   //(0,0,0,0,0,0,0,0)         Databyte bit 0 = F13 ....

String outputFunctionString;

//========================================== Function instuction =========================

void sendFuncGr1Instruction()                       // F1 - F4
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses(bitRead(functGrp1Byte, k));
  }
}

void sendFuncGr2Instruction()                       // F5 - F12
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses(bitRead(functGrp2Byte, k));
  }
}

void sendFuncCtrlInstruction()                        // F13 - F20
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses(bitRead(functCtrlByte, k));
  }
}

void sendFuncDataInstruction()                          // F13 - F20
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses(bitRead(functDataByte, k));
  }
}

//============================================= Send Function Instruction Packets ====================

void functionGr1Packet()
{
  sendDccPreamble();
  dccZeroPulse();         // start bit
  sendAddress();
  dccZeroPulse();         // start bit
  sendFuncGr1Instruction();
  dccZeroPulse();         // start bit
  sendErrorDetection();
  dccOnePulse();          // end pulse
}

void functionGr2Packet()
{
  sendDccPreamble();
  dccZeroPulse();         // start bit
  sendAddress();
  dccZeroPulse();         // start bit
  sendFuncGr2Instruction();
  dccZeroPulse();         // start bit
  sendErrorDetection();
  dccOnePulse();          // end pulse
}

void functionCtrlPacket()
{
  sendDccPreamble();
  dccZeroPulse();         // start bit
  sendAddress();
  dccZeroPulse();         // start bit
  sendFuncCtrlInstruction();
  dccZeroPulse();         // start bit
  sendFuncDataInstruction();
  dccZeroPulse();
  sendErrorDetection();
  dccOnePulse();          // end pulse
}

//===============================  Update Function Packets =============================
void UpdateFuncGr1Packet()
{
  String tempStr = inputString;

  int colPos1 = tempStr.indexOf('=');                           // Search Address string
  tempStr.setCharAt(colPos1, '+');
  int colPos2 = tempStr.indexOf('!');
  String ADstr = tempStr.substring(colPos1 + 1, colPos2);
  addrByte = ADstr.toInt();                                     // Address to byte

  colPos1 = tempStr.indexOf('=');                                 // Search functGrp1Byte string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('$');
  String FU1str = tempStr.substring(colPos1 + 1, colPos2);
  functGrp1Byte = FU1str.toInt();                                 // Function functGrp1Byte string to byte

  colPos1 = tempStr.indexOf('=');                                 // Search ErrorByte functGrp1Byte string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('#');
  String EBstr = tempStr.substring(colPos1 + 1, colPos2);
  errorByte = EBstr.toInt();                                 // ErrorByte functGrp1Byte string to byte
}

void UpdateFuncGr2Packet()
{
  String tempStr = inputString;

  int colPos1 = tempStr.indexOf('=');                           // Search Address string
  tempStr.setCharAt(colPos1, '+');
  int colPos2 = tempStr.indexOf('!');
  String ADstr = tempStr.substring(colPos1 + 1, colPos2);
  addrByte = ADstr.toInt();                                     // Address to byte

  colPos1 = tempStr.indexOf('=');                                 // Search functGrp1Byte string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('$');
  String FU1str = tempStr.substring(colPos1 + 1, colPos2);
  functGrp2Byte = FU1str.toInt();                                 // Function functGrp1Byte string to byte

  colPos1 = tempStr.indexOf('=');                                 // Search ErrorByte functGrp1Byte string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('#');
  String EBstr = tempStr.substring(colPos1 + 1, colPos2);
  errorByte = EBstr.toInt();                                 // ErrorByte functGrp1Byte string to byte
}

void UpdateFuncCtrlPacket()
{
  String tempStr = inputString;

  int colPos1 = tempStr.indexOf('=');                           // Search Address string
  tempStr.setCharAt(colPos1, '+');
  int colPos2 = tempStr.indexOf('!');
  String ADstr = tempStr.substring(colPos1 + 1, colPos2);
  addrByte = ADstr.toInt();                                     // Address to byte

  colPos1 = tempStr.indexOf('=');                                 // Search functCtrlByte string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('$');
  String FUCtrlstr = tempStr.substring(colPos1 + 1, colPos2);
  functCtrlByte = FUCtrlstr.toInt();                                 // Function functCtrlByte string to byte

  colPos1 = tempStr.indexOf('=');                                 // Search functDataByte string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('%');
  String FuData = tempStr.substring(colPos1 + 1, colPos2);
  functDataByte = FuData.toInt();                                 // functDataByte string to byte

  colPos1 = tempStr.indexOf('=');                                 // Search ErrorByte functGrp1Byte string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('#');
  String EBstr = tempStr.substring(colPos1 + 1, colPos2);
  errorByte = EBstr.toInt();                                 // ErrorByte functGrp1Byte string to byte
}

//======================================= Group1 Functions ==============================

void findGroup1Function()
{
  if (inputString.indexOf("FUNCF0") > -1)
  {
    UpdateFuncGr1Packet();
    outputFunctionString = "Function F0= " + String (functGrp1Byte);
  }

  if (inputString.indexOf("FUNCF1") > -1)
  {
    UpdateFuncGr1Packet();
    outputFunctionString = "Function F1= " + String (functGrp1Byte);
  }

  if (inputString.indexOf("FUNCF2") > -1)
  {
    UpdateFuncGr1Packet();
    outputFunctionString = "Function F2= " + String (functGrp1Byte);
  }

  if (inputString.indexOf("FUNCF3") > -1)
  {
    UpdateFuncGr1Packet();
    outputFunctionString = "Function F3= " + String (functGrp1Byte);
  }

  if (inputString.indexOf("FUNCF4") > -1)
  {
    UpdateFuncGr1Packet();
    outputFunctionString = "Function F4= " + String (functGrp1Byte);
  }

  if (inputString.indexOf("FUNCALLGR1") > -1)
  {
    UpdateFuncGr1Packet();
    outputFunctionString = "Function ALLGR1= " + String (functGrp1Byte);
  }
  functionGr1Packet();
  functionGr1Packet();
  functionGr1Packet();    
}

//======================================= Group2 Functions ==============================

void findGroup2Function()
{    
  if (inputString.indexOf("FUNCF5") > -1)
  {
    UpdateFuncGr2Packet();
    outputFunctionString = "Function F5= " + String (functGrp2Byte);
  }

  if (inputString.indexOf("FUNCF6") > -1)
  {
    UpdateFuncGr2Packet();
    outputFunctionString = "Function F6= " + String (functGrp2Byte);
  }

  if (inputString.indexOf("FUNCF7") > -1)
  {
    //digitalWrite(2, HIGH);
    UpdateFuncGr2Packet();
    outputFunctionString = "Function F7= " + String (functGrp2Byte);
  }

  if (inputString.indexOf("FUNCF8") > -1)
  {
    UpdateFuncGr2Packet();
    outputFunctionString = "Function F8= " + String (functGrp2Byte);
  }

  if (inputString.indexOf("FUNCF9") > -1)
  {
    UpdateFuncGr2Packet();
    outputFunctionString = "Function F9= " + String (functGrp2Byte);
  }

  if (inputString.indexOf("FUNCF10") > -1)
  {
    UpdateFuncGr2Packet();
    outputFunctionString = "Function F10= " + String (functGrp2Byte);
  }

  if (inputString.indexOf("FUNCF11") > -1)
  {
    UpdateFuncGr2Packet();
    outputFunctionString = "Function F11= " + String (functGrp2Byte);
  }

  if (inputString.indexOf("FUNCF12") > -1)
  {
    UpdateFuncGr2Packet();
    outputFunctionString = "Function F12= " + String (functGrp2Byte);
  }

  if (inputString.indexOf("FUNCALLGR2") > -1)
  {
    UpdateFuncGr2Packet();
    outputFunctionString = "Function ALLGR2= " + String (functGrp2Byte);
  }
  functionGr2Packet();
  functionGr2Packet();
  functionGr2Packet();    
}

//======================================= Control Functions ==============================

void findControlFunction()
{
  if (inputString.indexOf("FUNCF13") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F13= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF14") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F14= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF15") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F15= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF16") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F16= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF17") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F17= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF18") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F18= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF19") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F19= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF20") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F20= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF21") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F21= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF22") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F22= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF23") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F23= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF24") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F24= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCF25") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function F25= " + String (functCtrlByte) + String (functDataByte);
  }

  if (inputString.indexOf("FUNCALLD") > -1)
  {
    UpdateFuncCtrlPacket();
    outputFunctionString = "Function ALLD= " + String (functCtrlByte);
    Serial.println(outputFunctionString);
  }
  functionCtrlPacket();
  functionCtrlPacket();
  functionCtrlPacket();    
}
