//RF24 radio(17, 5, 18, 19, 23);//ce, cs, sck, miso, mosi

void NRF24setup()
{
  myDeviceAddress = _myDeviceInfo.myDeviceID;
  radio.begin();//~
  radio.openReadingPipe(1, myDeviceAddress); //~Reading From my device ID pipe address
  radio.setPALevel(RF24_PA_MIN);//~
  radio.setDataRate(RF24_250KBPS);//~
  radio.enableAckPayload();//~
  radio.startListening();
}

void sendMessage() 
{
  // Sedning //
//  _displayMessagesTask = false;
  _receiveMessagesTask = false;
  delay(50);                                                  // set the delay to 5 milliseconds for stability

  strcpy (messageOutput,nameOut);                             //combine name, colon and message into one char array
  strcat (messageOutput,colonOut);
  strcat (messageOutput,messageOut);
  
  radio.stopListening();                                      // Stop listening and start sending
  Serial.print("sending:");
  Serial.println(messageOutput);                              // Print message output to the serial for debugging
  radio.write(&messageOutput, sizeof(messageOutput));         // Send message output to the Receiver
  radio.startListening();                                   // Stop sending and Start receiving
  radio.flush_tx();
//  _displayMessagesTask = true;
  _receiveMessagesTask = true;
  memset(messageOutput, NULL, typedTextLength);//clear messageOutput array of any values
  memset(messageOut, NULL, typedTextLength);//clear messageOutput array of any values
  display.setTextCursor(0,0); //maintain cursor position
  display.write("Message sent");
  delay(2000);
  delay(50);
  display.clear();
}

void messenger()
{
  while(inMessenger)
  {
    //int testA = 10901;
    int k = 1;
    display.setTextCursor(0,0); //maintain cursor position
    display.write("Enter Contact number:");

    keypadStatus = true;
    while(keypadStatus)//activate keypad
      {
        keypadActive();
      }
    _contactSelected = keypadInputNum;
    memset(keypadInput, NULL, typedTextLength);//clear keypadInput array of any values
    memset(keypad_input_num, NULL, typedTextLength);//clear keypadInput array of any values 


    if(inMessenger)//if still in messenger, begin connection with other device
    {
      myDeviceAddress = _myDeviceInfo.myDeviceID;
      ContactAddress = myContacts[_contactSelected].deviceID;
      Serial.println("Contact Address selected"); 
      Serial.println(myContacts[_contactSelected].deviceID);
      radio.openWritingPipe(ContactAddress); // Writing To selected contact pipe address//
      radio.startListening();//~
      radio.stopListening();
      
      if (radio.write( &k, sizeof(k)))//If connection to device is available
      {
        display.write("Connected");
        Serial.println("Connected");
        messengerLayer = 2;
        delay(3000);
      }
      else//if connection to device is not available
      {
        Serial.println("Failed to connect");
        display.write("Failed to connect");
        inMessenger = false;
        functionSelected = false;
        delay(3000);
      }
    }

    if(inMessenger)//if still in messenger, begin loop for messaging
    {
      if(messengerLayer == 2)
      {
//        display.setTextCursor(0,0); //maintain cursor position
//        display.write("Enter message:");
        strncpy (nameOut, _myDeviceInfo.myDeviceName, sizeof(nameOut));//copy my device name for message output
        delay(2000);
        while(inMessenger)
        {
          display.setTextCursor(0,0); //maintain cursor position
          display.write("Enter message:");
//          _displayMessagesTask = true;
          keypadStatus = true;
          while(keypadStatus)//activate keypad
            {
              keypadActive();
            }
          strncpy (messageOut, keypadInput, sizeof(messageOutput));
          memset(keypadInput, NULL, typedTextLength);//clear keypadInput array of any values
          if(inMessenger)
          {
            sendMessage();
          }
        }
      }
    }
  }
//  _displayMessagesTask = false;
  display.clear();
}
