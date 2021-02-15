/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2021-02-14 (lab 3) by Alexander Lundqvist

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declarations for these labs */

int mytime = 0x0001; // The starting time

char textstring[] = "text, more text, and even more text!";


// We need to declare the pointers as volatile because
volatile int *tris_E;
volatile int *port_E;



/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

void labinit( void )
{
  // Cast their respective adress as they were declared as volatile
  tris_E = (volatile int*) 0xbf886100; // Address of TRISE register
  port_E = (volatile int*) 0xbf886110; // Address of PORTE register


  *tris_E = *tris_E & 0xffffff00; // To mask out the 8 LSBs

  *port_E = 0x00000000; // Set PORTE to 0 so we can see the ticks better

  // e)
  TRISD = TRISD | 0x0fe0; // OR to keep values at 11-5 bits

  return;
}

void labwork( void )
{
  int switches = getsw();
	int button = getbtns();

  // Button 2
  if(button == 1 || button == 3 || button == 5 || button == 7){
    mytime = mytime & 0xff0f;
    mytime = (switches << 4) | mytime;
  }

  // Button 3
  if(button == 2 || button == 3 || button == 6 || button == 7){
    mytime = mytime & 0xf0ff;
    mytime = (switches << 8) | mytime;

  }

  // Button 4
  if(button == 4 || button == 5 || button == 6 || button == 7){
    mytime = mytime & 0x0fff;
    mytime = (switches << 12) | mytime;

  }

  delay( 1000 );
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  display_image(96, icon);

  *port_E = *port_E + 0x00000001; // d) Increment the value per tick
}
