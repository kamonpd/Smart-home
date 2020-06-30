void IsClients2()
{
  if (TKDServer.hasClient()) 
  {    
    // Serial.println("1");
    for(int i = 0; i < MAXSC; i++)
    {
      // Serial.println("2");
      //find free/disconnected spot
      if (!TKDClient[i] || !TKDClient[i].connected())
      {
        //Serial.println("3");
        if(TKDClient[i]) 
        {          
          Serial.println("3.1");
          TKDClient[i].stop();
        }
          TKDClient[i] = TKDServer.available();
          Serial.print("New Client : "); Serial.println(String(i+1) + " - ");
          continue;
      }
    }      
    // no free / disconnected spot so reject
    // Serial.println("4");
    WiFiClient TKDClient = TKDServer.available();
    TKDClient.stop();
    
  }

    //check clients for data -------------------------------------------------------
  for(int i = 0; i < MAXSC; i++)
  {    
    // Serial.println(" 5 ");
    if (TKDClient[i] && TKDClient[i].connected())
    {
      //  Serial.println("  6");
      if(TKDClient[i].available())
      {                
        // Serial.println("7");
        // If Any Data Was Available We Read IT
        while(TKDClient[i].available()) 
        {          
          //Serial.println("8");
          // Read From Client           
          String data_form_client = TKDClient[i].readStringUntil('\n');
          
          data_form_client.trim();
          Serial.println(data_form_client);
          Serial.println("asdfasdfadsfsdf");
          // Reply To Client
          TKDClient[i].println("<OK>");
//          if(data_form_client = "-CONNECTED>")
//          {
//            
//          }
//          else 
          nodeServer.println(data_form_client);

        }
      }
    }
  }
}
