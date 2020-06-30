#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WiFiClient.h>

//SoftwareSerial nodeIR(D5,D6);

ESP8266WebServer server(80);
WiFiClient TKDClient;
IPAddress centralIP(192,168,4,1);


unsigned long timeout; 
//// test esp07 OK 17/5/19

const byte interruptPin = 12;                   // Reset-register
const byte resetPin = 14;         
const byte ledpin = 5;                          // Relay
const byte buttonLED = 13;                      // SW-Home

//const byte interruptPin = D2;                 // d2 = gpio4
//const byte resetPin = D7;                     // d7 = gpio13
//const byte ledpin = D6;                       // gpio2
//const byte buttonLED = D5;                    // D3 = gio15  

char NewId[6];
char IdinEE[6];
char defaultId[6] = "00000";

const char* ssid_Client_AP = "Enbip ISwitch";
const char* pws_Client_AP =  "123456789";

String nameClient,ssidC,wpwsC,roomid,type,closer;

byte AP = 1;
byte ADD = 1;
bool statusLED = LOW;
int b ;

void setup() 
{
  pinMode(resetPin,OUTPUT);
  digitalWrite(resetPin,HIGH);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(buttonLED, INPUT_PULLUP);
  attachInterrupt(interruptPin,Button_Check,FALLING);
  attachInterrupt(buttonLED,LEDSwitch,CHANGE);

//  pinMode(LED_BUILTIN,OUTPUT);
//  digitalWrite(LED_BUILTIN,HIGH);
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin,LOW);
  
  Serial.begin(115200);
  Serial.println("Hello");

//  pinMode(D5,INPUT);                   /////////////// IR
//  pinMode(D6,OUTPUT);
  

  
  EEPROM.begin(512);
  delay(10); 
  EEPROM.get(65,AP);
  
  if( AP == 1)
  {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid_Client_AP, pws_Client_AP);
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");
    while (WiFi.softAPgetStationNum()>=0)
    {
      Serial.print("Ready......Connected  ");
      Serial.println(WiFi.softAPIP());
      Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum()); 
      server.handleClient();
  
      if(closer == "ok")
      {
         break;
      }     
      delay(1000);

    }
    
    char savessid[30];
    char savepwd[30];
    strcpy(savessid,ssidC.c_str());
    strcpy(savepwd,wpwsC.c_str());  
    
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(savessid, savepwd);
    Serial.println("Connect to "+ssidC);
    Serial.println("connecting");
      
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
      Serial.println("connect success");

      AP = 0;
      ADD = 0;

      EEPROM.put(0,savessid);       // บันทึก ssid_Home ลงตำแหน่ง 0
      EEPROM.commit();
      //delay(1000);        
      EEPROM.put(32,savepwd);       // บันทึก wpws_Home ลงตำแหน่ง 32
      EEPROM.commit();
      //delay(1000);
      EEPROM.put(65,AP);
      EEPROM.commit();
      //delay(1000);
      EEPROM.put(66,ADD);
      EEPROM.commit();
      //delay(1000);
      EEPROM.put(99,defaultId);
      EEPROM.commit();

      delay(1000);
      Request();


  }

  
  else
  {
   char ssid[30];
   char pwd[30];

   EEPROM.get(0,ssid);      // อ่านค่าจาก eeprom ที่ตำแหน่ง 0 เก็บไว้ ssid_Home
   EEPROM.get(32,pwd);

   WiFi.softAPdisconnect(true);
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pwd);
   Serial.println("ssid = "+String(ssid)+"  password = "+String(pwd));

   while (WiFi.status() != WL_CONNECTED) 
   {
    delay(500);
    Serial.print("."); 
   }  
   Serial.println("connect success");
   delay(500); 
   Request();

//   delay(500);
//   TKDClient.stop();
//   while(!TKDClient.connect(centralIP,9001))
//   {
//     Serial.println("Connecting to Central");
//   }
//     Serial.println    ("-CONNECTED>");
//     TKDClient.println ("-CONNECTED>");
   
  }

  timeout = millis();
       
}


