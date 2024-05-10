void displayReceivedMessages()
{
  if (receivedMessageNum == 1)
  {
    Serial.println("No New Messages.");
    display.printFixed(0, 0, "No New Messages", STYLE_NORMAL);
    delay(2000);
    inDisplayReceivedMessages = false;
    functionSelected = false;
  }
  while(inDisplayReceivedMessages)
  {
    button.tick();
    w = g;
    display.printFixed(0, 0, messagesBuffer[g].messageInBuffer, STYLE_NORMAL);   
  }
  
  g = 1;
//  display.clear();   
}
