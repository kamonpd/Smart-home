#include <IRremote.h>
#include <IRremoteInt.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#include <FujitsuHeatpumpIR.h>
#include <PanasonicCKPHeatpumpIR.h>
#include <PanasonicHeatpumpIR.h>
#include <CarrierHeatpumpIR.h>
#include <MideaHeatpumpIR.h>
#include <MitsubishiHeatpumpIR.h>
#include <SamsungHeatpumpIR.h>
#include <SharpHeatpumpIR.h>
#include <DaikinHeatpumpIR.h>
#include <MitsubishiHeavyHeatpumpIR.h>
#include <MitsubishiSEZKDXXHeatpumpIR.h>
#include <HyundaiHeatpumpIR.h>
#include <HisenseHeatpumpIR.h>
#include <GreeHeatpumpIR.h>  
#include <ToshibaHeatpumpIR.h>
#include <ToshibaDaiseikaiHeatpumpIR.h>
#include <IVTHeatpumpIR.h>
#include <HitachiHeatpumpIR.h>
#include <BalluHeatpumpIR.h>
#include <AUXHeatpumpIR.h>


//SoftwareSerial nodeIR(3,2);
IRsend irsend;
IRSenderPWM irSender(9);  

HeatpumpIR *heatpumpIR[] = {new PanasonicCKPHeatpumpIR(), new PanasonicDKEHeatpumpIR(), new PanasonicJKEHeatpumpIR(),new PanasonicNKEHeatpumpIR(),
new PanasonicLKEHeatpumpIR(),new CarrierNQVHeatpumpIR(), new CarrierMCAHeatpumpIR(), new MideaHeatpumpIR(), new FujitsuHeatpumpIR(),new MitsubishiFDHeatpumpIR(),
new MitsubishiFEHeatpumpIR(), new MitsubishiMSYHeatpumpIR(), new MitsubishiFAHeatpumpIR(),new SamsungAQVHeatpumpIR(), new SamsungFJMHeatpumpIR(),new SharpHeatpumpIR(), 
new DaikinHeatpumpIR(),new MitsubishiHeavyZJHeatpumpIR(), new MitsubishiHeavyZMHeatpumpIR(),new MitsubishiSEZKDXXHeatpumpIR(),new HyundaiHeatpumpIR(), new HisenseHeatpumpIR(),
new GreeGenericHeatpumpIR(), new GreeYANHeatpumpIR(), new GreeYAAHeatpumpIR(), new ToshibaHeatpumpIR(), new ToshibaDaiseikaiHeatpumpIR(),new IVTHeatpumpIR(), 
new HitachiHeatpumpIR(),new BalluHeatpumpIR(), new AUXHeatpumpIR(),NULL};

int i;
String Get,Band,Mode;
uint8_t mode1,mode2,mode3,mode4,mode5,mode6;
int indTV,ind1,ind2,ind3,ind4,ind5,ind6,ModeTV,Mode1,Mode2,Mode3,Mode4,Mode5,Mode6; 
void setup() {
  
  Serial.begin(115200);
  //nodeIR.begin(57600);
  Serial1.begin(57600);
  //pinMode(2,INPUT);
  //pinMode(3,OUTPUT); 
  Serial.setTimeout(50); 
  Serial1.setTimeout(50); 
  delay(500);
 
}

