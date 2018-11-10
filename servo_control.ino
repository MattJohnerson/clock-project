#include <Arduino.h>
#include <SoftwareServo.h>

const byte commandMaxLen = 210;
char receivedCommand[commandMaxLen];
char tempCommand[commandMaxLen];

const byte numServos = 48;
SoftwareServo servo[numServos];
int targetServoPos[numServos];

boolean newCommandReceived = false;
unsigned long loopCounter = 0;

void setup() {
  Serial.begin(115200);
  //servo[0].attach(50);
}

void loop() {
  while(Serial.available() == 0){}
  receiveCommand();
  if(newCommandReceived == true){
    parseCommand();
    printCommand();
    Serial.print("Number of times loop executed since previous command: ");
    Serial.println(loopCounter);
    loopCounter = 0;
    newCommandReceived = false;
  }
  loopCounter++;
  
//  SoftwareServo::refresh();
//  curPos = servo[0].read();
//  if(curPos != pos){  
//    servo[0].write(pos);
//    delay(15);
//  }
//  Serial.println(curPos);
//  else{
//  if(pos >= 180 || pos <= 0){
//    d = -d;
//  }
//  pos = pos + d;
//  Serial.println(pos);
//  servo[0].write(pos);
//  SoftwareServo::refresh();
//  delay(15);
//  }
}

void receiveCommand(){
  static boolean incomingCommand = false;
  static byte index = 0;
  char commandStart = '[';
  char commandEnd = ']';
  char ch = '\0';

  while(Serial.available() > 0 && newCommandReceived == false){
    ch = Serial.read();
    if(incomingCommand == true){
      if(ch != commandEnd){
        receivedCommand[index] = ch;
        index++;
        if(index >= commandMaxLen){
          index = commandMaxLen - 1;
          Serial.println("here");
        }
      }
      else{
        receivedCommand[index] = '\0';
        incomingCommand = false;
        index = 0;
        newCommandReceived = true;
      }
    }
    else if(ch == commandStart){
      incomingCommand = true;
    }
  }  
  
}

void parseCommand(){
  strcpy(tempCommand, receivedCommand);
  char * strtokIndex;
  for(int i = 0; i < numServos; i++){
    if(i == 0){
      strtokIndex = strtok(tempCommand,",");
      targetServoPos[i] = atoi(strtokIndex);
    }
    else{
      strtokIndex = strtok(NULL,",");
      targetServoPos[i] = atoi(strtokIndex);
    }
  }
}

void printCommand(){
  Serial.print("Original command: ");
  Serial.println(receivedCommand);
  Serial.println();
  for(int i = 0; i < numServos; i++){
    Serial.print("Position ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(targetServoPos[i]);
  }
  Serial.println();
}
