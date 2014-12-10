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
void setup(){
Serial.begin(9600);

  delay(7000);

  Serial.write("7\n");
   LCDA.initDriverPin(2,7,10); 
  LCDA.Initialise(); // INIT SCREEN  
  Serial.write("8");


}
unsigned char* fuse3(const unsigned char* a ,const unsigned char* b,const unsigned char* c){
 unsigned char* fused =(unsigned char*)malloc(sizeof(char)*1024);
  int i=0;
  int ac=0;//a counter 
  int bc=0; //b counter 
  int cc=0;//c counter 
  Serial.write("3");

  for(int n=0;n<64;n++){
    Serial.print("i=");
    Serial.print(i);
    Serial.print("n=");
    Serial.println(n);
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
    Serial.write("5");

  return fused;
}
const unsigned char* getMatch(int input){
  return g1;
}
void displayfuse(int number){
 LCDA.CLEAR();
   delay(100);
 Serial.write("0");
  int mask1 = 0xF000;
 int mask2 = 0x0F00;
 int mask3 = 0x00F0;
int firstnumber = number&mask1>>12;
 int operation = number&mask2>>8;
 int secondnumber= number&mask3>>4;
 Serial.print(firstnumber);
 Serial.print(" ");
 Serial.print(operation);
 Serial.print(" ");
 Serial.print(firstnumber);
 Serial.println(" ");
 const unsigned char* a = getMatch(firstnumber);
 const unsigned char* b = getMatch(firstnumber);
 const unsigned char* c = getMatch(firstnumber);

 const unsigned char* d = fuse3(a,b,c);
 Serial.write("drawing");
  LCDA.DrawFullScreen(d);
  delay(5000);
}

void loop(){
displayfuse(0x1234);

}

