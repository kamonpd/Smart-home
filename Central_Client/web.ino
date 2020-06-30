void handleRoot() 
{
  server.send(200, "text/plain", statusServer);

  if (server.args() > 0 ) 
  {    
    for ( uint8_t i = 0; i < server.args(); i++ ) 
    {     
      if (server.argName(i) == "data") 
      {
        Serial.println(" Input received was: "+server.arg(i));
        data = server.arg(i);
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
