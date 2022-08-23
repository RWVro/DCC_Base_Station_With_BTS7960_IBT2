# DCC_Base_Station_With_BTS7960_IBT2
 

I am sending command strings to the ESP32 from my Android tablet.

Look at the input strings in the Main Loop prog.

Speed Table 28 or speed table 128 is possible.

Also functions can be sent.

Examples of the input strings.
------------------------------

string sendStr;    
string commandStr;	// = "TBL128-Speed-Update>" for table128 or = "TBL28-Speed-Update>" for table28 or 
					= "FG1FUNCF0>" for function F0, = "FG1FUNCF1>" for function F1, ..... ;

int iad;
speedInstruct128 = 63;
int dirInt;
SendSpdStepByte128 = 0;
int errorSpeedByte = 0;

int speedInstruct = 63;

int functGrp1Byte = 128;	// (1,0,0,0,0,0,0,0)        F0 - F4 (bit0 = f0, bit1 = f1, ...) 


        //======================================================== Send Speed128 Settings to ESP32 =========================================

private void Update128SpeedSettings()
        {
            string saveStr = commandStr + "AD=" + iad + "!" + "SI=" + speedInstruct128 + "*" + "DF=" + dirInt + ":" +
                                    "SS=" + SendSpdStepByte128 + "%" + "EB=" + errorSpeedByte + "#";
            sendStr = saveStr;
        }

        // AD = Address
        // SI = Speed Instruction
        // DF = Diredtion flag
        // SS = SpeedStep
        // EB = Error Detection

        //======================================================== Send Speed28 Settings to ESP32 =========================================


        private void UpdateTBL28SpeedSettings()
        {
            string saveStr = commandStr + "AD=" + iad + "!" + "SI=" + speedInstruct + "*" + "DF=" + dirInt + ":" + "EB=" + errorSpeedByte + "#";
            sendStr = saveStr;
        }

        // AD = Address
        // SI = Speed Instruction
        // EB = Error Detection

 
       //======================================================== Send Function Settings to ESP32 =========================================

        public void UpdateESP32FuncGr1Settings()
        {
            string saveStr = commandStr + "AD=" + iad + "!" + "FB1=" + functGrp1Byte + "$" + "EB=" + errorFunctionByte + "#";
            sendStr = saveStr;
        }

        // AD  = Address
        // FB1 = Function Byte Group1
        // EB = Error Detection

