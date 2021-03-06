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
int bike1_crash;
int bike2_crash;
int bike1_score;
int bike2_score;

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
  *port_E = 0xff; // Set PORTE to 0 so we can see the ticks better
  TRISFSET = 0x2;

  /* Timer from lab 3 */
  TRISD = TRISD | 0x0fe0;
  T2CON = 0x0;
  T2CONSET = 0x70;
	TMR2 = 0x0;
	PR2 = ((80000000 / 256) / 50);
	T2CONSET = 0x8000;

  /* Do initial clearing and drawing a new board */
  clear_display();
  border_init();
  players_init();

  bike1_crash = 0;			//To not crash player1's bike at the start
  bike2_crash = 0;			//To not crash player2's bike at the start
  bike1_score = ~(1);
  bike2_score = 8;

  return;
}

/* This code is called repeatedly from mipslabmain */
void game_loop( void )
{
  int switches = getsw();
	int button = getbtns();
	int button1 = getbtn1();


  if(IFS(0) & 0x100){
    timeoutcount++;
    IFSCLR(0) = 0x100;
  }

  if(timeoutcount == 10){
	
	//Flip different switches to get faster or slower game speed
	if(switches = getsw()){
		if(getsw() == 0x8)  //Switch 4?
      PR2 = ((80000000 / 256) / 120);
		
		if(getsw() == 0x4)  //Switch 3
      PR2 = ((80000000 / 256) / 100);

    if(getsw() == 0x2)		//Switch 2
      PR2 =((80000000 / 256) / 50);

    if(getsw() == 0x1)		//Switch 1
      PR2 = ((80000000 / 256) / 10);
  }

  	if(button1 & 0x200){ //Button 1
    	switch(bike2_direction){
        case 1:
          bike2_direction = 4;
          break;

        case 2:
          bike2_direction = 1;
          break;

        case 3:
          bike2_direction = 2;
          break;

        case 4:
          bike2_direction = 3;
          break;
  	     }
    }

    if(button & 1){		//Button 2
      switch(bike2_direction){
        case 1:
          bike2_direction = 2;
          break;

        case 2:
          bike2_direction = 3;
          break;

        case 3:
          bike2_direction = 4;
          break;

        case 4:
          bike2_direction = 1;
          break;
      }
    }

    if(button & 2){			//Button 3
	    switch(bike1_direction){
        case 1:
          bike1_direction = 4;
          break;

        case 2:
          bike1_direction = 1;
          break;

        case 3:
          bike1_direction = 2;
          break;

        case 4:
          bike1_direction = 3;
          break;
      }
    }

    // Button 4
    if(button & 4){
      switch(bike1_direction){

        case 1:
          bike1_direction = 2;
          break;

        case 2:
          bike1_direction = 3;
          break;

        case 3:
          bike1_direction = 4;
          break;

        case 4:
          bike1_direction = 1;
          break;

      }
    }

    player1_update(bike1_direction);
    player2_update(bike2_direction);

    if(bike1_x == 127 || bike1_x == 0 || bike1_y == 0 || bike1_y == 31){
      bike1_crash = 1;
    }

    if(bike2_x == 127 || bike2_x == 0 || bike2_y == 0 || bike2_y == 31){
      bike2_crash = 1;
    }

    // Update the screen with new information
    display_image(0, display);
    timeoutcount = 0;


  	//Check if player 1 (button 1-2) has crashed
  	if(bike1_crash == 1){
	 bike1_score++; //Losing player takes a hit
	 get_score();	//Decrease a led-light
  	 clear_display();
  	 display_string(1, " PLAYER 2 WON!  ");
     display_update();
     quicksleep(15000000);
	  
     labinit();
      return;
  		//write something here to get back to main
  	}

    //Check if player 1 (button 1-2) has crashed
    if(bike2_crash == 1){
	  bike2_score--; //Losing player takes a hit
	  get_score();	//Decrease a led-light
      clear_display();
      display_string(1, " PLAYER 1 WON!  ");
      display_update();
      quicksleep(15000000);
      labinit();
	  
      //write something here to get back to main
    }

  }
}
