
//26807
//76129
//25708
//25708
//42515
//42515
//25708

//53358
//59322
//59322
//59322













//void selectedFunction()
//{
//  
//}
//
//
//
//void mainMenuDisplay()
//{
//  if (menuSelection == 1)
//  {
//    display.printFixed(0,  0, ">Pair", STYLE_BOLD);
//    display.printFixed(0,  8, " Contacts", STYLE_BOLD);
//    display.printFixed(0,  16, " Messenger", STYLE_BOLD);
//    display.printFixed(0,  24, " LED", STYLE_BOLD);
//    display.printFixed(0,  32, " Device Info", STYLE_BOLD);
//  }
//  if (menuSelection == 2)
//  {
//    display.printFixed(0,  0, " Pair", STYLE_BOLD);
//    display.printFixed(0,  8, ">Contacts", STYLE_BOLD);
//    display.printFixed(0,  16, " Messenger", STYLE_BOLD);
//    display.printFixed(0,  24, " LED", STYLE_BOLD);
//    display.printFixed(0,  32, " Device Info", STYLE_BOLD);
//  }
//  if (menuSelection == 3)
//  {
//    display.printFixed(0,  0, " Pair", STYLE_BOLD);
//    display.printFixed(0,  8, " Contacts", STYLE_BOLD);
//    display.printFixed(0,  16, ">Messenger", STYLE_BOLD);
//    display.printFixed(0,  24, " LED", STYLE_BOLD);
//    display.printFixed(0,  32, " Device Info", STYLE_BOLD);
//  }
//  if (menuSelection == 4)
//  {
//    display.printFixed(0,  0, " Pair", STYLE_BOLD);
//    display.printFixed(0,  8, " Contacts", STYLE_BOLD);
//    display.printFixed(0,  16, " Messenger", STYLE_BOLD);
//    display.printFixed(0,  24, ">LED", STYLE_BOLD);
//    display.printFixed(0,  32, " Device Info", STYLE_BOLD);
//  }
//  if (menuSelection == 5)
//  {
//    display.printFixed(0,  0, " Pair", STYLE_BOLD);
//    display.printFixed(0,  8, " Contacts", STYLE_BOLD);
//    display.printFixed(0,  16, " Messenger", STYLE_BOLD);
//    display.printFixed(0,  24, " LED", STYLE_BOLD);
//    display.printFixed(0,  32, ">Device Info", STYLE_BOLD);
//  }
//}
//
//
//
//
//void creatContact()
//{
//  pairMode = true;
//  if (pairMode)
//  {
//    if (pairModeLayer == 1)
//    {
//      display.clear();
//      display.printFixed(0,  0, "Enter Contact Name:", STYLE_BOLD);
//      keypadStatus = true;
//      while(keypadStatus)//activate keypad
//      {
//        keypadActive();
//      }
//      strncpy (contact_name, keypadInput, sizeof(contact_name));
//      //contact_name = keypadInput;
//      memset(keypadInput, NULL, typedTextLength);//clear keypadInput array of any values
//      pairModeLayer = 2;
//    }
//    if(pairMode)
//    {
//      if (pairModeLayer == 2)
//      {
//        display.clear();
//        display.printFixed(0,  0, "Enter Device ID:", STYLE_BOLD);
//        keypadStatus = true;
//        while(keypadStatus)//activate keypad
//        {
//          keypadActive();
//        }
//        device_ID = keypadInputNum;
//        memset(keypadInput, NULL, typedTextLength);//clear keypadInput array of any values
//        memset(keypadInputNum, NULL, typedTextLength);//clear keypadInput array of any values 
//        pairModeLayer = 3;
//      }
//    }
//    
//    if(pairMode)//if still in pair mode, save contact to EEPROM
//    {
//      display.clear();
//      myContacts[savedContacts + 1].deviceID = device_ID; 
//      strncpy (myContacts[savedContacts = 1].contactName, contact_name, sizeof(myContacts[savedContacts = 1].contactName));
//      EEPROM.put(255, myContacts);//save contact to EEPROM
//      savedContacts++;
//      pairMode = false;
//      pairModeLayer = 1;
//      display.printFixed(0,  0, "Contact Saved!", STYLE_BOLD);
//      delay(3000);
//      display.clear();
//    }
//    
//  }
//  memset(contact_name, NULL, 20);//clear contact_name array of any values
//}
//
//
//
//
//
//
//
//
//
//
//
////--------------fuctions for button presses----------------//
//void singleClick()
//{
//  Serial.println("single click");
//  if(!functionSelected)//if in main menu
//  {
//    menuSelection++;//scroll down
//    menuSelected++;
//    if(menuSelection == menuItems + 1)
//    {
//      menuSelection = 1;
//      menuSelected = 1;
//    }
//  }
//}
//
//void doubleclick()
//{
//  Serial.println("double click");
//  if(!functionSelected)//if in main menu
//  {
//    menuSelection--;//scroll up
//    menuSelected--;
//    if(menuSelection == 0)
//    {
//      menuSelection = 5;
//      menuSelected = 5;
//    }
//  }
//  if(pairMode)//if in pair mode
//  {
//    numpressed = 15;
//    pairMode = false;
//  }
//}
//
//void longpress()
//{
//  Serial.println("long press");
//  if(!functionSelected)
//  {
//    functionSelected = true;
//  }
//  if(pairMode)//if in pair mode
//  {
//    numpressed = 15;
//  }
//}








//26807
//64201
