
#define RPWM 5                 // to RPWM MotorShield 
#define LPWM 15                 // to LPWM MotorShield 

String inputString;
String outputString;
String outputIdString;

int locArray128Row = 0;
int locArr128RowCnt = 0;

int locArray28Row = 0;
int locArr28RowCnt = 0;

byte addrByte = 3;

byte errorByte;

//============ To save 5 loc variables (0-4) in 20 loc addresses (0-19) in multidimensional array ===========

byte addr128Byte = 3;                       // Loc address
int locAddr128Byte = 0;

int loc128Array[20][5];    // [20] = locArray128Row    // [5] = locAddr128Byte, locSpdInstr128Byte, locSpdSpdSte28Byte, locError128Byte, spdFlg

//============ To save 5 loc variables (0-4) in 20 loc addresses (0-19) in multidimensional array ===========

byte addr28Byte = 3;                       // Loc address
int locAddr28Byte = 0;

int loc28Array[20][4];    // [20] = locArray28Row    // [4] = locAddr28Byte, locSpdInstr28Byte, locError28Byte, spd28Flg

//===================================== functions to make pulses =========================================

void dccOnePulse()
{
  // 58 Microseconds High
  digitalWrite(4, HIGH);        // Enable

  digitalWrite(LPWM, LOW);        // LPWM LOW
  digitalWrite(RPWM, HIGH);        // RPWM HIGH
  delayMicroseconds(57);

  // 58 Microseconds Low
  digitalWrite(RPWM, LOW);         // RPWM LOW
  digitalWrite(LPWM, HIGH);       // LPWM HIGH
  delayMicroseconds(57);
}

void dccZeroPulse()       // Zero pulse on CH A (Main Track)
{
  // 100 Microseconds High
  digitalWrite(4, HIGH);        // Enable + pulse

  digitalWrite(LPWM, LOW);        // LPWM LOW
  digitalWrite(RPWM, HIGH);        // RPWM HIGH
  delayMicroseconds(99);

  // 100 Microseconds Low
  digitalWrite(RPWM, LOW);         // RPWM LOW
  digitalWrite(LPWM, HIGH);       // LPWM HIGH
  delayMicroseconds(99);
}


//===================================== Binaire digitals to pulses ================

void dccBitsToPulses(boolean digit)
{
  if (digit == 1) {
    dccOnePulse();
  }
  else dccZeroPulse();
}

//========================================== Packet Instruction =======================================

void sendDccPreamble()
{
  for (byte k = 0; k < 14; k++)            // Service Preamble = 14 x dccOnePulse
  {
    dccOnePulse();
  }
}

//======================================== Loco Adres ========================================

void sendAddress()
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses(bitRead(addrByte, k));
  }
}

//============================================ Error Detection ============================

void sendErrorDetection()
{
  for (int k = 7; k >= 0; k--)
  {
    dccBitsToPulses( bitRead(errorByte, k));
  }
}

//=============================================== Idle pulse ==============================

void dccIdleOnes() 
{
  for (byte k = 0; k < 8; k++) 
  {
    dccOnePulse();
  }
}

void dccIdleZeros() 
{
  for (byte k = 0; k < 8; k++) 
  {
    dccZeroPulse();
  }
}

void dccIdlePackets(int value)
{
  for (int x = 0; x < value; x++)
  {
    sendDccPreamble();
    dccZeroPulse();     // start bit
    dccIdleOnes();
    dccZeroPulse();     // start bit
    dccIdleZeros();
    dccZeroPulse();     // start bit
    dccIdleOnes();
    dccOnePulse();      // end pulse
  }
}

//==================================================== Clear LocArray128 =================================

void clearAddressLocArray128()
{
  for (locArr128RowCnt = 0; locArr128RowCnt <= 20; locArr128RowCnt++)
  {
    locArray128Row = locArr128RowCnt;
    if (loc128Array[locArray128Row][0] == addr128Byte)                // Address found? clear detectionbyte
    {
      loc128Array[locArray128Row][4] = 0;                             // Clear detectionbyte (128)
    }
  }
}

//================================================== Clear LocArray28 =================================

void clearAddressLocArray28()
{
  for (locArr28RowCnt = 0; locArr28RowCnt <= 20; locArr28RowCnt++)
  {
    locArray28Row = locArr28RowCnt;
    if (loc28Array[locArray28Row][0] == addr28Byte)                // Address found? clear detectionbyte
    {
      loc28Array[locArray28Row][3] = 0;                             // Clear detectionbyte (128)
    }
  }
}
