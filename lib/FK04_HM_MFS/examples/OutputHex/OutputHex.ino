#include <Arduino.h>
#include <mfs.h>

int main(void)
{
  int a=0;
  unsigned long old_ts;
  init();
  initMFS();

  old_ts=millis();
  while (1) {
    writeHexToSegment(a);
    if (millis()-old_ts > 500) {
      old_ts=millis();
      a++;
    }
  }
  return 0;
}
