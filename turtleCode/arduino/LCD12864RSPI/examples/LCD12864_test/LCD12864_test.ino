/***********************************************
1. SPI Interface Inatruction
      clockPin --> SCK(EN)
      latchPin --> CS(RS)
      dataPin --> SID(RW)
 2. Connection:
    1)Turn the BL_ON Switch to the "ON" side;
    2)Turn the PBS_ON Switch to the "SPI" side

Method1:
      LCD                   Arduino
      EN                 Digital Pin 2
      RS                 Digital Pin 7
      RW                 Digital Pin 10
      VCC                     5V
      GND                     GND;

Method2:
      LCD                          Arduino
      SCK       clockPin(defined in the "initDriverPin" function)
      CS        latchPin(defined in the "initDriverPin" function)
      SID       dataPin (defined in the "initDriverPin" function)
      VCC                            5V
      GND                           GND
***********************************************/

#include "LCD12864RSPI.h"
#include "DFrobot_bmp.h"
#include "DFrobot_char.h"

unsigned char fuse[1024];
void setup()
{  Serial.begin(9600);

  delay(20000);

  Serial.write("7");
 // delay(100);
//  Serial.write("5");
  LCDA.initDriverPin(2,7,10); 



  LCDA.Initialise(); // INIT SCREEN  
 
}
unsigned char* fuse3(unsigned char* a ,unsigned char* b,unsigned char* c){
 unsigned char* fused =(unsigned char*)malloc(sizeof(char)*1024);
  int i=0;
  int ac=0;//a counter 
  int bc=0; //b counter 
  int cc=0;//c counter 

  for(int n=0;n<64;n++){
    for(int k=0;k<5;k++){
      fused[i]=a[ac];
      ac++;
       i++;
    }
        for(int k=0;k<5;k++){
      fused[i]=b[bc];
      bc++;
       i++;
    }
        for(int k=0;k<5;k++){
      fused[i]=c[cc];
      cc++;
       i++;
    }
    fused[i]=0x00;
    i++;
  }
  return fused;
}
unsigned char* getMatch(int input){

  input = input & 0x000f;
  Serial.write(input);

if(input == 2)
{
  Serial.write("2");
  return g1;
}

  unsigned char* ret =g0;
  switch (input){
   case 0: ret =g0; break;
   case 1: ret =g1; break;
   case 2: ret =g2;break;
   case 3: ret =g3;break;
   case 4: ret =g4;break;
   case 5: ret =g5;break;
   case 6: ret =g6;break;
   case 7: ret =g7;break;
   case 8: ret =g8;break;
   case 9: ret =g9;break;
   case 10: ret =plus;break;
   case 11: ret =minus;break;
   case 12: ret =times;break;
   case 13: ret =g1;break;
   case 14: ret =g1;break;
   case 15: ret =g1;break;
  }
   return ret;
  }

void getMatch1()
{
  Serial.write("hello1\n");
}

void displayfuse(int number){
 LCDA.CLEAR();
   delay(100);
 Serial.write("fuse");
 int mask1 = 0xF000;
 int mask2 = 0x0F00;
 int mask3 = 0x00F0;

 int firstnumber = number&mask1>>12;
 int operation = number&mask2>>8;
 int secondnumber= number&mask3>>4;
  getMatch(2);
  LCDA.DrawFullScreen(fuse3(getMatch(firstnumber),getMatch(operation),getMatch(secondnumber)));
  delay(1000);
}
void loop(){
  delay(100);
displayfuse(0x1234);
  //getMatch1();
  Serial.write("\nlloo");
  delay(1000);
}

