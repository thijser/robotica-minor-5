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

unsigned char wangzhi[]="4+4=";//

unsigned char en_char1[]="ST7920 LCD12864 ";// 

unsigned char en_char2[]="Test, Copyright ";// 

unsigned char en_char3[]="by DFRobot ---> ";// 

void setup()
{
  LCDA.initDriverPin(2,7,10); 
  LCDA.Initialise(); // INIT SCREEN  
  delay(100);
  LCDA.DrawFullScreen(logo);//LOGO
  delay(5000);
  LCDA.DrawFullScreen(angry);
  delay(5000);
  

}
int voidisplayfuse(int number){
  unsigned char fuse[8192];
 int mask1 = 0xF000;
 int mask2 = 0x0F00;
 int mask3 = 0x00F0;

 int firstnumber = number&mask1;
 int operation = number&mask2;
 int secondnumber= number&mask3;

}
void loop(){
LCDA.CLEAR();
//LCDA.DrawFullScreen(logo);//LOGO
  delay(5000);
  LCDA.CLEAR();//����

 // LCDA.DrawFullScreen(angry);
  delay(5000);
LCDA.CLEAR();//����
delay(100);
  delay(5000);


}

