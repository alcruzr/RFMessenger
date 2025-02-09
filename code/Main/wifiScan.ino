//void _wifiScan()
//{
//  //display.setFixedFont( ssd1306xled_font5x7 );
//  int numOfNetworksInt = WiFi.scanNetworks();
//  itoa(numOfNetworksInt, numOfNetworksChar, 10);
//  Serial.print(numOfNetworksInt);
//  Serial.println(" networks found");
//  display.printFixed(0, 0, numOfNetworksChar, STYLE_NORMAL);
//  display.printFixed(15, 0, "networks found", STYLE_NORMAL);
//  delay(3000);
//  display.clear();
//  pgsNeeded = numOfNetworksInt/8;
//  if(numOfNetworksInt % 8)
//  {
//    pgsNeeded += 1;
//  }
//  while(inWifiScan)
//  {
//    for (b ; b < numOfNetworksInt && b < Max; ++b) 
//    {
//        // Print SSID and RSSI for each network found
//  //      Serial.print(b + 1);
//  //      networkNumInt = b + 1;
//  //      itoa(networkNumInt, networkNumChar, 10);
//        display.setTextCursor(0,_cursory3); 
//  //      display.write(networkNumChar); 
//  //      Serial.print(": ");
//  //      display.write(":"); 
//        Serial.print(WiFi.SSID(b));
//        String _SSID = WiFi.SSID(b);
//        _SSID.toCharArray(SSIDChar, sizeof(SSIDChar));
//        display.write(SSIDChar); 
//        Serial.print(" (");
//        display.write(" ("); 
//        Serial.print(WiFi.RSSI(b));
//        RSSIInt = WiFi.RSSI(b);
//        itoa(RSSIInt, RSSIChar, 10);
//        display.write(RSSIChar); 
//        Serial.print(")");
//        display.write(")");
//        Serial.println((WiFi.encryptionType(b) == WIFI_AUTH_OPEN)?" ":"*");
//        delay(10);
//        _cursory3 = _cursory3 + 8;
//    }
//    
//    while(!goToNextPage)
//    {
//      button.tick();//what for button press
//    }
//    display.clear();
//    goToNextPage = false;
//
//  }
//  inWifiScan = false;
//  functionSelected = false;
//  //display.setFixedFont( ssd1306xled_font6x8 );
//  display.clear();
//}



void wifiScan()
{
  numOfNetworksInt = WiFi.scanNetworks();
  itoa(numOfNetworksInt, numOfNetworksChar, 10);
  Serial.print(numOfNetworksInt);
  Serial.println(" networks found");
  display.printFixed(0, 0, numOfNetworksChar, STYLE_NORMAL);
  display.printFixed(15, 0, "networks found", STYLE_NORMAL);
  delay(3000);
  display.clear();
  for (int i = 0; i < numOfNetworksInt; ++i) 
  {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
//      networkNumInt = i + 1;
//      itoa(networkNumInt, networkNumChar, 10);
      display.setTextCursor(0,_cursory3); 
//      display.write(networkNumChar); 
//      Serial.print(": ");
//      display.write(":"); 
      Serial.print(WiFi.SSID(i));
      String _SSID = WiFi.SSID(i);
      _SSID.toCharArray(SSIDChar, sizeof(SSIDChar));
      display.write(SSIDChar); 
      Serial.print(" (");
      display.write(" ("); 
      Serial.print(WiFi.RSSI(i));
      RSSIInt = WiFi.RSSI(i);
      itoa(RSSIInt, RSSIChar, 10);
      display.write(RSSIChar); 
      Serial.print(")");
      display.write(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
      _cursory3 = _cursory3 + 8;
  }
  while(inWifiScan)
  {
    button.tick();
  }
  display.clear(); 
  _cursory3 = 0;
}
