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

char textstring[] = "text, more text, and even more text!";

volatile int *tris_E;
volatile int *port_E;

int count = 0;
int timeoutcount = 0;
int bike2_score;
int bike1_score;
int bike1_crash;
int bike2_crash;

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

void labinit( void )
{
  tris_E = (volatile int*) 0xbf886100; // Address of TRISE register
  port_E = (volatile int*) 0xbf886110; // Address of PORTE register

  *tris_E = *tris_E & 0xffffff00; // To mask out the 8 LSBs
  *port_E = 0xff; // Set all bits PORTE to 1 so we light up all the LED:s on the IO shield
  TRISFSET = 0x2;

  // Timer from lab 3
  TRISD = TRISD | 0x0fe0;
  T2CON = 0x0;
  T2CONSET = 0x70;
	TMR2 = 0x0;
	PR2 = ((80000000 / 256) / 50);
	T2CONSET = 0x8000;

  // Sometimes fragments are left so we update the screen.
  display_string(0, "");
	display_string(1, "");
	display_string(2, "");
	display_string(3, "");
	display_update();


  // Initialize scoring
  bike2_score = 4;
  bike1_score = 8;
}

void game_init(void){
  /* Do initial clearing and drawing a new board */
  clear_display();
  clear_taken();
  border_init();
  players_init();
  bike1_crash = 0;			//To not crash player1's bike at the start
  bike2_crash = 0;			//To not crash player2's bike at the start
}

/* This code is called repeatedly from mipslabmain */
void game_loop(void)
{
  int switches = getsw();	  //Switches 1-4 are located on PORT registers RD8-RD11
	int button = getbtns();   //Buttons 2-4 are located on PORT registers RD5-RD7
	int button1 = getbtn1();  //Button 1 is located on PORT register RF1

  //Flip different switches to get faster or slower game speed
	if(switches = getsw()){
		if(getsw() == 0x8)  //Switch 4
      PR2 = ((80000000 / 256) / 100);	//Sets a period of 3125, very fast

		if(getsw() == 0x4)  //Switch 3
      PR2 = ((80000000 / 256) / 70);	//Sets a period of 4464, pretty fast

    if(getsw() == 0x2)		//Switch 2
      PR2 =((80000000 / 256) / 50);		//Sets a period of 6250, standard speed

    if(getsw() == 0x1)		//Switch 1
      PR2 = ((80000000 / 256) / 30);	//Sets a period of 10417, pretty slow
  }

  if(IFS(0) & 0x100){
    timeoutcount++;
    IFSCLR(0) = 0x100;
  }

  if(timeoutcount == 10){

    /* Player 2 controls */
    if(button1 & 0x200){ //Button 1
    	switch(bike2_direction){
        case 1:
          bike2_direction = -2;
          break;

        case 2:
          bike2_direction = 1;
          break;

        case -1:
          bike2_direction = 2;
          break;

        case -2:
          bike2_direction = -1;
          break;
  	     }
    }

    if(button & 1){		//Button 2
      switch(bike2_direction){
        case 1:
          bike2_direction = 2;
          break;

        case 2:
          bike2_direction = -1;
          break;

        case -1:
          bike2_direction = -2;
          break;

        case -2:
          bike2_direction = 1;
          break;
      }
    }

    /* Player 1 controls */
    if(button & 2){	//Button 3
	    switch(bike1_direction){
        case 1:
          bike1_direction = -2;
          break;

        case 2:
          bike1_direction = 1;
          break;

        case -1:
          bike1_direction = 2;
          break;

        case -2:
          bike1_direction = -1;
          break;
      }
    }

    if(button & 4){ // Button 4
      switch(bike1_direction){

        case 1:
          bike1_direction = 2;
          break;

        case 2:
          bike1_direction = -1;
          break;

        case -1:
          bike1_direction = -2;
          break;

        case -2:
          bike1_direction = 1;
          break;

      }
    }


    player1_update(bike1_direction);
    player2_update(bike2_direction);

    check_crash();
    check_gameover();

    // Update the screen with new information
    display_image(0, display);
    timeoutcount = 0;


  }
}
