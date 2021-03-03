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

#define DISPLAY_WIDTH = 16; // Bytes or 8 bits = 8 pixels
#define DISPLAY_HEIGHT = 4;
#define DISPLAY_SIZE = DISPLAY_WIDTH * DISPLAY_HEIGHT;


char textstring[] = "text, more text, and even more text!";

volatile int *tris_E;
volatile int *port_E;

int timeoutcount = 0;

/*
  Interrupt Service Routine handling the inputs from the buttons.

*/
void user_isr( void )
{
  return;
}

void labinit( void )
{
  tris_E = (volatile int*) 0xbf886100; // Address of TRISE register
  port_E = (volatile int*) 0xbf886110; // Address of PORTE register

  *tris_E = *tris_E & 0xffffff00; // To mask out the 8 LSBs
  *port_E = 0x00000000; // Set PORTE to 0 so we can see the ticks better

  /* Timer from lab 3 */
  TRISD = TRISD | 0x0fe0;
  T2CON = 0x0;
  T2CONSET = 0x70;
	TMR2 = 0x0;
	PR2 = ((80000000 / 256) / 10);
	T2CONSET = 0x8000;

  return;
}

// This code is called repeatedly from mipslabmain
void game_loop( void )
{
  int switches = getsw();
	int button = getbtns();

  if(IFS(0) & 0x100){
    timeoutcount++;
    IFSCLR(0) = 0x100;
  }

  // Test
  if(button & 1){
    clear_display();
  }

  if(timeoutcount == 10){


    // Debug to display what is stored in display array
    display_image(0, display);

    timeoutcount = 0;

  }
}
/*void difficulty(){			//Uses the switches 1-3 to set speed of the game
int swt;
if(swt = getsw()){
	if(getsw() == 0x3)
		diff =+ 50;

	if (getsw() = 0x2)
		diff =+ 100;

	if	(getsw() = 0x1)
		diff =+ 200;
	}


}
*/
