





void keypadSetup()
{
  //OLED setup
  display.begin();
  display.clear();
  display.setFixedFont( ssd1306xled_font6x8 );
  display.setTextCursor(0,0);   //Place cursor on first row first coulomb
  //keypad setup
  pinMode(row1,OUTPUT); //set the rows as outputs
  pinMode(row2,OUTPUT);
  pinMode(row3,OUTPUT);
  pinMode(row4,OUTPUT);
  pinMode(col1,INPUT); //set the  columns as inputs
  pinMode(col2,INPUT);
  pinMode(col3,INPUT);
  //pinMode(col4,INPUT);
  delay(wait);
  rowshigh(); //sets all rows high
  
}

void displayRecentMessage()
{
  if(messengerLayer == 2)
  {
//    for (unsigned int q = (receivedMessageNum - 4);q < receivedMessageNum;q++)
//    {
//      if(q < 1)
//      {
//        q = 1;
//      }
      if(last != receivedMessageNum)
      {
        last = receivedMessageNum;
        display.printFixed(0, _cursory2, "                              ", STYLE_NORMAL);//receivedMessageNum-1
        //display.clear();
      }
      display.printFixed(0, _cursory2, messagesBuffer[receivedMessageNum-1].messageInBuffer, STYLE_NORMAL);//receivedMessageNum-1
//      _cursory2 = _cursory2 + 8;
//      if(_cursory2 >= 64)
//      {
//        _cursory2 = 32;
//      }
//    }
  }
}

void keypadActive(){
  displayRecentMessage();//**********************************************************************
  button.tick();
  if (numpressed==15){ //if enter on the pad was pressed,
      strcpy(keypadInput, typedtext);
      outputserial(); //output message and display remaining characters to serial console
      memset(typedtext, NULL, typedTextLength);//clear typedtext array of any values
      positionintypedtext=0; //reset the position in the stored text to the beginning
      keypadStatus = false;
      //convert char numbers to integers
      keypadInputNum = atol(&keypad_input_num[0]);
    }
  numpressed=16; //reset "numpressed" (16 doesn't refer to any button on the pad)
  if (findpress()){ //look for presses, if one has occurred, identify it and continue
    timespressed=0; //reset "timespressed"
    if (numpressed==10){ //if bottom left on the pad was pressed,
      textstorage(2); //perform a space in storage
      dospace(); //do a space
    }
    if (numpressed==14){ //if bottom right button on the pad was pressed,
      dobackspace(); //do a backspace
    }
    if ((numpressed<10)||numpressed==11){ //if 1,2,3,4,5,6,7,8,9, or 11 was pressed (any one of the keys with multiple characters assigned),
      Serial.println("test");
      lastnumpressed=numpressed; //record which number was pressed,
      basetime=millis(); //and take a base time for the while loop
      while (elapsed<wait){ //while below the time to wait,
        if(findpress()){ //look for presses, if one has occurred, identify it and continue
          if (numpressed==lastnumpressed){ //if it was the same as before,
            display.clear();
            displayRecentMessage();//*************************************************************
            display.printFixed(0,  0, typedtext, STYLE_NORMAL);
            incrementtimespressed(); //increment "timespressed"
            basetime=basetime+(wait-(wait-elapsed)); //roll up the base time, to allow another wait period until next press of the same button
            definepress(); //use "numpressed" and "timespressed" to define "letter"
            display.setTextCursor(cursorx,cursory); //maintain cursor position
            display.write(letter); //print the letter that was defined
            rowshigh(); //return all rows high
            delay(preventholddelay); //delay a little to prevent continuous cycling through "timespressed" during a single press
          }
          else{ //if the number that was pressed was different than before,
            disablespacedelay=true; //disable the delay in the space function to allow the press to be detected a second time, at the beginning of void loop
            break; //break out of the while loop
          }
        }
        elapsed=millis()-basetime; //refresh the elapsed time for the while loop
      }
      elapsed=0; //reset the elapsed time for the while loop
      textstorage(1); //store character
      dospace(); //do a space
    }
  }
  if (positionintypedtext==139){ //if the end of the stored text has been reached,
    promptkeypress=false; //reset keypress detection
    cursorx=0; //set cursor to the beginning of first row
    cursory=0;
    display.setTextCursor(cursorx,cursory);
    display.write("Msg end. <>=back"); //print this out to the lcd
    cursorx=0; //set cursor to the beginning of second row
    cursory=1;
    display.setTextCursor(cursorx,cursory);
    display.write("enter=serial out"); //print this out to the lcd
    rowshigh(); //sets all rows high
    numpressed=16; //reset "numpressed" (16 doesn't refer to any button on the pad)
    while(!promptkeypress){ //while no relevant keypresses have occurred, 
      if (findpress()){ //look for presses, if one has occurred, identify it and continue
        timespressed=0; //reset "timespressed"
        if (numpressed==14){ //if the arrows on the pad were pressed,
          promptkeypress=true; //take note so that the while loop can be broken
          textstorage(3); //perform a backspace in storage
          for (int i=0;i<16;i++){ //print out to the first line on the lcd from the stored text
            cursorx=i;
            cursory=0;
            display.setTextCursor(cursorx,cursory);
            display.write(typedtext[108+i]); //print this out to the lcd
          }
          for (int j=0;j<16;j++){ //print out to the second line on the lcd from the stored text
            cursorx=j;
            cursory=1;
            display.setTextCursor(cursorx,cursory);
            display.write(typedtext[123+j]); //print this out to the lcd
          }
          cursorx=15; //set cursor to the beginning of second row
          cursory=1;
          display.setTextCursor(cursorx,cursory);
          rowshigh(); //sets all rows high
        }
        if (numpressed==15){ //if enter on the pad was pressed,
          promptkeypress=true; //take note so that the while loop can be broken
          Serial.print("Final message: "); //print this to the serial console
          Serial.println(typedtext); //print out all the text typed so far to the serial console
          Serial.println(); //print a blank line
          for (int i=0;i<140;i++){ //write all positions in the stored text to be blank
            typedtext[i]=' ';
          }
          positionintypedtext=0; //reset the position in the stored text to the beginning
          doclear();
          rowshigh(); //sets all rows high
        }
      }
    }
    delay(preventholddelay); //delay a little to prevent continuous cycling
  }
}

