
/***********************************************
 * 1. SPI Interface Inatruction
 * clockPin --> SCK(EN)
 * latchPin --> CS(RS)
 * dataPin --> SID(RW)
 * 2. Connection:
 * 1)Turn the BL_ON Switch to the "ON" side;
 * 2)Turn the PBS_ON Switch to the "SPI" side
 * 
 * 
 * LCD                          Arduino
 * SCK       clockPin(defined in the "initDriverPin" function)  port 2 
 * CS        latchPin(defined in the "initDriverPin" function)  port 3 
 * SID       dataPin (defined in the "initDriverPin" function)  port 4 
 * VCC                            5V
 * GND                           GND
 * 
 * 
 ***********************************************/

/*********************************
 * Manual for sending math stuff:
 * 
 * 1: Serial receives an "s" as the first character to indicate a sum is coming.
 * 2:  first number after the s can be anything from 0 up to and including 9.
 * second number after the s should be either ":", ";" or "<". these signal +, - and * respectively.
 * third number after the s can be anything from 0 up to and including 9.
 *********************************/

#include "LCD12864RSPI.h"
#include "DFrobot_bmp.h"
#include "LCD12864_test.h"
#include "DFrobot_char.h"
#include <avr/pgmspace.h>
#include <SPI.h>
#include "MFRC522.h"

#include <Servo.h>

#define RST_PIN	        9		
#define SS_PIN		      10	
#define servopin        6
#define tailpin         5
#define mouth_open_pos    70
#define mouth_wait_pos    55
#define mouth_closed_pos  0
#define mouth_semiclosed_pos  30

#define wiglebig  120
#define wiglesmall  60

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance
MFRC522::MIFARE_Key key;
byte cardUID;

Servo mouthservo; //our servo
Servo tailservo; //our servo

byte lb1,lb2,lb3,lb4; 

int answer;
int input;
unsigned char fuse[1024];
int wrong = 0;

void longboot(){
  displaySingle('n','e','u');
  fullcloseMouth();

  wiggle(5000);
}
void setup(){ //////////////SETUP////////////////////////
  pinMode(12, OUTPUT);     
  digitalWrite(12, HIGH); 
  Serial.begin(9600);
  mouthservo.attach(servopin);
  tailservo.attach(tailpin);
  SPI.begin();
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  readloop(100);
  LCDA.initDriverPin(2,3,4); 
  LCDA.Initialise(); // INIT SCREEN  

  longboot();
  semiclose();
}

void openMouth(){ //open the mouth
  for(int i = mouthservo.read(); i < mouth_open_pos; i++){
    mouthservo.write(i);
    delay(8);
  }
}

int wiglespeed =12;
void wiggle(int mseconds){
  long starttime= millis();
  int  keepgoing=1;
  while(keepgoing){
   
    for(int i = tailservo.read(); i < wiglebig; i++){
            tailservo.write(i);
            delay(wiglespeed);
            
            if(starttime+mseconds<millis()){
              keepgoing=0;break;}
      }
      
          for(int i = tailservo.read(); i > wiglesmall; i--){
            tailservo.write(i);
            delay(wiglespeed);
            if(starttime+mseconds<millis())
             {
              keepgoing=0;break;}
      }
  }
}

void waitMouth(){ //open the mouth

    for(int i = mouthservo.read(); i < mouth_wait_pos; i++){
    mouthservo.write(i);
    delay(15);
  }
  for(int i = mouthservo.read(); i > mouth_wait_pos; i--){
    mouthservo.write(i);
    delay(15);
  }

}
void semiclose(){
  for(int i = mouthservo.read(); i < mouth_semiclosed_pos; i++){
    mouthservo.write(i);
    delay(15);
  }
  for(int i = mouthservo.read(); i > mouth_semiclosed_pos; i--){
    mouthservo.write(i);
    delay(15);
  }
}
void fullcloseMouth(){
  for(int i = mouthservo.read(); i > mouth_closed_pos; i--){
    mouthservo.write(i);
    delay(15);
  }

}
void closeMouth(){ //close the mouth
  int repeat= 3;
  for(int i=0;i<repeat;i++){
    fullcloseMouth();
    semiclose();
  }
  fullcloseMouth();
}



const unsigned char* getMatch(int input){ //getmatch function
  if(input>0||input<13){
    return index[input];
  }  
  return index[0];
}


