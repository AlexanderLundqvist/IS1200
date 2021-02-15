/* time4io.c

   This file written 2021 by Alexander Lundqvist
   Part lab 3 in IS1200.

 */

#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

// Shift then mask to give the data bits for SW4-SW1
int getsw(){
  return ((PORTD>>8) & 0x000f);
}


int getbtns(){
  return ((PORTD>>5) & 0x0007);
}
