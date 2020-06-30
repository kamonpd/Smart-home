void handleRoot()
{
  server.send(200, "text/plain", "Hi.............");
  
  if (server.args() > 0 ) 
  { 
    for ( uint8_t i = 0; i < server.args(); i++ ) 
    {
      
      if (server.argName(i) == "nameClient" )
      {
        Serial.println(" nameClient received was: "+server.arg(i));
        nameClient = server.arg(i);
      }

      if (server.argName(i) == "ssidCC") 
      {
        Serial.println(" ssidCC received was: "+server.arg(i));
        ssidC = server.arg(i);
      }
  
      if (server.argName(i) == "pwsCC") 
      {
        Serial.println(" pwsCC received was: "+server.arg(i));
        wpwsC = server.arg(i);
      }

      if (server.argName(i) == "room_id")  // เลข 5 หลัก
      {
        Serial.println(" room_id received was: "+server.arg(i));
        roomid = server.arg(i);
      }
      
      if (server.argName(i) == "device_type")  // Normal หรือ IR 
      {
        Serial.println(" type received was: "+server.arg(i));
        type = server.arg(i);
      }
      
    
      if (server.argName(i) == "close") 
      {
        Serial.print(" close received was: "+server.arg(i));
        closer = server.arg(i);
      }
    }
  }
}
  


void Button_Check()
{
  EEPROM.put(0,0);
  EEPROM.commit();
  EEPROM.put(32,0);
  EEPROM.commit();
  EEPROM.put(65,1);
  EEPROM.commit();
//  EEPROM.put(99,1);
//  EEPROM.commit();
  digitalWrite(resetPin,LOW);

}
