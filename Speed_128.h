
byte spd128InstrByte = 63;                  // Advanced Speedinstruction 63

//============================== Speed 128 SpeedValue =======================================

byte Spd128SpdStepByte;

bool forw128Flg = true;
int forw128Int = 1;

//================================= Error Detection ================================

byte error128Byte;

String outputSpeed128String;

//========================================= Loc Array ==============================

bool loc128Set = false;

int locSpdInstr128Byte = 0;
int locSpdSpdSte28Byte = 0;
int locError128Byte = 0;
int spd128Flg = 0;

//========================================== Packet Instruction =======================================

void send128Preamble()
{
  for (byte k = 0; k < 14; k++)            // Preamble = 14 x dccOnePulse
  {
    dccOnePulse();
  }
}

void send128Address()
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses(bitRead(addr128Byte, k));
  }
}

void send128SpeedInstruction()
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses(bitRead(spd128InstrByte, k));
  }
}

void send128SpeedStep()
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses(bitRead(Spd128SpdStepByte, k));
  }
}

void send128ErrorDetection()
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses( bitRead(error128Byte, k));
  }
}

//============================================== Update Speed settings ===============================

void UpdateSettings128SpeedPacket()
{
  String tempStr = inputString;

  int colPos1 = tempStr.indexOf('=');                           // Search Address string
  tempStr.setCharAt(colPos1, '+');
  int colPos2 = tempStr.indexOf('!');
  String ADstr = tempStr.substring(colPos1 + 1, colPos2);
  addr128Byte = ADstr.toInt();                                     // Address to byte

  colPos1 = tempStr.indexOf('=');                               // Search Speed Instruction string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('*');
  String SIstr = tempStr.substring(colPos1 + 1, colPos2);
  spd128InstrByte = SIstr.toInt();                                     // Speed Instruction  to int

  colPos1 = tempStr.indexOf('=');                               // Search DirectionFlag string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf(':');
  String DFstr = tempStr.substring(colPos1 + 1, colPos2);

  if (DFstr == "1")
  {
    forw128Flg = true;
    forw128Int = 1;
  }

  if (DFstr == "0")
  {
    forw128Flg = false;
    forw128Int = 0;
  }

  colPos1 = tempStr.indexOf('=');                                 // Search Backward SpeedStepByte string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('%');
  String SSstr = tempStr.substring(colPos1 + 1, colPos2);
  Spd128SpdStepByte = SSstr.toInt();                                 // Backward SpeedStepByte string to byte

  colPos1 = tempStr.indexOf('=');                                 // Search ErrorByte SpeedStepByte string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('#');
  String EBstr = tempStr.substring(colPos1 + 1, colPos2);
  error128Byte = EBstr.toInt();                                 // ErrorByte SpeedStepByte string to byte

  clearAddressLocArray128();
  clearAddressLocArray28();
  Serial.println("Settings128_Updated " + String (addr128Byte) + String (" ") + String (spd128InstrByte) + String (" ") + String (Spd128SpdStepByte)+ String (" ") + String (error128Byte));
}

//============================================= Send 128 Speed Instruction Packets ====================

void speed128Packet()
{
  send128Preamble();
  dccZeroPulse();         // start bit
  send128Address();
  dccZeroPulse();         // start bit
  send128SpeedInstruction();
  dccZeroPulse();         // start bit
  send128SpeedStep();
  dccZeroPulse();         // start bit
  send128ErrorDetection();
  dccOnePulse();          // end pulse
}


//================================================== 128 SpeedSteps ==================================

void findTable1128StepsCode()
{
  if (inputString.indexOf("Forward>") > -1)
  {
    UpdateSettings128SpeedPacket();
    outputSpeed128String = "Forward_128";
  }

  if (inputString.indexOf("Backward>") > -1)
  {
    UpdateSettings128SpeedPacket();
    outputSpeed128String = "Backward_128";
  }

  if (inputString.indexOf("Stop>") > -1)
  {
    UpdateSettings128SpeedPacket();
    if (forw128Flg)
    {
      Spd128SpdStepByte = 128;                   // Lights must stay forwards
      outputSpeed128String = "FWStop_128";
    }
    else
    {
      Spd128SpdStepByte = 0;                   // Lights must stay backwards
      outputSpeed128String = "BWStop_128";
    }
  }

  //============================================= Speed Update ================================

  if (inputString.indexOf("TBL128-Speed-Update>") > -1)
  {
    UpdateSettings128SpeedPacket();
    if (forw128Flg)
    {
      outputSpeed128String = "FWSpeed-128";
    }

    if (!forw128Flg)
    {
      outputSpeed128String = "BWSpeed-128";
    }
  }

next:;
  loc128Set = true;
  locArray128Row = addr128Byte;
  loc128Array[locArray128Row][0] = addr128Byte;
  loc128Array[locArray128Row][1] = spd128InstrByte;
  loc128Array[locArray128Row][2] = Spd128SpdStepByte;
  loc128Array[locArray128Row][3] = error128Byte;
  loc128Array[locArray128Row][4] = 128;
  //Serial.println("locArray128 " + String (loc128Array[locArray128Row][0]) + String (" ") + String (loc128Array[locArray128Row][1]) + String (" ") + String (loc128Array[locArray128Row][2]) + String (" ") + String (loc128Array[locArray128Row][3]) + String (" ") + String (loc128Array[locArray128Row][4]));

  speed128Packet();
}
