/***********************************************
1. SPI Interface Inatruction
      clockPin --> SCK(EN)
      latchPin --> CS(RS)
      dataPin --> SID(RW)
 2. Connection:
    1)Turn the BL_ON Switch to the "ON" side;
    2)Turn the PBS_ON Switch to the "SPI" side


      LCD                          Arduino
      SCK       clockPin(defined in the "initDriverPin" function)  port 2 
      CS        latchPin(defined in the "initDriverPin" function)  port 3 
      SID       dataPin (defined in the "initDriverPin" function)  port 4 
      VCC                            5V
      GND                           GND
      

***********************************************/

/*********************************
Manual for sending math stuff:

1: Serial receives an "s" as the first character to indicate a sum is coming.
2:  first number after the s can be anything from 0 up to and including 9.
    second number after the s should be either ":", ";" or "<". these signal +, - and * respectively.
    third number after the s can be anything from 0 up to and including 9.
*********************************/

#include "LCD12864RSPI.h"
#include "DFrobot_bmp.h"
#include "DFrobot_char.h"
#include <avr/pgmspace.h>
#include <SPI.h>
#include "MFRC522.h"

#include <Servo.h>

#define RST_PIN	        9		
#define SS_PIN		      10	
#define servopin        6

#define mouth_open_pos    120
#define mouth_closed_pos  40

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance
MFRC522::MIFARE_Key key;
byte cardUID;

Servo mouthservo; //our servo
int servo_pos = 0; //var to store servo position

unsigned char fuse[1024];

void setup(){ //////////////SETUP////////////////////////
  Serial.begin(9600);
  mouthservo.attach(servopin);
  SPI.begin();
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
    
  readloop(100);
  LCDA.initDriverPin(2,3,4); 
  LCDA.Initialise(); // INIT SCREEN  

  openMouth();

  //Serial.println(F("boot completed"));
}

void openMouth(){ //open the mouth
  mouthservo.write(mouth_open_pos);
}

void closeMouth(){ //close the mouth
  mouthservo.write(mouth_closed_pos);
}


/*unsigned char* fuse3(const unsigned char* a ,const unsigned char* b,const unsigned char* c){ /////////////////////////////////// IS THIS OBSOLETE? /////////
 unsigned char* fused =(unsigned char*)malloc(sizeof(char)*1024);
 int i=0;
 int ac=0;//a counter 
 int bc=0; //b counter 
 int cc=0;//c counter 

 for(int n=0;n<64;n++){
   for(int k=0;k<5;k++){
     memcpy_P(&fused[i],&a[ac],sizeof(char)*5);
     ac++;
      i++;
   }

     for(int k=0;k<5;k++){
      memcpy_P(&fused[i],&b[bc],sizeof(char)*5);
      bc++;
      i++;
     }
     for(int k=0;k<5;k++){
      memcpy_P(&fused[i],&c[cc],sizeof(char)*5);
      cc++;
      i++;
     }
    fused[i]=0x00;
    i++;
  }
  return fused;
}*/

const unsigned char* getMatch(int input){ //getmatch function
 if(input>0||input<13){
  return index[input];
 }  
 return index[0];
}


void displayfuse(int firstnumber, int operation , int secondnumber ){
 LCDA.CLEAR();
 readloop(100);

 const unsigned char* a = getMatch(firstnumber);
 const unsigned char* b = getMatch(operation);
 const unsigned char* c = getMatch(secondnumber);

// const unsigned char* d = fuse3(a,b,c);
// LCDA.DrawFullScreen(d);
  LCDA.DrawFullScreen_F3(a,b,c);
  readloop(5000);
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
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    //Serial.print(F("Data in block "));
    //Serial.print(blockAddr);
    //Serial.println(F(":"));
    dump_byte_array(buffer, 1);
    Serial.println();
    
        // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
}


void dump_byte_array(byte *buffer, byte bufferSize) { //dump byte array function
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

void loop(){
  
 mouthservo.write(40);
  
 int r1=0;
 int r2=0;
 int r3=0;
 int r4=0;
 byte b1,b2,b3,b4; 
 if(Serial.available()>3){
   b1=Serial.read();
   b2=Serial.read();
   b3=Serial.read();
   b4=Serial.read();     
   if(b1=='s'){ //indicates a math problem is coming
     displayfuse(b2-48,b3-48,b4-48); //fuses the three numbers after the s to a math problem.
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


