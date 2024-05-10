//C:\Users\Angel Cruz\Documents\Arduino\libraries\lcdgfx\src\lcd_hal\arduino\arduino_wire.h       change scl and sda in this file
//savedContacts address 1
//myDeviceInfo address 2
//myContacts address 0
#include <FastLED.h>//for RGB led
#include <EEPROM.h>//for saving contacts
#include "OneButton.h"//for main button control
#include "lcdgfx.h"//for OLED and Menu system
#include <SPI.h>//for RF radio
#include <RF24.h>//for RF radio
#include <nRF24L01.h>//for RF radio
#include "WiFi.h"


#define menuItems 5
#define typedTextLength 80
#define mainButton 4
#define notificationLED 15

#define ledPin    2//arduino 22
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    2
#define FRAMES_PER_SECOND  120//120
CRGB leds[NUM_LEDS];
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
//Main-------------------------------------------------------------------------------------
OneButton button(mainButton, true);//initiate oneButton button on pin 2
bool inDisplayDeviceInfo = false;
long int _myDeviceID;
char _myDeviceID_[6];
char _myDeviceName[40];
struct myDeviceInfo
{
  long int myDeviceID;
  char myDeviceName[40];
};

myDeviceInfo _myDeviceInfo;
int restartInt = 0;//used only when powering ON for first time

//--------for creating/displaying contact--------//
bool inPairMode = false;
int pairModeLayer = 1;
char contact_name[20];//used to temporarily store entered contact name
long int device_ID;//used to temporarily store entered device ID
int savedContacts;
int contactsPage = 1;
bool inDisplayContacts = false;

char _contactName[30];//used to temporarily hold contact name to display to OLED
char _deviceID_ [6];//used to temporarily hold contact device ID number in a char array to display to OLED
long int _deviceID;//used to temporarily hold contact device ID number in an integer for conversion to char array
int _cursory = 0;

int j = 1;
int pgsNeeded;
int pgsNeededRd;
int crntPg = 1;
int Max = 9;
bool goToNextPage = false;

struct contacts 
{
  long int deviceID;
  char contactName[20];
};
contacts myContacts[21];//creates object to store contacts

//--------for menus display--------//
bool functionSelected = false;
bool LEDMenu = false;
//int function_Selected;
bool ledFunctionSelected = false;
bool inLedMenu = false;

SAppMenu mainMenu;
SAppMenu ledMenu;

const char *mainMenuItems[] =
{
    "Pair",                 //0
    "Contacts",             //1
    "Messenger",            //2
    "Recieved Messages", //3
    "Device Info",          //4
    "LED",                  //5
    "Wifi Scan",                //6
};

const char *ledMenuItems[] =
{
    "RGB",
    "White",
    "Red",
    "Green",
    "Blue",
    "Teal",
    "OFF",
};



//keypadOLED----------------------------------------------------------------------------------------------
DisplaySSD1306_128x64_I2C display(-1); // This line is suitable for most platforms by default
//const byte row1=13; //keypad row pins   arduino 2, 3, 4, 5    for FIRST PROTOTYPE
//const byte row2=12;//12
//const byte row3=14;
//const byte row4=27;
const byte row1=27; //for SECOND PROTOTYPE
const byte row2=14;
const byte row3=12;
const byte row4=13;//
const byte col1=26; //keypad column lines     arduino A0, A1, A2, A3
const byte col2=25;
const byte col3=33; //original 33
//const byte col4=32;

byte numpressed; //latest number pressed on the keypad
byte timespressed; //times the number has been pressed
byte cursorx=0; //cursor x position
byte cursory=0; //cursor y position
char letter; //stores letter that needs to be printed to the lcd
const int wait=1000; //time to wait for additional presses to same number
const int preventholddelay=150; //time to wait to prevent cycling through things too quickly
unsigned long basetime; //base time for while loop
unsigned long elapsed=0; //elapsed time in while loop
byte lastnumpressed; //the initial number pressed on the keypad
bool disablespacedelay=false; //disables the delay in the space function in the case that a different number is pressed while in while loop
const byte maxtimespressed[16]={
  1,5,4,4,4,4,4,5,4,5,2,8,2,2,2,2}; //stores maximum number of times any given key can be pressed before looping back to its first letter (used by incrementtimespressed function)
