#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

SoftwareSerial NodeAp (D3,D4); ////Rx,Tx
ESP8266WebServer server(80); 
WiFiClient client;
/*
#define D1 5 
#define button D1     // switch input Active Low
#define pressed LOW
*/

/////////////// SSID REGISTER ////////////////////////////  
const char* ssid_Central_AP = "ENBIP_Control";   
const char* pws_Central_AP = "123456789";

/////////////// SSID HOME ////////////////////////////  
const char* ssid = "";
const char* password = "";

/////////////// SSID CC //////////////////////////// 
const char* ssidCC = "";
const char* passwordCC = "";

//const char* host = "172.20.10.3";        //IP Server
const char* host = "54.169.122.66";        
//const int httpPort = 80;                 //Port Server

String Username,nameBox,ssid_Home,wpws_Home,ssidCC,wpws_CC,closer;  //ตัวแปรสำหรับรับค่าจาก Web
String str;
String path1,path2,path3;            // path ไป Server
String IdCC;                         // ID กล่อง Central
char EEpromID[6];                    // ID กล่อง Central(Char aray) 

String same;                         // เอาไว้เทียบค่าไม่ให้ซ้ำ
byte AP = 1;                         // Mode AP
byte ADD = 1;                        // Mode ADD

//int buttonPin = D6;
const byte interruptPin = D2;
const byte resetPin  = D7;

void setup() 
{
  // กำหนดฟังก์ชันขา
  
  pinMode(resetPin,OUTPUT);
  digitalWrite(resetPin,HIGH);
  //pinMode(button,INPUT_PULLUP);
  pinMode(D3,INPUT);
  pinMode(D4,OUTPUT);

   //กำหนด Interrupt
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(interruptPin,Button_Check,FALLING);
  
  
  // กำหนด buadrate
  Serial.begin(115200);
  NodeAp.begin(9600);
  NodeAp.setTimeout(50);
   
  //เริ่ม EEPROM
  EEPROM.begin(512); //เริ่ม eeprom
  delay(10);

  //เริ่มเว็บเซิฟเวอร์ 
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  // อ่านค่าจาก eeprom ตำแหน่ง 65 เก็บที่ AP
  EEPROM.get(65,AP);
  Serial.println("AP = "+String(AP));
  
  //ถ้า AP=1 จะเข้าโหมดการลงทะเบียนกล่อง
  if( AP == 1)
  {
      // เริ่ม Mode Ap
      WiFi.mode(WIFI_AP);
      WiFi.softAP(ssid_Central_AP, pws_Central_AP);
     
      while (WiFi.softAPgetStationNum()>=0)
      {
        Serial.print("Ready......Connected  ");
        Serial.println(WiFi.softAPIP());
        Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
        
        server.handleClient();
        if(closer == "ok"){
          break;
        }
       // Button_Check();
        delay(1000);
      }
      
      NodeAP.println("SS"+ssidCC+"&"+passwordCC+"CRLF");
      
      ssid = ssid_Home.c_str();       //แปลง String เป็น  char
      password = wpws_Home.c_str();  
      
      WiFi.softAPdisconnect(true);   //เริ่มโหมด Staion
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      Serial.println("Connect to "+String(ssid_Home));
      Serial.println("connecting");
      
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        //Button_Check();
      }
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      
      AP = 0;
      ADD = 0;
      EEPROM.put(0,ssid);      // บันทึก ssid_Home ลงตำแหน่ง 0
      EEPROM.commit();         
      EEPROM.put(32,password);     // บันทึก wpws_Home ลงตำแหน่ง 32
      EEPROM.commit();
      EEPROM.put(65,AP);             // บันทึก AP ลงตำแหน่ง 65
      EEPROM.commit();
      EEPROM.put(66,ADD);           // บันทึก ADD ลงตำแหน่ง 66
      EEPROM.commit();
      
  }
  /*
  if(AP==2)
  {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid_Central_AP, pws_Central_AP);
  }
  */
  else
  {  
      
      EEPROM.get(0,ssid);      // อ่านค่าจาก eeprom ที่ตำแหน่ง 0 เก็บไว้ ssid_Home
      EEPROM.get(32,password);
     // ssid = ssid_Home.c_str();     // แปลง String เป็น char  
     // password = wpws_Home.c_str();
      str = String(ssid_Home);
      WiFi.softAPdisconnect(true);
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      Serial.print("Connect to ");
      Serial.println(EEPROM.get(0,ssid));
      Serial.println(EEPROM.get(32,password));
      Serial.println("connecting");
      while (WiFi.status() != WL_CONNECTED) 
      {
        delay(500);
        Serial.print(".");
       // Button_Check();
      }
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

  }
  delay(300);
  //client.connect(host, httpPort);
/*
  //เชื่อมต่อ Server
  while(!client.connect(host, httpPort)) 
     {
      Serial.println("connecting to server");
     }
      Serial.println("connection success");
*/
}

