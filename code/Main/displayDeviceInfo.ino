


void displayDeviceInfo()
{
  while(functionSelected)
  {
    while(inDisplayDeviceInfo)
    {
      button.tick();
      strncpy (_myDeviceName, _myDeviceInfo.myDeviceName, sizeof(_myDeviceName));//copy my device name into temporary char array
      _myDeviceID = _myDeviceInfo.myDeviceID;//copy my device ID into temporary integer variable
      String myDeviceIDString = String(_myDeviceID);//_myDeviceID is a long int//used to convert long int to a string
      myDeviceIDString.toCharArray(_myDeviceID_, sizeof(_myDeviceID_));//_deviceID_ is a char array//used to convert a string to a char array
      //display.printFixed(0, 0, "My Device Name:", STYLE_NORMAL);
      display.setTextCursor(0, 0);
      display.write("Device Name:");
      display.write(_myDeviceName);
      //display.printFixed(0, 8, "My Device ID:", STYLE_NORMAL);
      display.setTextCursor(0, 8);
      display.write("Device ID:");
      display.write(_myDeviceID_);
//      display.printFixed(16, 0, _myDeviceName, STYLE_NORMAL);
//      display.printFixed(16, 8, _myDeviceID_, STYLE_NORMAL);
    }
  }
  display.clear();
}
