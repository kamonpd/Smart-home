void handleRoot() {
   
    server.send(200, "text/plain", "Hi.............");


 if (server.args() > 0 ) { 
 
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      
     
      if (server.argName(i) == "Username") 
      {
        Serial.println(" Input received was: "+server.arg(i));
        Username = server.arg(i);
        
      }
    
      if (server.argName(i) == "nameBox") 
      {
        Serial.println(" Input received was: "+server.arg(i));
        nameBox = server.arg(i);
        
      }
      
      if (server.argName(i) == "ssid") 
      {
        Serial.println(" Input received was: "+server.arg(i));
        ssid_Home = server.arg(i);
      }
  
      if (server.argName(i) == "pws") 
      {
        Serial.println(" Input received was: "+server.arg(i));
        wpws_Home = server.arg(i);
      }

       if (server.argName(i) == "ssidCC") 
      {
        Serial.println(" Input received was: "+server.arg(i));
        ssidCC = server.arg(i);
      }
  
      if (server.argName(i) == "pwsCC") 
      {
        Serial.println(" Input received was: "+server.arg(i));
        wpws_CC = server.arg(i);
      }
      
      if (server.argName(i) == "close") 
      {
        Serial.println(" Input received was: "+server.arg(i));
        closer = server.arg(i);

        /*
          if((server.arg("close") == "ok")){
          WiFi.softAPdisconnect(true);
          }

          */
      }
   }
 }  
}
