#include <Arduino.h>
#include <stdio.h>
#include <stddef.h>
#include "mfs.h"


/* Segment byte maps for numbers 0 to 9  with ' '  and '-'*/
/* Explanation https://c2plabs.com/blog/2020/09/13/arduino-multi-function-shield-seven-segment-display/ */
const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, /* 0 - 9 */
                               0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 
                               0xBF, 0xFF};
/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0x1, 0x2, 0x4, 0x8};

const size_t SEGMENT_MAP_LEN=sizeof(SEGMENT_MAP)/sizeof(uint8_t);

void initMFS(void){
  pinMode(S1,INPUT);
  pinMode(S2,INPUT);
  pinMode(S3,INPUT);
  pinMode(POTI,INPUT);

  digitalWrite(BUZZER, BUZZER_OFF);
  digitalWrite(LED1, MFS_LOW);
  digitalWrite(LED2, MFS_LOW);
  digitalWrite(LED3, MFS_LOW);
  digitalWrite(LED4, MFS_LOW);
  
  pinMode(LATCH_DIO, OUTPUT);
  pinMode(CLK_DIO, OUTPUT);
  pinMode(DATA_DIO, OUTPUT);

  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, 0xFF);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, 0xFF);
  digitalWrite(LATCH_DIO, HIGH);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void writeDecToSegment(int val)
{
  int i, end=0;
  bool ok=true;
  if (val<0) {
	  end=1;
	  val=-val;
  }

  for (i=3; ok && i>=end; --i) {
  	writeCharToSegment(i, val % 10, false);
  	val/=10;
    if (val==0)
      ok=false;
  }
  
  if (end==1)		// number was negative
    writeCharToSegment(i--, SEGMENT_DASH, false);

  // convert leading zeros to blanks
  for(;i>=0;--i)
    writeCharToSegment(i, SEGMENT_CLEAR, false);
  
}

void writeHexToSegment(unsigned val)
{
  int i;
	
  for (i=3; i>=0; --i) {
	  writeCharToSegment(i, val % 16, false);
	  val/=16;
  }
  
}

void writeCharToSegment(uint8_t segment, uint8_t value, int dot)
{
  uint8_t dataout=SEGMENT_MAP[SEGMENT_MAP_LEN-1];

  if (value>=0 && value<SEGMENT_MAP_LEN)
		dataout=SEGMENT_MAP[value];

  digitalWrite(LATCH_DIO,LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, dataout & (dot ? 0x7F : 0xFF));
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[segment]);
  digitalWrite(LATCH_DIO,HIGH);
}
