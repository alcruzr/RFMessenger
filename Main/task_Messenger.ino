void taskSetup()
{
  xTaskCreatePinnedToCore(
      receiveMessagesTask,    /* Function to implement the task */
      "receiveMessages",  /* Name of the task */
      1000,     /* Stack size in words */
      NULL,     /* Task input parameter */
      1,        /* Priority of the task */
      &receiveMessagesTaskHandle,   /* Task handle. */
      0);       /* Core where the task should run */

//      xTaskCreatePinnedToCore(
//      displayMessagesTask,    /* Function to implement the task */
//      "displayInMessengerTask",  /* Name of the task */
//      1000,     /* Stack size in words */
//      NULL,     /* Task input parameter */
//      1,        /* Priority of the task */
//      &displayMessagesTaskHandle,   /* Task handle. */
//      0);       /* Core where the task should run */

  xTaskCreatePinnedToCore(
      ledRainbowTask,    /* Function to implement the task */
      "rainbowLED",  /* Name of the task */
      1000,     /* Stack size in words */
      NULL,     /* Task input parameter */
      1,        /* Priority of the task */
      &ledRainbowTaskHandle,   /* Task handle. */
      0);       /* Core where the task should run */
}

void receiveMessagesTask(void * parameter) //use DO...WHILE loops instead of WHILE loops
{
  for(;;)//do this task forever
  {

    if(_receiveMessagesTask)
    {
      vTaskDelay(100);
      radio.startListening();
      if (radio.available())// check if there is communication available
      {                                  
        radio.read(&receivedMessage, sizeof(receivedMessage));              // Receiving the input from the receiver
        if (receivedMessage[0] != NULL && receivedMessage[0] != 1)
        {
          Serial.println("Received:");
          Serial.println(receivedMessage);                          // Print whatever is received to the serial for debugging
          strcpy (messagesBuffer[receivedMessageNum].messageInBuffer,receivedMessage);
          receivedMessageNum++;
          //turn on notification led
          digitalWrite(notificationLED, HIGH);
        }   
      }
      radio.flush_rx(); 
    }
    else
    {
      vTaskDelay(100);
    }
  
  }  
}