char typedtext[typedTextLength]; //stores typed text while in keypadActive function
char keypadInput[typedTextLength];//stores typed text to be used in program
char keypad_input_num[10];//stores typed char numbers while in keypadActive function for conversion to integers
long int keypadInputNum;//stores the converted char numbers to integers to be used in program
int positionintypedtext=0; //position in typedtext character array
int charremaining; //remaining characters in message
bool promptkeypress=false; //used for waiting for, and keypress detection, at the end-of composition prompt
bool keypadStatus = false;//used in program to activate keypadActivate function

//Messenger--------------------------------------------------------------------------------------------------------------
RF24 radio(17, 5); // define CE, CSN Pins for the NRF24L01
uint8_t ContactAddress;
uint8_t myDeviceAddress;
//char finalMessageOut[typedTextLength];
char nameOut[typedTextLength];              //used to store this device name for output
char colonOut[2] = ":";                     //used to store colon 
char messageOut[typedTextLength];           //used to temporarily store user message
char messageOutput[typedTextLength];        //combined message sent to other device
bool inMessenger = false;
bool inDisplayReceivedMessages = false;
int _contactSelected;
int messengerLayer = 1;
char receivedMessage[30];
int receivedMessageNum = 1;
int last = 1;//used to display messages in messenger function (used in keypadOLED function)
int g = 1;
int w;
int _cursory2 = 32;//used to display messages in messenger function (used in keypadOLED function)
bool _receiveMessagesTask = true;
//bool _displayMessagesTask = false;
struct recievedBuffer 
{
  char messageInBuffer[typedTextLength];
};
recievedBuffer messagesBuffer[20];


bool ledRainbowOn = false;


//wifi scan------------------------------------------------------------------------------------------------------------
bool inWifiScan = false;
int b = 0;
int networkNumInt;
int numOfNetworksInt;
int RSSIInt;
char networkNumChar[20];
char SSIDChar[30];
char numOfNetworksChar[10];
char RSSIChar[20];
int _cursory3 = 0;
int pgsNeededWifi;



// Task Handles---------------------------------------------------------------------------------------------------------
TaskHandle_t receiveMessagesTaskHandle;
TaskHandle_t displayMessagesTaskHandle;
TaskHandle_t ledRainbowTaskHandle;


