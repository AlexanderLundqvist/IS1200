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

int prime = 1234567;

char textstring[] = "text, more text, and even more text!";

volatile int *tris_E;
volatile int *port_E;

int timeoutcount = 0;


/* Interrupt Service Routine */
void user_isr( void ) {

  if(IFS(0) & 0x0100){ // Check if time out event
    timeoutcount++;

    if(timeoutcount == 10){
      time2string( textstring, mytime );
      display_string( 3, textstring );
      display_update();
      tick( &mytime );
      timeoutcount = 0;
    }

    IFSCLR(0) = 0x100; // Clear the flag
  }

  /* Surprise assignment */
  if(IFS(0) & 0x80){
    PORTE = PORTE + 0x1; // Increment value in PORTE

    IFSCLR(0) = 0x80; // Clear INT1IF
  }
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

  T2CON = 0x0;
  T2CONSET = 0x70;
	TMR2 = 0x0;
	PR2 = ((80000000 / 256) / 10);
	T2CONSET = 0x8000;

  /* Enabling interrupts */

  // Interrupt enable control. Bit 8 is T2IF.
  // Set it to 1 to enable.
  IECSET(0) = 0x100;

  // Interrupt priority. Bit 4-2 is T2IP.
  // Setting priority 7 and sub-priority 3.
  // bit 4-2: 0111 = 7
  // bit 1-0: 11 = 3
  // IPC2: 000111 11 = 0x1F
  IPCSET(2) = 0x1F;

  /* Surprise assignment */

  // Enable interrupt for external interrupt 1 (INT1IE)
  IECSET(0) = 0x080;

  // Set priority for INT1
  // bit 12-10: 110 for priority 6
  // bit 9-8: 11 for sub-priority 3
  // IPC1: 000 110 11 = 0x1b0
  IPCSET(1) = 0x1b00;

  enable_interrupt();


  return;
}

// This code is called repeatedly from mipslabmain
void labwork( void ) {
  prime = nextprime(prime);
  display_string(0, itoaconv(prime));
  display_update();
}
