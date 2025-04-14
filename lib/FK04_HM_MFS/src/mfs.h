#ifndef MFS_H
#define MFS_H

#include <stddef.h>
#include <stdint.h>

// based on http://arduinolearning.com/code/multi-function-shield-examples.php
#define LATCH_DIO 	4
#define CLK_DIO 	7
#define DATA_DIO 	8
#define LED1 	13
#define LED2 	12
#define LED3 	11
#define LED4 	10
#define S1 			A1
#define S2 			A2
#define S3 			A3
#define POTI 		A0
#define BUZZER 		3
#define MFS_LOW 	HIGH
#define MFS_HIGH 	LOW
#define BUZZER_OFF	HIGH
#define BUZZER_ON	LOW

#define SEGMENT_CLEAR 	(SEGMENT_MAP_LEN-1)
#define SEGMENT_DASH 	(SEGMENT_MAP_LEN-2)

#ifdef __cplusplus
extern "C" {
#endif

/* Constant declaration of size of SEGMENT_MAP-Array */
extern const size_t SEGMENT_MAP_LEN;

/* Init the  multi function shield */
void initMFS(void);

/* Write a decimal value between -999 to 9999 to the display */
void writeDecToSegment(int val);
/* Write a hex value between 0 to FFFF to the display */
void writeHexToSegment(unsigned val);

/* Write a char from the segment array to one of the 4 digits of the display incl. dot activation*/
void writeCharToSegment(uint8_t segment, uint8_t value, int dot);


#ifdef __cplusplus
}
#endif

#endif
