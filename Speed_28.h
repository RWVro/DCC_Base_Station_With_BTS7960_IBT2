
//============================== Speed 28 Instruction ================================

byte spd28InstrByte = 96;             // Speedinstruction 96 = speed 0 forward, 64 = speed 0 reverse

//============================== Speed 28 SpeedValue =======================================

bool forw28Flg = true;
int forw28Int = 1;

//================================= Error Detection ================================

byte error28Byte;
bool error28Detection[9];

String outputSpeed28String;

//========================================= Loc Array ==============================

bool loc28Set = false;

int locSpdInstr28Byte = 0;
int locError28Byte = 0;
int spd28Flg = 0;

//========================================== Packet Instruction =======================================

void send28Preamble()
{
  for (byte k = 0; k < 14; k++)            //  Preamble = 14 x dccOnePulse
  {
    dccOnePulse();
  }
}

void send28Address()
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses(bitRead(addr28Byte, k));
  }
}

void send28SpeedInstruction()
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses(bitRead(spd28InstrByte, k));
  }
}

void send28ErrorDetection()
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses( bitRead(error28Byte, k));
  }
}

//============================================== Update Speed settings ===============================

void UpdateSettings28SpeedPacket()
{
  String tempStr = inputString;

  int colPos1 = tempStr.indexOf('=');                           // Search Address string
  tempStr.setCharAt(colPos1, '+');
  int colPos2 = tempStr.indexOf('!');
  String ADstr = tempStr.substring(colPos1 + 1, colPos2);
  addr28Byte = ADstr.toInt();                                     // Address to byte

  colPos1 = tempStr.indexOf('=');                               // Search Speed Instruction string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('*');
  String SIstr = tempStr.substring(colPos1 + 1, colPos2);
  spd28InstrByte = SIstr.toInt();                                     // Speed Instruction  to int

  colPos1 = tempStr.indexOf('=');                               // Search DirectionFlag string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf(':');
  String DFstr = tempStr.substring(colPos1 + 1, colPos2);

  if (DFstr == "1")
  {
    forw28Flg = true;
    forw28Int = 1;
  }

  if (DFstr == "0")
  {
    forw28Flg = false;
    forw28Int = 0;
  }

  colPos1 = tempStr.indexOf('=');                                 // Search ErrorByte SpeedStepByte string
  tempStr.setCharAt(colPos1, '+');
  colPos2 = tempStr.indexOf('#');
  String EBstr = tempStr.substring(colPos1 + 1, colPos2);
  error28Byte = EBstr.toInt();                                 // ErrorByte SpeedStepByte string to byte

  clearAddressLocArray128();
  clearAddressLocArray28();
}

//============================================= Send 28 Speed Instruction Packets ==========

void speed28Packet()
{
  send28Preamble();
  dccZeroPulse();         // start bit
  send28Address();
  dccZeroPulse();         // start bit
  send28SpeedInstruction();
  dccZeroPulse();         // start bit
  send28ErrorDetection();
  dccOnePulse();          // end pulse
}

//============================================ 28 SpeedSteps ==========================

void findTable28StepsCode()
{
  if (inputString.indexOf("Forward>") > -1)
  {
    UpdateSettings28SpeedPacket();
    outputSpeed28String = "Forward_28";
   }

  if (inputString.indexOf("Backward>") > -1)
  {
    UpdateSettings28SpeedPacket();
    outputSpeed28String = "Backward_28";
  }

  if (inputString.indexOf("Stop>") > -1)
  {
    UpdateSettings28SpeedPacket();
    if (forw28Flg)
    {
      outputSpeed28String = "FWStop_28";
    }
    else
    {
      outputSpeed28String = "BWStop_28";
    }
  }

  //============================================= Speed Up ================================

  if (inputString.indexOf("TBL28-Speed-Update>") > -1)
  {
    UpdateSettings28SpeedPacket();          // spdInstrByte is set
    if (forw28Flg)
    {
      outputSpeed28String = "FWSpeed-28";
    }

    if (!forw28Flg)
    {
      outputSpeed28String = "BWSpeed-28";
    }
  }

  //============================================== Speed Down ==============================

  if (inputString.indexOf("Speed-Down>") > -1)
  {
    UpdateSettings28SpeedPacket();
    if (forw28Flg)
    {
      outputSpeed28String = "FWSpeed-Down";
      Serial.println ("FWSpeed-Down28");
    }

    if (!forw28Flg)
    {
      outputSpeed28String = "BWSpeed-Down";
      Serial.println ("BWSpeed-Down28");
    }
  }
next:;
  loc28Set = true;
  locArray28Row = addr28Byte;
  loc28Array[locArray28Row][0] = addr28Byte;
  loc28Array[locArray28Row][1] = spd28InstrByte;
  loc28Array[locArray28Row][2] = error28Byte;
  loc28Array[locArray28Row][3] = 28;
  speed28Packet();
}
