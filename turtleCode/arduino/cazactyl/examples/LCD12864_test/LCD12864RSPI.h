
//Arduino library v1.0
//Demo LCD12864 spi
//www.dfrobot.com

#ifndef LCD12864RSPI_h
#define LCD12864RSPI_h
#include <Arduino.h>


class LCD12864RSPI {
typedef unsigned char uchar;


public:

LCD12864RSPI();

void Initialise(void);
void initDriverPin(int clock,int latch,int data);
void delayns(void);

void WriteByte(int dat);
void WriteCommand(int CMD);
void WriteData(int CMD);


void CLEAR(void);
void DisplayString(int X,int Y,uchar *ptr,int dat);
void DisplaySig(int M,int N,int sig);
void DrawFullScreen(const uchar *p);
void img1(uchar img[]);
void img2(uchar img[]);

int delaytime;
int DEFAULTTIME;


//static const int latchPin = 8; 
//static const int clockPin = 3;  
//static const int dataPin = 9;  

int latchPin ; 
int clockPin ;  
int dataPin ;  
};
extern LCD12864RSPI LCDA;    
#endif