void rowshigh(){ //sets all rows high
  digitalWrite(row1,HIGH); //write all the rows high
  digitalWrite(row2,HIGH); 
  digitalWrite(row3,HIGH);
  digitalWrite(row4,HIGH);
}

bool findpress(){ //finds a press to define "numpressed", if any press occurs, returns true
  bool pressfound=false; //variable for any press detection, is returned by this function

  digitalWrite(row1,LOW); //write all rows low
  digitalWrite(row2,LOW);
  digitalWrite(row3,LOW);
  digitalWrite(row4,LOW);

  digitalWrite(row1,HIGH); //write first row high
  if (digitalRead(col1)==HIGH){ //if the first column is now high, "1" has been pressed
    numpressed = 1;
    pressfound=true;
  }
  if (digitalRead(col2)==HIGH){ //if the second column is now high, "2" has been pressed
    numpressed = 2;
    pressfound=true;
  }
  if (digitalRead(col3)==HIGH){ //if the third column is now high, "3" has been pressed
    numpressed = 3;
    pressfound=true;
  }
//  if (digitalRead(col4)==HIGH){ //if the fourth column is now high, "reset" has been pressed
//    numpressed = 12;
//    pressfound=true;
//  }
  digitalWrite(row1,LOW); //return first row low

  digitalWrite(row2,HIGH); //write second row high
  if (digitalRead(col1)==HIGH){ //if the first column is now high, "4" has been pressed
    numpressed = 4;
    pressfound=true;
  }
  if (digitalRead(col2)==HIGH){ //if the second column is now high, "5" has been pressed
    numpressed = 5;
    pressfound=true;
  }
  if (digitalRead(col3)==HIGH){ //if the third column is now high, "6" has been pressed
    numpressed = 6;
    pressfound=true;
  }
//  if (digitalRead(col4)==HIGH){ //if the fourth column is now high, "dial" has been pressed
//    numpressed = 13;
//    pressfound=true;
//  }
  digitalWrite(row2,LOW); //return second row low

  digitalWrite(row3,HIGH); //write third row high
  if (digitalRead(col1)==HIGH){ //if the first column is now high, "7" has been pressed
    numpressed = 7;
    pressfound=true;
  }
  if (digitalRead(col2)==HIGH){ //if the second column is now high, "8" has been pressed
    numpressed = 8;
    pressfound=true;
  }
  if (digitalRead(col3)==HIGH){ //if the third column is now high, "9" has been pressed
    numpressed = 9;
    pressfound=true;
  }
//  if (digitalRead(col4)==HIGH){ //if the fourth column is now high, the arrows have been pressed
//    numpressed = 14;//
//    pressfound=true;
//  }
  digitalWrite(row3,LOW); //return third row low

  digitalWrite(row4,HIGH); //write fourth row high
  if (digitalRead(col1)==HIGH){ //if the first column is now high, "shift" has been pressed
    numpressed = 10;//space
    pressfound=true;
  }
  if (digitalRead(col2)==HIGH){ //if the second column is now high, "0" has been pressed
    numpressed = 11;//
    pressfound=true;
  }
  if (digitalRead(col3)==HIGH){ //if the third column is now high, "." has been pressed
    numpressed = 14;//backspace
    pressfound=true;
  }
//  if (digitalRead(col4)==HIGH){ //if the fourth column is now high, "enter" has been pressed
//    numpressed = 15;//
//    pressfound=true;
//  }
  digitalWrite(row4,LOW); //return fourth row low

  rowshigh(); //write all rows high

    return pressfound; //function returns true if any press found, otherwise returns false
}

