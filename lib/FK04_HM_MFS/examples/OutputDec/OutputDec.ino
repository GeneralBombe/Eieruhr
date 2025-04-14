#include <Arduino.h>
#include <mfs.h>

int main(void)
{
  int a=-1010;
  unsigned long old_ts;
  init();
  initMFS();

  old_ts=millis();
  while (1) {
    writeDecToSegment(a);
    if (millis()-old_ts > 100) {
      old_ts=millis();
      a++;
    }
  }
  return 0;
}