void loop() 
{
  //Serial.println("LOOP");
  
  if(Serial.available())
  {
    TKDClient.println(Serial.readStringUntil('\n'));
    TKDClient.flush();
  }  
 
//  EEPROM.get(66,ADD);
  EEPROM.get(99,IdinEE);
  String IDinEE = String(IdinEE);
  
  

//    if(nodeIR.available())
//    {
//      Serial.println(nodeIR.readStringUntil('\n'));
//    }
    
  while (TKDClient.available())
  {
    Serial.print("ID in EEROM = ");
    Serial.println(IDinEE);
    Serial.println("resive data from server ");
    String dataserver = TKDClient.readStringUntil('\n');
    dataserver.trim();
    Serial.print("DatafromCentral = ");
    Serial.println(dataserver);

    if(dataserver.startsWith("CL"))
    {
      if(dataserver.endsWith("CRLF"))
      { 
        String sub1=dataserver.substring(2,7);
        String sub2=dataserver.substring(7,12);
        Serial.println(sub1);
        Serial.println(sub2);

        sub1.toCharArray(NewId,6);
        EEPROM.get(99,IdinEE);
        String IDinEE = String(IdinEE);
        Serial.print("ID in EEROM = ");
        Serial.println(IDinEE);
        if(IDinEE == sub2)
        {
          EEPROM.put(99,NewId);
          EEPROM.commit();
          Serial.println("Save ID");
          ADD = 1 ;
          EEPROM.put(66,ADD);
          EEPROM.commit();
          delay(1000);
        }
      }    
     }

     if (dataserver.startsWith("A"))
     {
      if(dataserver.endsWith("CRLF"))
      {
        Serial.println("Read_Command");
        String address,value;
        address = dataserver.substring(1,6);
        value = dataserver.substring(6,dataserver.length()-4);
        Serial.println(address);
        Serial.println(value);
        EEPROM.get(99,IdinEE);
        String IDinEE = String(IdinEE);
        Serial.print("ID in EEROM = ");
        Serial.println(IDinEE);
        if(IDinEE == address)
        { 
          Serial.println("ID_TRUE");
//        nodeIR.println(value);                     /////// IR
          if(value == "ON")
          {       
            Serial.println("Switch ON");
//            digitalWrite(ledpin,HIGH);             //d3 = gpio0
            statusLED = !LOW;  
          }
          if(value == "OFF")
          {
            Serial.println("Switch OFF");
//            digitalWrite(ledpin,LOW);
            statusLED = LOW;
          }
        }
        else Serial.println("ID_FALSE");        
       }
      }

      if(dataserver == "ServerON2")
      {
        TKDClient.println("STATUS"+IDinEE+String(statusLED)+"CRLF");
      }
  }

    digitalWrite(ledpin,statusLED);
  
  
  if(ADD == 0)
  {  
    Serial.println("ADD"+type+roomid+nameClient+"CRLF");
    TKDClient.println("ADD"+type+roomid+nameClient+"CRLF");
    TKDClient.flush();
    ADD = 1;
    delay(6000);
  }

//////// request connect every 1 min ///////////
  if(millis() - timeout > 60000)    
  {
    Request();
    timeout = millis();    
  }
}

void Request()
{

   TKDClient.stop();
   while(!TKDClient.connect(centralIP,9001))
   {
     Serial.println("Connecting to Central");
     delay(500);
   }
     Serial.println    ("-CONNECTED to Central");
     TKDClient.println ("-CONNECTED to Central");

}

  void LEDSwitch()
  {
     Serial.println("LED_SW");
    if(digitalRead(buttonLED))
    {
      statusLED = LOW;
    }
    else statusLED = !LOW;

    digitalWrite(ledpin,statusLED);

    EEPROM.get(99,IdinEE);
    String IDinEE = String(IdinEE);
    TKDClient.println("STATUS"+IDinEE+String(statusLED)+"CRLF");

  }