void setup()
{
  //--Multitask setup--//
  taskSetup();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  pinMode(notificationLED, OUTPUT);//led for debugging
  pinMode(32, OUTPUT);//led for buttons
  digitalWrite(notificationLED, LOW);
  digitalWrite(32, HIGH);
  randomSeed( analogRead(A6) );

  EEPROM.begin(512);
  
  Serial.begin(115200); //start serial communication so that we can print the typed text to the serial console
  
  //--keypadOLED setup--//
  keypadSetup();
  
  //--Main button (OneButton) setup--//
  //pinMode(mainButton, INPUT_PULLUP);
  button.attachClick(singleClick);
  button.setDebounceTicks(50);
  button.attachDoubleClick(doubleclick);
  button.attachLongPressStart(longpress);
  //button.setPressTicks(1500);
  //button.setClickTicks(50);

  //--ledFunction (RGB LED) setup--//
  FastLED.addLeds<LED_TYPE, ledPin, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  fill_solid(&(leds[0]), NUM_LEDS, CRGB(0, 0, 0));
  FastLED.show();

  //--contacts and device initial setup--//
  if(EEPROM.read(256) != 1)   //if this is the first time device is being turned on, ask for device name and assign random deviceID
  {
    Serial.println("setting up device...");
    Serial.println("Entering name for device....");
    display.printFixed(0,  0, "Setting up device for first time... ", STYLE_BOLD);
    delay(2000);
    display.clear();
    display.printFixed(0,  0, "Enter a name for this device:", STYLE_BOLD);
    keypadStatus = true;
    while(keypadStatus)
    {
      keypadActive();
    }
    strncpy (_myDeviceInfo.myDeviceName, keypadInput, sizeof(_myDeviceInfo.myDeviceName));
    memset(keypadInput, NULL, typedTextLength);//clear keypadInput array of any values
    _myDeviceInfo.myDeviceID = random(10000, 99999);//assign a random 5 digit deviceID
    savedContacts = 1;
    EEPROM.write(256, 1);// write 1 to EEPROM address 256 
    EEPROM.commit();
    EEPROM.write(1, savedContacts);//save savedContacts to EEPROM address 1
    EEPROM.commit();
    EEPROM.put(2, _myDeviceInfo);//save my device info to EEPROM address 2 
    EEPROM.commit();  
    Serial.println("Device has been setup.");
    Serial.print("My Device Name: ");
    Serial.println(_myDeviceInfo.myDeviceName);
    Serial.print("My Device ID: ");
    Serial.println(_myDeviceInfo.myDeviceID);
    display.printFixed(0,  0, "Device has been setup!", STYLE_BOLD);
    delay(2000);
    display.clear();
    restartInt = 1;
  }
  else                          //if device has been turned on before, retrieve deviceID and device name from EEPROM
  {
    Serial.println("device already setup");
    EEPROM.get(0, myContacts);//retrieve saved contacts from EEPROM address 0
    savedContacts = EEPROM.read(1);      //get saved number of contacts from EEPROM address 1
    pgsNeeded = savedContacts/8;
    if(savedContacts%8)
    {
      pgsNeeded += 1;
    }
    EEPROM.commit();
    EEPROM.get(2, _myDeviceInfo);//retrieve saved myDeviceInfo from EEPROM address 2
    EEPROM.commit();
    Serial.print("Number of saved contacts: ");
    Serial.println(savedContacts);
    Serial.print("Number of pages needed: ");
    Serial.println(pgsNeeded);
    Serial.print("Device ID: ");
    Serial.println(_myDeviceInfo.myDeviceID);
  }
  
  //--setup for menu's--//
  display.createMenu( &mainMenu, mainMenuItems, sizeof(mainMenuItems) / sizeof(char *) );  
  display.createMenu( &ledMenu, ledMenuItems, sizeof(ledMenuItems) / sizeof(char *) );
  
  //--restart --//
  if (restartInt == 1)//used only when powering ON for first time
  {
    Serial.println("restarting ESP32........");
    ESP.restart();
  }

  //--RF radio setup--//
  NRF24setup();
  
  

}



void loop()
{
  Serial.println("In Main Menu...");
  button.tick();
  while (!functionSelected)//In Main Menu
  {
    button.tick();
    display.showMenu( &mainMenu );
  }
  display.clear();
  if(functionSelected)
  {
    switch (display.menuSelection(&mainMenu))
    {
      case 0:
          Serial.println("In pairing mode....");
          inPairMode = true;
          creatContact();
          break;

      case 1:
          Serial.println("In Contacts...");
          inDisplayContacts = true;
          displayContacts();
          goToNextPage = false;
          break;

      case 2:
          Serial.println("In Messenger...");
          inMessenger = true;
          messengerLayer = 1;
          messenger();
          break;

      case 3:
          Serial.println("In Recieved Messages...");
          inDisplayReceivedMessages = true;
          displayReceivedMessages();
          display.clear();
          break;

      case 4:
          Serial.println("In Device Info...");
          inDisplayDeviceInfo = true;
          displayDeviceInfo();
          break;

      case 5:
          Serial.println("In LED Menu...");
          inLedMenu = true;
          ledFunction();
          functionSelected = false;
          break;

      case 6:
          Serial.println("In WIFI Scan Mode...");
          inWifiScan = true;
          wifiScan();
          functionSelected = false;
          break;


          
          
    }
    
  }
  
}