void loop() {
  if(Serial.available())
  {
    String test = Serial.readStringUntil('\n');
    Serial.println(test);
    Serial1.println(test);
  }
 if(Serial1.available())
 {
   Get = Serial1.readStringUntil('\n');
   Serial.println(Get);
   Get.trim();
   
  // Get = "AIRFujitsuHeatpumpIR2,2,3,24,3,4";
    if(Get.startsWith("TV"))
    {
      Get.replace("TV","");
      Band = Get;
      if(Band.startsWith("LG"))
      {
        Band.replace("LG","");
        Mode = Band;
        indTV = Mode.indexOf('*');
        ModeTV = Mode.substring(0,indTV).toInt();
        switch (ModeTV)
        {
         case 1 :
         irsend.sendLG(0xE0E0807F,32);
         Serial.println("Input");
         break;
          case 2 :
           irsend.sendLG(0x20DF10EF,32);
           Serial.println("ON&OFF"); 
           break;
          case 3 :
           irsend.sendLG(0xE0E0D02F,32);
           Serial.println("VolumeDown");
           break;
          case 4 :
           irsend.sendLG(0x20DF40BF,32);
           Serial.println("volumeUp");
           break;
          case 5 :
           irsend.sendLG(0xE0E0F00F,32);
           Serial.println("MUTE");
           break;
          case 6 :
           irsend.sendLG(0xE0E016E9,32);
           Serial.println("OK");
           break;
          case 7 :
           irsend.sendLG(0x20DF55AA,32);
           Serial.println("INFO");
           break;
          case 8 :
           irsend.sendLG(0xE0E006F9,32);
           Serial.println("ChanalUp");
           break;
          case 9 :
           irsend.sendLG(0xE0E08679,32);
           Serial.println("ChanalDown");
           break;
          case 10 :
           irsend.sendLG(0xE0E08679,32);
           Serial.println("Menu");
           break;
          case 11 :
           irsend.sendLG(0x20DF02FD,32);
           Serial.println("ProgramUp");
           break;
          case 12 :
           irsend.sendNEC(0x20DF827D,32);
           Serial.println("ProgramDown");
           break;
          case 13 :
           irsend.sendNEC(0x20DFE01F,32);
           Serial.println("ProgramLeft");
           break;
          case 14 :
           irsend.sendNEC(0x20DF609F,32);
           Serial.println("ProgramRight");
           break;
      }
     }
      else if(Band.startsWith("SONY"))
      {
        Band.replace("SONY","");
        Mode = Band;
        indTV = Mode.indexOf('*');
        ModeTV = Mode.substring(0,indTV).toInt();
        switch (ModeTV)
        {
         case 1 :
          irsend.sendSony(0xA50,12);
          Serial.println("Input");
          break;
          case 2 :
           irsend.sendSony(0xA90,12);
           Serial.println("On_Off");
            break;
          case 3 :
           irsend.sendSony(0x490,12);
           Serial.println("VolumnUp");
            break;
          case 4 :
           irsend.sendSony(0xc90,12);
           Serial.println("VolumnDown");
            break;
          case 5 :
           irsend.sendSony(0x90,12);
           Serial.println("ChanalUp");
            break;
          case 6 :
           irsend.sendSony(0x890,12);
           Serial.println("ChanalDown");
            break;
          case 7 :
           irsend.sendSony(0x290,12);
           Serial.println("Mute");
            break;
          case 8 :
           irsend.sendSony(0xA70,12);
           Serial.println("Mute");
            break;
          case 9 :
           irsend.sendSony(0x70,12);
           Serial.println("Menu");
            break;
          case 10 :
           irsend.sendSony(0x2F0,12);
           Serial.println("ProgramUp");
            break;
          case 11 :
           irsend.sendSony(0xAF0,12);
           Serial.println("ProgramDown");
            break;
          case 12 :
           irsend.sendSony(0x2D0,12);
           Serial.println("ProgramLeft");
            break;
          case 13 :
           irsend.sendSony(0xCD0,12);
           Serial.println("ProgramRight");
            break;
      }
     }
      else if(Band.startsWith("Sumsung"))
      {
        Band.replace("Sumsung","");
        Mode = Band;
        indTV = Mode.indexOf('*');
        ModeTV = Mode.substring(0,indTV).toInt();
        switch (ModeTV)
        {
         case 1 :
          irsend.sendSAMSUNG(0xE0E0807F,32);
          Serial.println("Input");
          break;
         case 2 :
          irsend.sendSAMSUNG(0xE0E040BF,32);
          Serial.println("On_Off");
          break;
         case 3 :
          irsend.sendSAMSUNG(0xE0E0E01F,32);
          Serial.println("VolumnUp");
          break;
         case 4 :
          irsend.sendSAMSUNG(0xE0E0D02F,32);
          break;
         case 5 :
          irsend.sendSAMSUNG(0xE0E048B7,32);
          Serial.println("ChanalUp");
          break;
         case 6 :
          irsend.sendSAMSUNG(0xE0E008F7,32);
          Serial.println("ChanalDown");
          break;
         case 7 :
          irsend.sendSAMSUNG(0xE0E0F00F,32);
          Serial.println("Mute");
          break;
         case 8 :
          irsend.sendSAMSUNG(0xE0E016E9,32);
          Serial.println("OK");
          break;
         case 9 :
          irsend.sendSAMSUNG(0xE0E058A7,32);
          Serial.println("Menu");
          break;
         case 10 :
          irsend.sendSAMSUNG(0xE0E006F9,32);
          Serial.println("ProgramUp");
          break;
         case 11 :
          irsend.sendSAMSUNG(0xE0E08679,32);
          Serial.println("ProgramDown");
          break;
         case 12 :
          irsend.sendSAMSUNG(0xE0E0A659,32);
          Serial.println("ProgramLeft");
          break;
         case 13 :
         irsend.sendSAMSUNG(0xE0E046B9,32);
         Serial.println("ProgramRight");
         break;
      }
     }
      else if(Band.startsWith("Panasonic"))
      {
        Band.replace("Panasonic","");
        Mode = Band;
        indTV = Mode.indexOf('*');
        ModeTV = Mode.substring(0,indTV).toInt();
        switch (ModeTV)
        {
         case 1 :
          irsend.sendPanasonic(0x100A0A1,48);
          Serial.println("Input");
          break;
         case 2 :
          irsend.sendPanasonic(0x100BCBD,48);
          Serial.println("On_Off");
          break;
         case 3 :
          irsend.sendPanasonic(0x1000405,48);
          Serial.println("VolumnUp");
          break;
         case 4 :
          irsend.sendPanasonic(0x1008485,48);
          Serial.println("VolumnDown");
          break;
         case 5 :
          irsend.sendSony(0x1002C2D,48);
          Serial.println("ChanalUp"); 
          break;
         case 6 :
          irsend.sendPanasonic(0x100ACAD,48);
          Serial.println("ChanalDown");
          break;
         case 7 :
          irsend.sendPanasonic(0x1004C4D,48);
          Serial.println("Mute");
          break;
         case 8 :
          irsend.sendPanasonic(0x1004A4B,48);
          Serial.println("Menu");
          break;
         case 9 :
          irsend.sendPanasonic(0x1005253,48);
          Serial.println("ProgramUp");
          break;
         case 10 :
          irsend.sendPanasonic(0x100D2D3,48);
          Serial.println("ProgramDown");
          break;
         case 11 :
          irsend.sendPanasonic(0x1007273,48);
          Serial.println("ProgramLeft");
          break;
         case 12 :
          irsend.sendPanasonic(0x100F2F3,48);
          Serial.println("ProgramRight");
          break;
        }
      }
      else if(Band.startsWith("JVC"))
      {
        Band.replace("JVC","");
        Mode = Band;
        indTV = Mode.indexOf('*');
        ModeTV = Mode.substring(0,indTV).toInt();
        switch (ModeTV)
        {
         case 1 :
          irsend.sendJVC(0xC0C8,16,false);
          Serial.println("Input");
          break;
         case 2 :
          irsend.sendJVC(0xC0E8,16,false);
          Serial.println("On_Off");
          break;
         case 3 :
          irsend.sendJVC(0xC078,16,false);
          Serial.println("VolumnUp");
          break;
         case 4 :
          irsend.sendJVC(0xC0F8,16,false);
          Serial.println("VolumnDown");
          break;
         case 5 :
          irsend.sendJVC(0xC098,16,false);
          Serial.println("ChanalUp");
          break;
         case 6 :
          irsend.sendJVC(0xC018,16,false);
          Serial.println("ChanalDown");
          break;
         case 7 :
          irsend.sendJVC(0xC038,16,false);
          Serial.println("Mute");
          break;
         case 8 :
          irsend.sendJVC(0xC050,16,false);
          Serial.println("OK");
          break;
         case 9 :
          irsend.sendJVC(0xC05E,16,false);
          Serial.println("Menu");
          break;
         case 10 :
          irsend.sendJVC(0xC03E,16,false);
          Serial.println("ProgramUp");
          break;
         case 11 :
          irsend.sendJVC(0xC0DE,16,false);
          Serial.println("ProgramDown"); 
          break;
         case 12 :
          irsend.sendJVC(0xC0DA,16,false);
          Serial.println("ProgramLeft");
          break;
         case 13 :
          irsend.sendJVC(0xC05A,16,false);
          Serial.println("ProgramRight");
          break;
        }
      }
      else if(Band.startsWith("Sharp"))
      {
        Band.replace("Sharp","");
        Mode = Band;
        indTV = Mode.indexOf('*');
        ModeTV = Mode.substring(0,indTV).toInt();
        switch (ModeTV)
        {
         case 1 :
          irsend.sendSharpRaw(0xC0C8,16);
          Serial.println("Input");
          break;
         case 2 :
          irsend.sendSharpRaw(0xC0E8,16);
          Serial.println("On_Off");
          break;
         case 3 :
          irsend.sendSharpRaw(0xC078,16);
          Serial.println("VolumnUp");
          break;
         case 4 :
          irsend.sendSharpRaw(0xC0F8,16);
          Serial.println("VolumnDown");
          break;
         case 5 :
          irsend.sendSharpRaw(0xC098,16);
          Serial.println("ChanalUp"); 
          break;
         case 6 :
          irsend.sendSharpRaw(0xC018,16);
          Serial.println("ChanalDown"); 
          break;
         case 7 :
          irsend.sendSharpRaw(0xC038,16);
          Serial.println("Mute");
          break;
         case 8 :
          irsend.sendSharpRaw(0xC050,16);
          Serial.println("OK"); 
          break;
         case 9 :
          irsend.sendSharpRaw(0xC05E,16);
          Serial.println("Menu"); 
          break;
         case 10 :
          irsend.sendSharpRaw(0xC03E,16);
          Serial.println("ProgramUp"); 
          break;
         case 11 :
          irsend.sendSharpRaw(0xC0DE,16);
          Serial.println("ProgramDown");
          break;
         case 12 :
          irsend.sendSharpRaw(0xC0DA,16);
          Serial.println("ProgramLeft");
          break;
         case 13 :
          irsend.sendSharpRaw(0xC05A,16);
          Serial.println("ProgramRight");
          break;
        }
      }
    }
    else if(Get.startsWith("AIR"))
    {
      Get.replace("AIR","");
      Band = Get;
      if(Band.startsWith("PanasonicCKPHeatpumpIR"))
      {
        Band.replace("PanasonicCKPHeatpumpIR","");
        Mode = Band;
        i=0;
      }
      else if(Band.startsWith("PanasonicDKEHeatpumpIR"))
      {
        Band.replace("PanasonicDKEHeatpumpIR","");
        Mode = Band;
        i=1;
      }
      else if(Band.startsWith("PanasonicJKEHeatpumpIR"))
      {
        Band.replace("PanasonicJKEHeatpumpIR","");
        Mode = Band;
        i=2;
      }
      else if(Band.startsWith("PanasonicNKEHeatpumpIR"))
      {
        Band.replace("PanasonicNKEHeatpumpIR","");
        Mode = Band;
        i=3;
      }
      else if(Band.startsWith("PanasonicLKEHeatpumpIR"))
      {
        Band.replace("PanasonicLKEHeatpumpIR","");
        Mode = Band;
        i=4;
      }
      else if(Band.startsWith("CarrierNQVHeatpumpIR"))
      {
        Band.replace("CarrierNQVHeatpumpIR","");
        Mode = Band;
        i=5;
      }
      else if(Band.startsWith("CarrierMCAHeatpumpIR"))
      {
        Band.replace("CarrierMCAHeatpumpIR","");
        Mode = Band;
        i=6;
      }
      else if(Band.startsWith("MideaHeatpumpIR"))
      {
        Band.replace("MideaHeatpumpIR","");
        Mode = Band;
        i=7;
      }
      else if(Band.startsWith("FujitsuHeatpumpIR"))
      {
        Band.replace("FujitsuHeatpumpIR","");
        Mode = Band;
        i=8;
      }
      else if(Band.startsWith("MitsubishiFDHeatpumpIR"))
      {
        Band.replace("MitsubishiFDHeatpumpIR","");
        Mode = Band;
        i=9;
      }
      else if(Band.startsWith("MitsubishiFEHeatpumpIR"))
      {
        Band.replace("MitsubishiFEHeatpumpIR","");
        Mode = Band;
        i=10;
      }
      else if(Band.startsWith("FujitsuHeatpumpIR"))
      {
        Band.replace("FujitsuHeatpumpIR","");
        Mode = Band;
        i=11;
      }
      else if(Band.startsWith("MitsubishiMSYHeatpumpIR"))
      {
        Band.replace("MitsubishiMSYHeatpumpIR","");
        Mode = Band;
        i=12;
      }
      else if(Band.startsWith("MitsubishiFAHeatpumpIR"))
      {
        Band.replace("MitsubishiFAHeatpumpIR","");
        Mode = Band;
        i=13;
      }
      else if(Band.startsWith("SamsungAQVHeatpumpIR"))
      {
        Band.replace("SamsungAQVHeatpumpIR","");
        Mode = Band;
        i=14;
      }
      else if(Band.startsWith("SamsungFJMHeatpumpIR"))
      {
        Band.replace("SamsungFJMHeatpumpIR","");
        Mode = Band;
        i=15;
      }
      else if(Band.startsWith(" "))
      {
        Band.replace("SharpHeatpumpIR","");
        Mode = Band;
        i=16;
      }
      else if(Band.startsWith("DaikinHeatpumpIR"))
      {
        Band.replace("DaikinHeatpumpIR","");
        Mode = Band;
        i=17;
      }
      else if(Band.startsWith("MitsubishiHeavyZJHeatpumpIR"))
      {
        Band.replace("MitsubishiHeavyZJHeatpumpIR","");
        Mode = Band;
        i=18;
      }
      else if(Band.startsWith("MitsubishiHeavyZMHeatpumpIR"))
      {
        Band.replace("MitsubishiHeavyZMHeatpumpIR","");
        Mode = Band;
        i=19;
      }
      else if(Band.startsWith("MitsubishiSEZKDXXHeatpumpIR"))
      {
        Band.replace("MitsubishiSEZKDXXHeatpumpIR","");
        Mode = Band;
        i=20;
      }
      else if(Band.startsWith("HyundaiHeatpumpIR"))
      {
        Band.replace("HyundaiHeatpumpIR","");
        Mode = Band;
        i=21;
      }
      else if(Band.startsWith("HisenseHeatpumpIR"))
      {
        Band.replace("HisenseHeatpumpIR","");
        Mode = Band;
        i=22;
      }
      else if(Band.startsWith("GreeGenericHeatpumpIR"))
      {
        Band.replace("GreeGenericHeatpumpIR","");
        Mode = Band;
        i=23;
      }
      else if(Band.startsWith("GreeYANHeatpumpIR"))
      {
        Band.replace("GreeYANHeatpumpIR","");
        Mode = Band;
        i=24;
      }
      else if(Band.startsWith("GreeYAAHeatpumpIR"))
      {
        Band.replace("GreeYAAHeatpumpIR","");
        Mode = Band;
        i=25;
      }
      else if(Band.startsWith("SamsungAQVHeatpumpIR"))
      {
        Band.replace("SamsungAQVHeatpumpIR","");
        Mode = Band;
        i=26;
      }
      else if(Band.startsWith("ToshibaHeatpumpIR"))
      {
        Band.replace("ToshibaHeatpumpIR","");
        Mode = Band;
        i=27;
      }
      else if(Band.startsWith("ToshibaDaiseikaiHeatpumpIR"))
      {
        Band.replace("ToshibaDaiseikaiHeatpumpIR","");
        Mode = Band;
        i=28;
      }
      else if(Band.startsWith("IVTHeatpumpIR"))
      {
        Band.replace("IVTHeatpumpIR","");
        Mode = Band;
        i=29;
      }
      else if(Band.startsWith("HitachiHeatpumpIR"))
      {
        Band.replace("HitachiHeatpumpIR","");
        Mode = Band;
        i=30;
      }
      else if(Band.startsWith("BalluHeatpumpIR"))
      {
        Band.replace("BalluHeatpumpIR","");
        Mode = Band;
        i=31;
      }
      else if(Band.startsWith("AUXHeatpumpIR"))
      {
        Band.replace("AUXHeatpumpIR","");
        Mode = Band;
        i=32;
      }
        ind1 = Mode.indexOf(',');
        Mode1 = Mode.substring(0,ind1).toInt();
        ind2 = Mode.indexOf(',',ind1+1);
        Mode2 = Mode.substring(ind1+1,ind2).toInt();
        ind3 = Mode.indexOf(',',ind2+1);
        Mode3 = Mode.substring(ind2+1,ind3).toInt();
        ind4 = Mode.indexOf(',',ind3+1);
        Mode4 = Mode.substring(ind3+1,ind4).toInt();
        ind5 = Mode.indexOf(',',ind4+1);
        Mode5 = Mode.substring(ind4+1,ind5).toInt();
        ind6 = Mode.indexOf(',',ind5+1);
        Mode6 = Mode.substring(ind5+1,ind6).toInt();

     switch (Mode1) {
       case 1:
          mode1 = POWER_ON ; 
          Serial.print("POWER_ON");
       break;
       case 2:
          mode1 = POWER_OFF; 
          Serial.print("POWER_OFF");
       break;
          }

          
     switch (Mode2) {
       case 1:
          mode2 = MODE_AUTO;
          Serial.print("MODE_AUTO");
       break;
       case 2:
          mode2 = MODE_HEAT;
          Serial.print("MODE_HEAT");
       break;
       case 3:
          mode2 = MODE_COOL;
          Serial.print("MODE_COOL");
       break;
       case 4:
          mode2 = MODE_DRY; 
          Serial.print("MODE_DRY");
       break;
       case 5:
          mode2 = MODE_FAN; 
          Serial.print("MODE_FAN");
       break;
       case 6:
          mode2 = MODE_MAINT; 
          Serial.print("MODE_MAINT");
       break;      
          }

          
          switch (Mode3) {
       case 1:
          mode3 = FAN_AUTO; 
          Serial.print("FAN_AUTO");
       break;
       case 2:
          mode3 = FAN_1;
          Serial.print("FAN_1");
       break;
       case 3:
          mode3 = FAN_2;
          Serial.print("FAN_2");
       break;
       case 4:
          mode3 = FAN_3;
          Serial.print("FAN_3");
       break;
       case 5:
          mode3 = FAN_4;
          Serial.print("FAN_4");
       break;
       case 6:
          mode3 = FAN_5;
          Serial.print("FAN_5");
       break;
          }

        mode4 =(uint8_t)Mode4;
        Serial.print(Mode4);
          
          switch (Mode5) {
       case 1:
          mode5 = VDIR_AUTO; 
          Serial.print("VDIR_AUTO");
       break;
       case 2:
          mode5 = VDIR_MANUAL; 
          Serial.print("VDIR_MANUAL");
       break;
       case 3:
          mode5 = VDIR_SWING;
          Serial.print("VDIR_SWING");
       break;
       case 4:
          mode5 = VDIR_UP;
          Serial.print("VDIR_UP");
       break;
       case 5:
          mode5 = VDIR_MUP; 
          Serial.print("VDIR_MUP");
       break;
       case 6:
          mode5 = VDIR_MIDDLE; 
          Serial.print("VDIR_MIDDLE");
       break;
       case 7:
          mode5 = VDIR_MDOWN; 
          Serial.print("VDIR_MDOWN");
       break;
       case 8:
          mode5 = VDIR_DOWN; 
          Serial.print("VDIR_DOWN");
       break;
          }

          
          switch (Mode6) {
       case 1:
          mode6 = HDIR_AUTO; 
          Serial.println("HDIR_AUTO");
       break;
       case 2:
          mode6 = HDIR_MANUAL;
          Serial.println("HDIR_MANUAL");
       break;
       case 3:
          mode6 = HDIR_SWING; 
          Serial.println("HDIR_SWING");
       break;
       case 4:
          mode6 = HDIR_MIDDLE; 
          Serial.println("HDIR_MIDDLE");
       break;
       case 5:
          mode6 = HDIR_LEFT; 
          Serial.println("HDIR_LEFT");
       break;
       case 6:
          mode6 = HDIR_MLEFT; 
          Serial.println("HDIR_MLEFT");
       break;
       case 7:
          mode6 = HDIR_MRIGHT; 
          Serial.println("HDIR_MRIGHT");
       break;
       case 8:
          mode6 = HDIR_RIGHT; 
          Serial.println("HDIR_RIGHT");
       break;
          }
 heatpumpIR[i]->send(irSender, mode1, mode2, mode3, mode4, mode5, mode6);
      }
    }
 }
