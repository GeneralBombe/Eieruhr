#include "mfs.h"
#include <Arduino.h>
#define T_WAIT 50
int main(void) {
init();
initMFS();
int T = 2000; 
int wait_o = 0; 
while (1) {
wait_o = analogRead(POTI);
int wait_p = T - 1.9*wait_o; 
digitalWrite(LED2, MFS_HIGH);
delayMicroseconds(wait_o);
digitalWrite(LED2, MFS_LOW);
delayMicroseconds(wait_p);
}
return 0;
}