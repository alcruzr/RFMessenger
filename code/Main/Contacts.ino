
void creatContact()
{
  if (inPairMode)
  {
    if (pairModeLayer == 1)
    {
      Serial.println("Entering New Contact Name...");
      display.clear();
      display.printFixed(0,  0, "Enter Contact Name:", STYLE_BOLD);
      keypadStatus = true;
      while(keypadStatus)//activate keypad
      {
        keypadActive();
      }
      strncpy (contact_name, keypadInput, sizeof(contact_name));
      memset(keypadInput, NULL, typedTextLength);//clear keypadInput array of any values
      pairModeLayer = 2;
    }
    if(inPairMode)
    {
      if (pairModeLayer == 2)
      {
        Serial.println("Entering New Contact ID #...");
        display.clear();
        display.printFixed(0,  0, "Enter 5 Digit Device ID:", STYLE_BOLD);
        keypadStatus = true;
        while(keypadStatus)//activate keypad
        {
          keypadActive();
        }
        device_ID = keypadInputNum;
        memset(keypadInput, NULL, typedTextLength);//clear keypadInput array of any values
        memset(keypad_input_num, NULL, typedTextLength);//clear keypadInput array of any values 
        pairModeLayer = 3;
      }
    }
    
    if(inPairMode)//if still in pair mode, save contact to EEPROM
    {
      display.clear();
      Serial.println("New Contact Saved");
      Serial.print("ID saved: ");
      Serial.println(device_ID);
      myContacts[savedContacts].deviceID = device_ID; 
      strncpy (myContacts[savedContacts].contactName, contact_name, sizeof(myContacts[savedContacts].contactName));
      savedContacts++;//increase the number of saved contacts
      EEPROM.put(0, myContacts);//save contacts in myContacts structure array to EEPROM address 0
      EEPROM.commit();
      EEPROM.write(1, savedContacts);// save number of saved contacts to EEPROM address 1
      EEPROM.commit();
      display.printFixed(0,  0, "Contact Saved!", STYLE_BOLD);
      delay(3000);
      display.clear();
      inPairMode = false;
      pairModeLayer = 1;
    }
    
  }
  pairModeLayer = 1;
  memset(keypadInput, NULL, typedTextLength);//clear keypadInput array of any values
  memset(keypad_input_num, NULL, typedTextLength);//clear keypadInput array of any values 
  memset(contact_name, NULL, 20);//clear contact_name array of any values
  device_ID = NULL;
  functionSelected = false;
}






void displayContacts()
{
  while(inDisplayContacts)
  {
    for( j ; j < savedContacts && j < Max ; j++)
    {
      //display up to 8 contacts
      char _contactNum_[2];
      int contactNum = j;
      String _contactNumString = String(contactNum);
      _contactNumString.toCharArray(_contactNum_, sizeof(_contactNum_));
      
      strncpy (_contactName, myContacts[j].contactName, sizeof(_contactName));//copy contact name into temporary char array
      _deviceID = myContacts[j].deviceID;//copy contact ID into temporary integer variable
      Serial.println(_deviceID);//this does display correct int values
      String deviceIDString = String(_deviceID);//_deviceID is a long int//used to convert long int to a string
      deviceIDString.toCharArray(_deviceID_, sizeof(_deviceID_));//_deviceID_ is a char array//used to convert a string to a char array
      
      display.printFixed(0, _cursory, _contactNum_, STYLE_NORMAL);
      display.printFixed(6,  _cursory, _contactName, STYLE_NORMAL);
      display.printFixed(96,  _cursory, _deviceID_, STYLE_NORMAL);
      _cursory = _cursory + 8;
    }
    while(!goToNextPage)
    {
      button.tick();//what for button press
    }
    display.clear();
    goToNextPage = false;
  }
  display.clear();
  inDisplayContacts = false;
  functionSelected = false;
}















//void displayContacts()
//{
//  while(functionSelected)
//  {
//    if(savedContacts < 10)//10 is temporary
//    {
//      button.tick();
//      for(int j = 1; j < savedContacts && j < 9; j++)//savedContacts
//      {
//        char _contactNum_[2];
//        int contactNum = j;
//        String _contactNumString = String(contactNum);
//        _contactNumString.toCharArray(_contactNum_, sizeof(_contactNum_));
//        
//        strncpy (_contactName, myContacts[j].contactName, sizeof(_contactName));//copy contact name into temporary char array
//        _deviceID = myContacts[j].deviceID;//copy contact ID into temporary integer variable
//        Serial.println(_deviceID);//this does display correct int values
//        String deviceIDString = String(_deviceID);//_deviceID is a long int//used to convert long int to a string
//        deviceIDString.toCharArray(_deviceID_, sizeof(_deviceID_));//_deviceID_ is a char array//used to convert a string to a char array
//        
//        display.printFixed(0, _cursory, _contactNum_, STYLE_NORMAL);
//        display.printFixed(6,  _cursory, _contactName, STYLE_NORMAL);
//        display.printFixed(96,  _cursory, _deviceID_, STYLE_NORMAL);
//        _cursory = _cursory + 8;
//      }
//      while(functionSelected)
//      {
//        button.tick();
//      }
//    }
//    if(savedContacts > 10)//10 is temporary
//    {
//      while(functionSelected)
//      {
//        while(contactsPage == 1)
//        {
//          button.tick();
//          for(int j = 0; j < 11; j++)//11 is temporary
//          {
//            strncpy (_contactName, myContacts[j].contactName, sizeof(_contactName));//copy contact name into temporary char array
//            _deviceID = myContacts[j].deviceID;//copy contact ID into temporary integer variable
//            Serial.println(_deviceID);//this does display correct int values
//            String deviceIDString = String(_deviceID);//_deviceID is a long int//used to convert long int to a string
//            deviceIDString.toCharArray(_deviceID_, sizeof(_deviceID_));//_deviceID_ is a char array//used to convert a string to a char array
//            display.printFixed(0,  _cursory, _contactName, STYLE_NORMAL);
//            display.printFixed(96,  _cursory, _deviceID_, STYLE_NORMAL);
//            _cursory = _cursory + 8;
//          }
//        }
//        cursory = 0;
//        while(contactsPage == 2)
//        {
//          button.tick();
//          for(int j = 11; j < 21; j++)//11 is temporary
//          {
//            strncpy (_contactName, myContacts[j].contactName, sizeof(_contactName));//copy contact name into temporary char array
//            _deviceID = myContacts[j].deviceID;//copy contact ID into temporary integer variable
//            Serial.println(_deviceID);//this does display correct int values
//            String deviceIDString = String(_deviceID);//_deviceID is a long int//used to convert long int to a string
//            deviceIDString.toCharArray(_deviceID_, sizeof(_deviceID_));//_deviceID_ is a char array//used to convert a string to a char array
//            display.printFixed(0,  _cursory, _contactName, STYLE_NORMAL);
//            display.printFixed(96,  _cursory, _deviceID_, STYLE_NORMAL);
//            _cursory = _cursory + 8;
//          } 
//        }
//        _cursory = 0;
//      }    
//    }  
//  }
//  contactsPage = 1;
//  _cursory = 0;
//  display.clear();
//}