void displayfuse(int firstnumber, int operation , int secondnumber ){
  LCDA.CLEAR();
  readloop(100); //for refreshing delay?

  const unsigned char* a = getMatch(firstnumber);
  const unsigned char* b = getMatch(operation);
  const unsigned char* c = getMatch(secondnumber);


  LCDA.DrawFullScreen_F3(a,b,c);
  readloop(5000); //for refreshing delay?
}

int getAnswer(int firstnumber, int operation , int secondnumber){
  switch(operation){
  case 'D':
    answer = firstnumber + secondnumber;
    break;
  case 'E':
    answer = firstnumber - secondnumber;
    break;
  case 'F':
    answer = firstnumber * secondnumber;
    break;
  }
  Serial.println();
  return answer;
}


void checkInput(int input, int answer){
  if (input == answer){
    displaySingle('h', 'a', 'p'); //show happy face (when implemented)
    writeserial("c");
    wrong = 0;
    closeMouth();
    wiggle(1000);
    //    openMouth();
    Serial.flush();
  }
  else{
    wrong++;
    displaySingle('s','a','d');

    writeserial("w");

    if(wrong>5){
      writeserial("n");
    }
    openMouth();
    delay(2000);
    waitMouth();
    displayfuse(lb2-48,lb3-47,lb4-48);
  }
}

void displaySingle(char a,char b , char c){
  char str[] = {
    a,b,c,(char)NULL  };
  if(strcmp(str,"anr")==0){
    LCDA.DrawFullScreen(angryFace); 
  }
  if(strcmp(str,"hap")==0){
    LCDA.DrawFullScreen(happyFace); 
  }
  if(strcmp(str,"sad")==0){
    LCDA.DrawFullScreen(sadFace); 
  }
  if(strcmp(str,"neu")==0){
    LCDA.DrawFullScreen(neutralFace); 
  }

}


void readrfid(){ //read rfid ball!
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // that is: sector #1, covering block #4 up to and including block #7
  byte blockAddr      = 4; // Reading Block 4 (Sector 1)
  byte trailerBlock   = 4; 
  byte status;
  byte buffer[18];
  byte size = sizeof(buffer);

  // Authenticate using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    //Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Authenticate using key B
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    //Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Read data from the block
  //Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
  //Serial.println(F(" ..."));
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
  }
  //Serial.print(F("Data in block "));
  //Serial.print(blockAddr);
  //Serial.println(F(":"));


  //Serial.println(input);
  //Serial.println(answer);

  //dump_byte_array(buffer, 1);
  //Serial.println();

  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  //Best workaround ever

  input = buffer[0];

  checkInput(input, answer);


} 


void dump_byte_array(byte *buffer, byte bufferSize) { //dump byte array function
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
   // Serial.print(buffer[i], HEX);
  }
}

boolean confirmed;
String lastmsg; 

void writeserial(char* input){
  lastmsg=input;

  while(1){ 
  Serial.println(input); //hugo> Took print out of the while loop to prevent overflow of information towards BBB thijs> thus trapping in an endless loop to ensure the arduino crashes instead?
  for(int i=0;i<20;i++){
     if(Serial.available()>=4){
      byte b1,b2,b3,b4; 
      delay(200);
      b1=Serial.read();
      if(b1=='c'){
        b2=Serial.read();
        b3=Serial.read();
        b4=Serial.read();
        if(b2=='o'&&b3=='n'&&b4=='f'){
          return;
        }
      }
     }
    }
    delay(800); 
  }
}

void loop(){
  if(Serial.available()>=4){
    
    lb1=Serial.read();
    if(lb1=='s'){ //indicates a math problem is coming
      lb2=Serial.read();
      lb3=Serial.read();
      lb4=Serial.read();    
      answer = getAnswer(lb2-48, lb3, lb4-48);

      waitMouth();
      displayfuse(lb2-48,lb3-48,lb4-48); //fuses the three numbers after the s to a math problem.

    }
    else{
      if(lb1=='f'){
        lb2=Serial.read();
        lb3=Serial.read();
        lb4=Serial.read();    
        displaySingle(lb2,lb3,lb4);
      }
      else{
            
      }
    }
  }

  readrfid();




}

void readloop(int length){
  long starttime= millis();
  while(millis()-starttime<length){
    readrfid();
  }
}