void definepress(){ //uses "lastnumpressed" and "timespressed" to define "letter"
  if (lastnumpressed==1){   //1
    if (timespressed==1){
      letter='@';
    }
    if (timespressed==2){
      letter='#';
    }
    if (timespressed==3){
      letter='>';
    }
    if (timespressed==4){
      letter='<';
    }
    if (timespressed==5){
      letter='1';
    }
  }
  if (lastnumpressed==2){   //2
    if (timespressed==1){
      letter='A';
    }
    if (timespressed==2){
      letter='B';
    }
    if (timespressed==3){
      letter='C';
    }
    if (timespressed==4){
      letter='2';
    }
  }
  if (lastnumpressed==3){   //3
    if (timespressed==1){
      letter='D';
    }
    if (timespressed==2){
      letter='E';
    }
    if (timespressed==3){
      letter='F';
    }
    if (timespressed==4){
      letter='3';
    }
  }
  if (lastnumpressed==4){   //4
    if (timespressed==1){
      letter='G';
    }
    if (timespressed==2){
      letter='H';
    }
    if (timespressed==3){
      letter='I';
    }
    if (timespressed==4){
      letter='4';
    }
  }
  if (lastnumpressed==5){   //5
    if (timespressed==1){
      letter='J';
    }
    if (timespressed==2){
      letter='K';
    }
    if (timespressed==3){
      letter='L';
    }
    if (timespressed==4){
      letter='5';
    }
  }
  if (lastnumpressed==6){   //6
    if (timespressed==1){
      letter='M';
    }
    if (timespressed==2){
      letter='N';
    }
    if (timespressed==3){
      letter='O';
    }
    if (timespressed==4){
      letter='6';
    }
  }
  if (lastnumpressed==7){   //7
    if (timespressed==1){
      letter='P';
    }
    if (timespressed==2){
      letter='Q';
    }
    if (timespressed==3){
      letter='R';
    }
    if (timespressed==4){
      letter='S';
    }
    if (timespressed==5){
      letter='7';
    }
  }
  if (lastnumpressed==8){   //8
    if (timespressed==1){
      letter='T';
    }
    if (timespressed==2){
      letter='U';
    }
    if (timespressed==3){
      letter='V';
    }
    if (timespressed==4){
      letter='8';
    }
  }
  if (lastnumpressed==9){   //9
    if (timespressed==1){
      letter='W';
    }
    if (timespressed==2){
      letter='X';
    }
    if (timespressed==3){
      letter='Y';
    }
    if (timespressed==4){
      letter='Z';
    }
    if (timespressed==5){
      letter='9';
    }
  }
  if (lastnumpressed==11){    //11 (0)
    if (timespressed==1){
      letter='.';
    }
    if (timespressed==2){
      letter='?';
    }
    if (timespressed==3){
      letter='!';
    }
    if (timespressed==4){
      letter=',';
    }
    if (timespressed==5){
      letter='(';
    }
    if (timespressed==6){
      letter=')';
    }
    if (timespressed==7){
      letter='-';
    }
    if (timespressed==8){
      letter='0';
    }
  }
}

