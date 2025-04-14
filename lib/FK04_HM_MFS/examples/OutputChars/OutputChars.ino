#include <Arduino.h>
#include <mfs.h>

int main(void)
{
  int a;
  unsigned long old_ts;
  init();
  initMFS();


  old_ts=millis();
  while (1) {
    writeCharToSegment(0, 10, false);
    writeCharToSegment(1, a, true);   // out with dot
    writeCharToSegment(2, SEGMENT_CLEAR, false);
    writeCharToSegment(3, SEGMENT_DASH, false);
    
    if (millis()-old_ts > 100) {
      old_ts=millis();
      a++;
      if (a>9) {
        a=1;
      }
    }
  }
  return 0;
}
