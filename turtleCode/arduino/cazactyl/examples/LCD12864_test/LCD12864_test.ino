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
#include <avr/pgmspace.h>

unsigned char fuse[1024];
void setup(){
 
Serial.begin(9600);

delay(100);
   LCDA.initDriverPin(2,7,10); 
  LCDA.Initialise(); // INIT SCREEN  


}
unsigned char* fuse3(const unsigned char* a ,const unsigned char* b,const unsigned char* c){
  
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
}
const unsigned char* getMatch(int input){
 if(input>0||input<13){
  return index[input];
 }
  
   return index[0];
}
void displayfuse(int firstnumber, int operation , int secondnumber ){
 LCDA.CLEAR();
   delay(100);

 const unsigned char* a = getMatch(firstnumber);
 const unsigned char* b = getMatch(operation);
 const unsigned char* c = getMatch(secondnumber);

 const unsigned char* d = fuse3(a,b,c);
  LCDA.DrawFullScreen(d);
  delay(5000);
}

void loop(){
displayfuse(1,11,4);

}