void incrementtimespressed(){ //increment "timespressed" until at max value stored in maxtimespressed for that lastnumpressed, then roll over to 1
  if (timespressed==maxtimespressed[lastnumpressed]){ //if at the maximum,
    timespressed=1; //roll over timespressed to one
  }
  else{ //otherwise,
    timespressed++; //increment timespressed
  }
}

void dozero(){ //prints zero
  display.write('0'); //print 0
  display.setTextCursor(cursorx,cursory);//maintain cursor position
  dospace(); //space
}

void dospace(){ //moves cursor forward once, wraps to next line if necessary, clears and returns to top of display if at bottom
  if (cursory==56){ //if on the bottom row,
    if (cursorx==128){ //if at the end of the row,
      cursorx=0; //define new cursor position as the upper-left corner
      cursory=0;
      display.clear(); //clear the lcd
    }
    else{ //otherwise,
      cursorx = cursorx + 8; //increment the cursor to the right
    }
  }
  else{ //if on the top row,
    if (cursorx == 120){ //if at the end of the row,
      cursorx=0; //define new cursor position as the bottom-left corner
      cursory = cursory + 8;
    }
    else{ //otherwise,
      cursorx = cursorx + 6; //increment the cursor to the right
    }
  }
  rowshigh(); //sets all rows high
  if (disablespacedelay){ //if the delay has been disabled,
    disablespacedelay=false; //reset its being disabled
  }
  else{ //otherwise,
    delay(preventholddelay); //delay a bit
  }
  while(digitalRead(col1)==HIGH)//while space button is pressed
  {
    //hold
  }
  
}

void doclear(){ //clears and returns to top-left of display
  cursorx=0;
  cursory=0;
  display.clear();
  display.setTextCursor(cursorx,cursory);
  rowshigh(); //sets all rows high
  delay(preventholddelay);
}

void dobackspace(){ //does a backspace, essentially the opposite of dospace
  if (cursory > 8 && cursorx == 0){ //if on any row besides top and at the beginning of row
    cursorx= 128;
    cursory=cursory - 8;
    textstorage(3);
    }
    
  if(cursory == 0 && cursorx == 0){ //if at top row and at beginning of row
    if(cursorx == 0){
      //do nothing
    }
  }
  else{                             //otherwise do regular backspace
    cursorx = cursorx - 6;
    textstorage(3);
  }

  display.clear();
  display.printFixed(0,  0, typedtext, STYLE_NORMAL);
  display.setTextCursor(cursorx,cursory);
  rowshigh(); //sets all rows high
  delay(preventholddelay);
}

void textstorage(byte mode){ //contains functions for text storage
  if (mode==1){ //regular character storage
    typedtext[positionintypedtext]=letter; //store letter that was printed to OLED in typedtext
    keypad_input_num[positionintypedtext]=letter;
    positionintypedtext++; //increment position in typedtext
   
  }
  if (mode==2){ //do a space in stored text
    typedtext[positionintypedtext]=' '; //set current position in typedtext to a space
    positionintypedtext++; //increment position in typedtext
  }
  if (mode==3){ //does a backspace in the stored text
    positionintypedtext--; //decrement position in typedtext
    typedtext[positionintypedtext]='\0'; //set current position in typedtext to a space
    keypad_input_num[positionintypedtext]='\0';
  }
}

void outputserial(){ //output message and display remaining characters to serial console
  Serial.print("Message: ");
  Serial.println(typedtext); //print out all the text typed so far to the serial console
  charremaining=(139-(positionintypedtext)); //calculate remaining characters
  Serial.print(charremaining); //display remaining characters
  if (charremaining==1){ //making sure that the plural is only used correctly
    Serial.println(" character remaining");
  }
  else{
    Serial.println(" characters remaining");
  }
  Serial.println(); //print a blank line
  doclear();
  delay(preventholddelay); //delay a little to prevent continuous cycling
}
