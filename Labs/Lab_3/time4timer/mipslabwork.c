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

volatile int *tris_E;
volatile int *port_E;

int timeoutcount = 0;


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

  TRISD = TRISD | 0x0fe0; // OR to keep values at 11-5 bits


  /* Initialization of Timer2 */

  // Set ON-bit (bit 15) to zero to stop the clock
  T2CON = 0x0;

  // Use the set-register for t2con to set prescaling to 1:256 (70 = 0111 0000)
  // Bit 6-4 are the TCKPS that controls prescaling. 2^3 = 8 configurations.
  T2CONSET = 0x70;

  // Reset the timer register
	TMR2 = 0x0;

  // Period register. Set the value to (clock frequency)/(prescaling) and then
  // divide it by 10 because we want it to be 100ms delay, and we have that the
  // main time update and display code only executes per 10th interrupt.
  // The timer ticks up to this value then sets the interrupt flag in IFS0 to 1.
	PR2 = ((80000000 / 256) / 10);

  // Set ON-bit (bit 15) to one to start the clock
	T2CONSET = 0x8000;

  return;
}

// This code is called repeatedly from mipslabmain
void labwork( void )
{
  int switches = getsw();
	int button = getbtns();

  if(button & 1){
    mytime = mytime & 0xff0f;
    mytime = (switches << 4) | mytime;
  }

  if(button & 2){
    mytime = mytime & 0xf0ff;
    mytime = (switches << 8) | mytime;

  }

  if(button & 4){
    mytime = mytime & 0x0fff;
    mytime = (switches << 12) | mytime;

  }

  /* Checking the timeout event flag for T2 ( bit 8 in IFS0) */

  // AND with 0001 0000 0000, if the 8th bit is 1 then increment count
  if(IFS(0) & 0x100){
    timeoutcount++;

    // Reset the event flag bit with the clear register
    IFSCLR(0) = 0x100;

  }

  // The following statement only runs every 10th interrupt
  if(timeoutcount == 10){

    // delay( 1000 );
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
    display_image(96, icon);

    // Reset the counter
    timeoutcount = 0;

  }
}
