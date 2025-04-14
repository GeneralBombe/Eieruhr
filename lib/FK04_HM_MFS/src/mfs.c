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
const uint8_t SevenSegmentASCII[96] = {
    0x00, /* (space) */
    0x86, /* ! */
    0x22, /* " */
    0x7E, /* # */
    0x6D, /* $ */
    0xD2, /* % */
    0x46, /* & */
    0x20, /* ' */
    0x29, /* ( */
    0x0B, /* ) */
    0x21, /* * */
    0x70, /* + */
    0x10, /* , */
    0x40, /* - */
    0x80, /* . */
    0x52, /* / */
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F, /* 9 */
    0x09, /* : */
    0x0D, /* ; */
    0x61, /* < */
    0x48, /* = */
    0x43, /* > */
    0xD3, /* ? */
    0x5F, /* @ */
    0x77, /* A */
    0x7C, /* B */
    0x39, /* C */
    0x5E, /* D */
    0x79, /* E */
    0x71, /* F */
    0x3D, /* G */
    0x76, /* H */
    0x30, /* I */
    0x1E, /* J */
    0x75, /* K */
    0x38, /* L */
    0x15, /* M */
    0x37, /* N */
    0x3F, /* O */
    0x73, /* P */
    0x6B, /* Q */
    0x33, /* R */
    0x6D, /* S */
    0x78, /* T */
    0x3E, /* U */
    0x3E, /* V */
    0x2A, /* W */
    0x76, /* X */
    0x6E, /* Y */
    0x5B, /* Z */
    0x39, /* [ */
    0x64, /* \ */
    0x0F, /* ] */
    0x23, /* ^ */
    0x08, /* _ */
    0x02, /* ` */
    0x5F, /* a */
    0x7C, /* b */
    0x58, /* c */
    0x5E, /* d */
    0x7B, /* e */
    0x71, /* f */
    0x6F, /* g */
    0x74, /* h */
    0x10, /* i */
    0x0C, /* j */
    0x75, /* k */
    0x30, /* l */
    0x14, /* m */
    0x54, /* n */
    0x5C, /* o */
    0x73, /* p */
    0x67, /* q */
    0x50, /* r */
    0x6D, /* s */
    0x78, /* t */
    0x1C, /* u */
    0x1C, /* v */
    0x14, /* w */
    0x76, /* x */
    0x6E, /* y */
    0x5B, /* z */
    0x46, /* { */
    0x30, /* | */
    0x70, /* } */
    0x01, /* ~ */
    0x00, /* (del) */
};
const uint8_t SEGMENT_SELECT[] = {0x1, 0x2, 0x4, 0x8};

const size_t SEGMENT_MAP_LEN = sizeof(SEGMENT_MAP) / sizeof(uint8_t);

void initMFS(void)
{
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(POTI, INPUT);

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
  int i, end = 0;
  bool ok = true;
  if (val < 0)
  {
    end = 1;
    val = -val;
  }

  for (i = 3; ok && i >= end; --i)
  {
    writeCharToSegment(i, val % 10, false);
    val /= 10;
    if (val == 0)
      ok = false;
  }

  if (end == 1) // number was negative
    writeCharToSegment(i--, SEGMENT_DASH, false);

  // convert leading zeros to blanks
  for (; i >= 0; --i)
    writeCharToSegment(i, SEGMENT_CLEAR, false);
}

void writeHexToSegment(unsigned val)
{
  int i;

  for (i = 3; i >= 0; --i)
  {
    writeCharToSegment(i, val % 16, false);
    val /= 16;
  }
}

void writeCharToSegment(uint8_t segment, uint8_t value, int dot)
{
  uint8_t dataout = SEGMENT_MAP[SEGMENT_MAP_LEN - 1];

  if (value >= 0 && value < SEGMENT_MAP_LEN)
  {
    dataout = SEGMENT_MAP[value];
  }
  else
  {
    dataout = ~SevenSegmentASCII[value - 32]; // ASCII to segment map
  }

  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, dataout & (dot ? 0x7F : 0xFF));
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[segment]);
  digitalWrite(LATCH_DIO, HIGH);
}