//--------------fuctions for button presses----------------//
void singleClick()
{
  Serial.println("single click");
  if(!functionSelected)//if in main menu
  {
    display.clear();
    //display.showMenu(&menu);
    //lcd_delay(100);
    display.menuDown(&mainMenu);
    display.updateMenu(&mainMenu);
    display.showMenu( &mainMenu );
  }
  
  if(inLedMenu)//if in LED menu
  {
    display.clear();
    //display.showMenu(&menu);
    //lcd_delay(100);
    display.menuDown(&ledMenu);
    display.updateMenu(&ledMenu);
    display.showMenu( &ledMenu );
  }
  
  if(inDisplayContacts)//if in displayContacts mode
  {
    crntPg++;
    Max = Max + 9;
    _cursory = 0;
    if(crntPg > pgsNeeded)
    {
      crntPg = 1;
      j = 1;
      Max = 9;
    }
    goToNextPage = true; 
  }

  if(inDisplayReceivedMessages)
  {
    w = 1;
    g++;
    if(g = receivedMessageNum - 1)
    {
      //turn off notification led
      digitalWrite(notificationLED, LOW);
    }
    if(g > receivedMessageNum - 1)
    {
      g = 1;
    }
    display.clear();
  }

//  if(inWifiScan)
//  {
//    crntPg++;
//    Max = Max + 9;
//    _cursory3 = 0;
//    if(crntPg > pgsNeededWifi)
//    {
//      crntPg = 1;
//      b = 0;
//      Max = 9;
//    }
//    goToNextPage = true; 
//  }
  
}

void doubleclick()
{
  Serial.println("double click");
  if(!functionSelected)//if in main menu
  {
    display.clear();
    display.showMenu(&mainMenu);
    lcd_delay(500);
    display.menuUp(&mainMenu);
    display.updateMenu(&mainMenu);
    display.showMenu( &mainMenu );
  }
  
  if(inPairMode)//if in pair mode
  {
    numpressed = 15;
    inPairMode = false;
  }

  if(inLedMenu)//if in LED menu
  {
    display.clear();
    //display.showMenu(&menu);
    //lcd_delay(100);
    display.menuUp(&ledMenu);
    display.updateMenu(&ledMenu);
    display.showMenu( &ledMenu );
  }

  if(inDisplayContacts)//if in displayContacts
  {
    inDisplayContacts = false;
    functionSelected = false;
    j = 1;
    crntPg = 1;
    Max = 9;
    _cursory = 0;
    goToNextPage = true;
  }

  if(inDisplayDeviceInfo)
  {
    inDisplayDeviceInfo = false;
    functionSelected = false;
  }

  if(inMessenger)
  {
    inMessenger = false;
    functionSelected = false;
    messengerLayer = 1;
    numpressed = 15;
  }

  if(inDisplayReceivedMessages)
  {
    inDisplayReceivedMessages = false;
    functionSelected = false;
    
  }

  if(inWifiScan)
  {
    functionSelected = false;
    inWifiScan = false;
//    functionSelected = false;
//    b = 0;
//    crntPg = 1;
//    Max = 9;
//    _cursory3 = 0;
//    goToNextPage = true;
  }


}

void longpress()
{
  Serial.println("long press");
  if(!functionSelected && (EEPROM.read(256) != 1))//if in setup
  {
    numpressed = 15;
  }
  if(!functionSelected && (EEPROM.read(256) == 1))//if in main menu
  {
    functionSelected = true;
  }
  if(inPairMode)//if in pair mode
  {
    numpressed = 15;
  }

  if(inLedMenu)//if in led menu
  {
    inLedMenu = false;
    ledFunctionSelected = true;
  }

  if(inMessenger)
  {
    numpressed = 15;
//    if(messengerLayer == 1)
//    {
//      numpressed = 15;
//    }
//    if(messengerLayer == 2)
//    {
//      //send message
//    }
  }
  
}
