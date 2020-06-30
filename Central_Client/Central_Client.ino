#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
//#include <SPI.h>


#define     MAXSC   8                 // MAXIMUM NUMBER OF CLIENTS
WiFiServer  TKDServer(9001);          // THE SERVER AND THE PORT NUMBER
WiFiClient  TKDClient[MAXSC];         // THE SERVER CLIENTS

//********* For ESP8266 ********//////////
//const byte tx = 5;
//const byte rx = 4;
//const byte resetPin  = 14;

//******** For NodeMCU8266 *******/////
const byte tx = D4;
const byte rx = D3;
const byte resetPin  = D7;

SoftwareSerial nodeServer(rx,tx);
ESP8266WebServer server(80); 

String data,closer,same;
String statusServer = "ON";
char ssid[30];
char pwd[30];
bool ok = true;
bool resivestatus = false;

void setup() 
{ 
  pinMode(resetPin,OUTPUT);
  digitalWrite(resetPin,HIGH);

  pinMode(rx,INPUT);
  pinMode(tx,OUTPUT);
  
  Serial.begin(115200); 
  Serial.setTimeout(50);
  nodeServer.begin(9600);
  EEPROM.begin(512);
  delay(10);

  EEPROM.get(0,ssid);
  EEPROM.get(32,pwd);
  delay(500);

  Serial.print("SSID in EEPROM : ");
  Serial.println(String(ssid));
  Serial.print("PWD in EEPROM : ");
  Serial.println(String(pwd));


  WiFi.mode(WIFI_OFF);
  delay(500);

  while(ok)
  {
    Serial.println("Wait CC Connect to router");
    
    while(nodeServer.available())
      {
        Serial.println("Serial Available");
        String mes = nodeServer.readStringUntil('\n');
        Serial.println(mes);
        mes.trim();

        if(mes.startsWith("SS"))
        {
          if(mes.endsWith("CRLF"))
          {            
            String ssid_st,pws_st;

            
            String sub = mes.substring(2,mes.length()-4);
            ssid_st = sub.substring(0,sub.indexOf('&'));
            Serial.println("ssid = "+ssid_st);
            pws_st = sub.substring(sub.indexOf('&')+1);
            Serial.println("pws = "+pws_st);
  
            strcpy(ssid,ssid_st.c_str());
            strcpy(pwd,pws_st.c_str());
            
            EEPROM.put(0,ssid);                   // บันทึก ssid_Home ลงตำแหน่ง 0
            EEPROM.commit();
            delay(500);         
            EEPROM.put(32,pwd);                   // บันทึก wpws_Home ลงตำแหน่ง 32
            EEPROM.commit();
            delay(500);
//            digitalWrite(resetPin,LOW);
            ok = false;
          }
        } 
        
        else if(mes == "ok")
        {
          ok = false;
        }

        else if(mes == "ServerON1"||mes == "ServerON")
        {
          ok = false;
          resivestatus = true;
        }

//        else if(mes == "ServerOFF")
//        {
//          ok = false;
//        }
        
      }
      delay(750);
  }


  Serial.println("Connected");

  Serial.print("SSID : ");
  Serial.println(String(ssid));
  Serial.print("PWD : ");
  Serial.println(String(pwd));

  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  
//****** !WiFi.softAP(ssid,pwd,1,1,8) ********//

  while(!WiFi.softAP(ssid,pwd,11,1,8))
  {
    Serial.println("SettingAP");
    delay(500);
  }
    Serial.println("Set AP OK");
    TKDServer.begin();
    Serial.println("Server Started");

    //เริ่มเว็บเซิฟเวอร์ 
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() 
{
  if(Serial.available()>0){
      String mes = Serial.readStringUntil('\n');
      Serial.println(mes);

        if(mes.startsWith("A"))
        {
          if(mes.endsWith("CRLF"))
          {
            for(int i = 0; i < MAXSC; i++)
            {
              Serial.println("Turn ON/OFF -- Enbip ISwitch");
              TKDClient[i].println(mes);
            }
          } 
        }      
      
  }
  IsClients2();                  // resive data form ClientSW
   
  server.handleClient();         //Handling of incoming requests
   
  while(nodeServer.available())
  {
    Serial.println("NodeAvailable");
    String val=nodeServer.readStringUntil('\n');
    val.trim();
    Serial.println(val);

    if(val.startsWith("A"))
    {      
      if(val.endsWith("CRLF"))
      {        
        String sub1 = val.substring(1,val.length()-4);
        Serial.println("sub : "+sub1);
        if(sub1=="NoCommand")
        {          
          Serial.println("No Broadcast");
        }
        else
        {                    
          for(int i = 0; i < MAXSC; i++)
          {
            TKDClient[i].println(val);        
          }
        }           
      } 
    }
    
    else if(val.startsWith("SS"))
    {
      if(val.endsWith("CRLF"))      
      {
        String ssid_st,pws_st;
        char savessid[30];
        char savepwd[30];

        String sub = val.substring(2,val.length()-4);
        ssid_st = sub.substring(0,sub.indexOf('&'));
        Serial.println("ssid = "+ssid_st);
        pws_st = sub.substring(sub.indexOf('&')+1);
        Serial.println("pws = "+pws_st);

        strcpy(savessid,ssid_st.c_str());
        strcpy(savepwd,pws_st.c_str());
            
        EEPROM.put(0,savessid);                   // บันทึก ssid_Home ลงตำแหน่ง 0
        EEPROM.commit();
        delay(1000);         
        EEPROM.put(32,savepwd);                   // บันทึก wpws_Home ลงตำแหน่ง 32
        EEPROM.commit();
        delay(1000);
        digitalWrite(resetPin,LOW);
      }  
    }
    
    else if(val.startsWith("CL"))
    {
      if(val.endsWith("CRLF"))
      {
        for(int i = 0; i < MAXSC; i++)
        {
          TKDClient[i].println(val);
        }
      }
    }

    else if(val.startsWith("START"))
    {
      if(val.endsWith("STOP"))
      {
        String sub = val.substring(5,val.length()-4);
        int m[sub.length()];
        int re;
        for(int i=0;i<sub.length();i++ )
        {
          if(i==0) m[i] = sub.indexOf('A');
          else
          {
            re = sub.indexOf('A',m[i-1]+1) ;
            if(re>0)
            {
              m[i]=re;
              String test = sub.substring(m[i-1],m[i]);
              Serial.println(test);
              for(int i = 0; i < MAXSC; i++)
              {
                TKDClient[i].println(test);
              }
            }
            else 
            {
              String test1 = sub.substring(m[i-1]);
              Serial.println(test1);
              for(int i = 0; i < MAXSC; i++)
              {
                TKDClient[i].println(test1);
              }                                   
              break;
            }
          }
        }
      }
    }

    else if (val == "Connecting_to_router")
    {
      digitalWrite(resetPin,LOW);
    }

    else if(val == "ServerOFF")
    {
      statusServer = "OFF";                 
    }

    else if(val == "ServerON2")
    {
      statusServer = "ON";
      resivestatus = true;
    }
  }
  
  if(resivestatus)
  {       
    for(int i = 0; i < MAXSC; i++)
    {
      Serial.println(" Sent ServerON to device ");
      TKDClient[i].println("ServerON2");
    }
    resivestatus = false;     
  }
   
  if(data != same)
  {
    Serial.println("data form app = "+data);
    same = data;
    if(data.startsWith("A"))
    {
      if(data.endsWith("CRLF"))
      {
        for(int i = 0; i < MAXSC; i++)
        {
          TKDClient[i].println(data);
        }           
      }
    }              
  }

//------------- TEST -------------------------------------//
//   if(Serial.available())
//   {
//     Serial.println("Serial Available");
//     String mes5 = Serial.readStringUntil('\n');
//        mes5.trim();
//        Serial.println(mes5);
//
//        if(mes5.startsWith("SS"))
//        {
//          if(mes5.endsWith("CRLF"))
//          {
//            String ssid_st,pws_st;
//            char savessid[30];
//            char savepwd[30];
//            
//            String sub = mes5.substring(2,mes5.length()-4);
//            ssid_st = sub.substring(0,sub.indexOf('&'));
//            Serial.println("ssid = "+ssid_st);
//            pws_st = sub.substring(sub.indexOf('&')+1);
//            Serial.println("pws = "+pws_st);
//
//            strcpy(savessid,ssid_st.c_str());
//            strcpy(savepwd,pws_st.c_str());
//
//            EEPROM.put(0,savessid);          // บันทึก ssid_Home ลงตำแหน่ง 0
//            EEPROM.commit();
//            delay(1000);         
//            EEPROM.put(32,savepwd);          // บันทึก wpws_Home ลงตำแหน่ง 32
//            EEPROM.commit();
//            delay(1000);
//            digitalWrite(resetPin,LOW);
//          }
//        }
//
//        if(mes5.startsWith("A"))
//        {
//          if(mes5.endsWith("CRLF"))
//          {
//            for(int i = 0; i < MAXSC; i++)
//            {
//              Serial.println(mes5);
//              TKDClient[i].println(mes5);
//            }
//          } 
//        }
//
//        if(mes5.startsWith("CL"))
//        {
//          if(mes5.endsWith("CRLF"))
//          {
//            for(int i = 0; i < MAXSC; i++)
//            {
//              Serial.println(mes5);
//              TKDClient[i].println(mes5);
//            }
//          } 
//        }
//
//        if(mes5 == "ServerOFF")
//        {
//          statusServer = "OFF";
//              
//        }
//
//        if(mes5 == "ServerON")
//        {
//          statusServer = "ON";
//
//          for(int i = 0; i < MAXSC; i++)
//            {
//              Serial.println(" Sent ServerON to device ");
//              TKDClient[i].println("ServerON");
//            }
//        }
//
//        if(mes5.startsWith("SV"))
//        {
//          mes5.replace("SV","");
//          nodeServer.println(mes5);
//          
//        }
//
//   }    
  delay(500);
  
}
