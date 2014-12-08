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

#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )

void setup()
{
  LCDA.initDriverPin(2,7,10); 
  LCDA.Initialise(); // INIT SCREEN  
   LCDA.CLEAR();
  delay(1000);
  displayfuse(angryFace);

  delay(5000);
   LCDA.CLEAR();
  delay(100);

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
  switch (input){
   case 0: return g0;
   case 1: return g1;
   case 2: return g2;
   case 3: return g3;
   case 4: return g4;
   case 5: return g5;
   case 6: return g6;
   case 7: return g7;
   case 8: return g8;
   case 9: return g9;
   case 10: return plus;
   case 11: return minus;
   case 12: return times;
   case 13: return g1;
   case 14: return g1;
   case 15: return g1;
   default: return g1;
  }
}
void displayfuse(int number){
 LCDA.CLEAR();
   delay(100);

 unsigned char fuse[8192];
 int mask1 = 0xF000;
 int mask2 = 0x0F00;
 int mask3 = 0x00F0;

 int firstnumber = number&mask1>>3;
 int operation = number&mask2>>2;
 int secondnumber= number&mask3>>1;
  
  LCDA.DrawFullScreen(fuse3(getMatch(firstnumber),getMatch(operation),getMatch(secondnumber)));
  delay(5000);
}
void loop(){
  displayfuse(0x1234);
  delay(1000);
// LCDA.DrawFullScreen(angry);



}

