#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw( void ) {
  int switches = PORTD >> 8;
  switches  = (switches & 0x0F);
  return switches;
}

int getbtns( void ) {
  int buttons = PORTD >> 5;
  buttons = (buttons & 0x07);
  return buttons;
}