void loop() 
{
  EEPROM.get(65,AP);
  WiFiClient client;
  const int httpPort = 80;
  if(AP==0||AP==1)
  {
  //delay(500);
  unsigned long timeout = millis();
  while(!client.connect(host, httpPort)) 
   {
      if(millis() - timeout > 10000)
    {
      NodeAp.println("ServerOFF");
      /*
      Serial.println("Mode AP Direct");
      AP=2;
      EEPROM.put(65,AP);
      EEPROM.commit();
      delay(100);
      digitalWrite(resetPin,LOW);

      */
    }
          
      Serial.println("connecting to server");
      //return;
      //delay(100);
      //Button_Check();
   }
        
  }
  
  EEPROM.get(66,ADD);
  Serial.println("AP = "+String(AP));
  Serial.println("ADD = "+String(ADD));
  EEPROM.get(68,EEpromID);
  IdCC= String(EEpromID);
  Serial.println("IDCC = "+IdCC);
  
  if(ADD == 0)
  {
    Serial.println("insertcenttral");
    ADD = 1; 
    path2 ="insertcentral/"+nameBox+"/"+Username;
    Serial.println(path2);
      client.print(String("GET /" + path2 + " HTTP/1.1\r\n") +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");         
  }
  
  

/////////   รับข้อมูลจาก client /////////////
  if (NodeAp.available())
  {
   String Data = NodeAp.readStringUntil('\n');
   Data.trim();
  
  //ถ้าข้อมูลเริ่มต้นที่ add...crlf ส่งค่าไปขอ ID client
     if(Data.startsWith("add"))
     {
      if(Data.endsWith("crlf"))
      {
        Data.replace("add","");     //แทนที่ add ด้วยที่ว่าง
        Data.replace("crlf","");
        Data.trim();
        Serial.println(Data);
        path3 = "insertchild/"+Data+"/"+IdCC;
        client.print(String("GET /" + path3 + " HTTP/1.1\r\n") +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
      }
    }     
  }

////////////////ส่ง readcomand ไปหา Server //////////////////
  path1="readcommand/"+IdCC;
  client.print(String("GET /"+path1+ " HTTP/1.1\r\n") +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
               
  
/////////// ถ้า Server ไม่ตอบกลับมา 5 วิ จะทำการเชื่อมต่อใหม่  ////////////////

if(AP==0||AP==1)
{
unsigned long timeout = millis();
  while(client.available() == 0) 
  {
    //Serial.println("Server no data");
    if (millis() - timeout > 5000) 
    {
      Serial.println(">>> Client Timeout !");
      client.stop();
      /*
     if(!client.connect(host, httpPort)) 
        {
          Serial.println("connecting to server");
        }
       // Serial.println("connection success");
       */ 
    }

    if(millis() - timeout > 10000)
    {
      Serial.println("Mode AP Direct");
      NodeAp.println("ServerOFF");
      /*
      AP=2;
      EEPROM.put(65,AP);
      EEPROM.commit();
      delay(100);
      digitalWrite(resetPin,LOW);
      */
      
    }
    
  }
  //Button_Check();
}

///////////////  รับข้อมูลจาก Server //////////////////////
    while(client.available())
 {
    String line = client.readStringUntil('\n');
    line.trim();
    //Serial.println("***********DATA form Server***********");
    Serial.println(line);

//ถ้าข้อมูล เริ่มที่ A จบที่ CRLF ให้ส่งไป Client
    if(line.startsWith("A"))
   {
    if(line.endsWith("CRLF"))
    {
       Serial.println(line);
       NodeAp.println(line);
    }
   }
//ถ้าข้อมูล เริ่มที่ CC จบที่ CRLF ให้บันทึก ID ลง eeprom   
   else if (line.startsWith("CC"))
   {
     if(line.endsWith("CRLF"))
    {
            IdCC= line.substring(2,7);
            IdCC.toCharArray(EEpromID,6);
            EEPROM.put(68,EEpromID);
            EEPROM.commit();
            delay(10);
            EEPROM.get(68,EEpromID);
            IdCC= String(EEpromID);
            Serial.println(IdCC);
            EEPROM.put(66,ADD);
            EEPROM.commit();
    }
   }

//ถ้าข้อมูล เริ่มที่ CL จบที่ CRLF ให้ส่งไป Client
   else if (line.startsWith("CL"))
   {
    if(line.endsWith("CRLF"))
    {
       Serial.println(line);
       NodeAp.println(line);
    }
   }

  // Button_Check();
  
 }



//////////////// นำข้อมูลจาก WEB ในเครือข่ายในบ้าน   /////////////////////
/*
server.handleClient();

 
if(AP==2)
{
  
  Serial.println("data = "+data);
 if(data != same)
 {
   if(data.startsWith("a"))
   {
    if(data.endsWith("crlf"))
    {
       Serial.println(data);
       NodeAp.println(data);
    }
    data = same;
   }
 }
 if(data=="close")
 {
   Serial.println("Close Mode AP Direct");
   AP=0;
   EEPROM.put(65,AP);
   EEPROM.commit();
   delay(100);
   digitalWrite(resetPin,LOW);
   
 }
 */


//delay(500);
   
}

void Button_Check()
{
  EEPROM.put(0,"");
  EEPROM.commit();
  EEPROM.put(32,"");
  EEPROM.commit();
  EEPROM.put(65,1);
  EEPROM.commit();
  digitalWrite(resetPin,LOW);  
}
