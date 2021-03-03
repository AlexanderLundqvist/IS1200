/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

/* Declare a helper function which is local to this file */
static void num32asc( char * s, int );

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

int bike1x;
int bike1y;
int bike2x;
int bike2y;

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

/* tick:
   Add 1 to time in memory, at location pointed to by parameter.
   Time is stored as 4 pairs of 2 NBCD-digits.
   1st pair (most significant byte) counts days.
   2nd pair counts hours.
   3rd pair counts minutes.
   4th pair (least significant byte) counts seconds.
   In most labs, only the 3rd and 4th pairs are used. */
void tick( unsigned int * timep )
{
  /* Get current value, store locally */
  register unsigned int t = * timep;
  t += 1; /* Increment local copy */

  /* If result was not a valid BCD-coded time, adjust now */

  if( (t & 0x0000000f) >= 0x0000000a ) t += 0x00000006;
  if( (t & 0x000000f0) >= 0x00000060 ) t += 0x000000a0;
  /* Seconds are now OK */

  if( (t & 0x00000f00) >= 0x00000a00 ) t += 0x00000600;
  if( (t & 0x0000f000) >= 0x00006000 ) t += 0x0000a000;
  /* Minutes are now OK */

  if( (t & 0x000f0000) >= 0x000a0000 ) t += 0x00060000;
  if( (t & 0x00ff0000) >= 0x00240000 ) t += 0x00dc0000;
  /* Hours are now OK */

  if( (t & 0x0f000000) >= 0x0a000000 ) t += 0x06000000;
  if( (t & 0xf0000000) >= 0xa0000000 ) t = 0;
  /* Days are now OK */

  * timep = t; /* Store new value */
}

/* display_debug
   A function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr )
{
  display_string( 1, "Addr" );
  display_string( 2, "Data" );
  num32asc( &textbuffer[1][6], (int) addr );
  num32asc( &textbuffer[2][6], *addr );
  display_update();
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void display_init(void) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);

	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}

void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;

	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

void display_image(int x, const uint8_t *data) {
	int i, j;

	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));

		DISPLAY_CHANGE_TO_DATA_MODE;

		// Adding multiplier of 4 so we calculate with the whole width (4 "pages")
		for(j = 0; j < 32*4; j++)
			spi_send_recv(~data[i*32*4 + j]);
	}
}

void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;

			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc( char * s, int n )
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}

/*
 * nextprime
 *
 * Return the first prime number larger than the integer
 * given as a parameter. The integer must be positive.
 */
#define PRIME_FALSE   0     /* Constant to help readability. */
#define PRIME_TRUE    1     /* Constant to help readability. */
int nextprime( int inval )
{
   register int perhapsprime = 0; /* Holds a tentative prime while we check it. */
   register int testfactor; /* Holds various factors for which we test perhapsprime. */
   register int found;      /* Flag, false until we find a prime. */

   if (inval < 3 )          /* Initial sanity check of parameter. */
   {
     if(inval <= 0) return(1);  /* Return 1 for zero or negative input. */
     if(inval == 1) return(2);  /* Easy special case. */
     if(inval == 2) return(3);  /* Easy special case. */
   }
   else
   {
     /* Testing an even number for primeness is pointless, since
      * all even numbers are divisible by 2. Therefore, we make sure
      * that perhapsprime is larger than the parameter, and odd. */
     perhapsprime = ( inval + 1 ) | 1 ;
   }
   /* While prime not found, loop. */
   for( found = PRIME_FALSE; found != PRIME_TRUE; perhapsprime += 2 )
   {
     /* Check factors from 3 up to perhapsprime/2. */
     for( testfactor = 3; testfactor <= (perhapsprime >> 1) + 1; testfactor += 1 )
     {
       found = PRIME_TRUE;      /* Assume we will find a prime. */
       if( (perhapsprime % testfactor) == 0 ) /* If testfactor divides perhapsprime... */
       {
         found = PRIME_FALSE;   /* ...then, perhapsprime was non-prime. */
         goto check_next_prime; /* Break the inner loop, go test a new perhapsprime. */
       }
     }
     check_next_prime:;         /* This label is used to break the inner loop. */
     if( found == PRIME_TRUE )  /* If the loop ended normally, we found a prime. */
     {
       return( perhapsprime );  /* Return the prime we found. */
     }
   }
   return( perhapsprime );      /* When the loop ends, perhapsprime is a real prime. */
}

/*
 * itoa
 *
 * Simple conversion routine
 * Converts binary to decimal numbers
 * Returns pointer to (static) char array
 *
 * The integer argument is converted to a string
 * of digits representing the integer in decimal format.
 * The integer is considered signed, and a minus-sign
 * precedes the string of digits if the number is
 * negative.
 *
 * This routine will return a varying number of digits, from
 * one digit (for integers in the range 0 through 9) and up to
 * 10 digits and a leading minus-sign (for the largest negative
 * 32-bit integers).
 *
 * If the integer has the special value
 * 100000...0 (that's 31 zeros), the number cannot be
 * negated. We check for this, and treat this as a special case.
 * If the integer has any other value, the sign is saved separately.
 *
 * If the integer is negative, it is then converted to
 * its positive counterpart. We then use the positive
 * absolute value for conversion.
 *
 * Conversion produces the least-significant digits first,
 * which is the reverse of the order in which we wish to
 * print the digits. We therefore store all digits in a buffer,
 * in ASCII form.
 *
 * To avoid a separate step for reversing the contents of the buffer,
 * the buffer is initialized with an end-of-string marker at the
 * very end of the buffer. The digits produced by conversion are then
 * stored right-to-left in the buffer: starting with the position
 * immediately before the end-of-string marker and proceeding towards
 * the beginning of the buffer.
 *
 * For this to work, the buffer size must of course be big enough
 * to hold the decimal representation of the largest possible integer,
 * and the minus sign, and the trailing end-of-string marker.
 * The value 24 for ITOA_BUFSIZ was selected to allow conversion of
 * 64-bit quantities; however, the size of an int on your current compiler
 * may not allow this straight away.
 */
#define ITOA_BUFSIZ ( 24 )
char * itoaconv( int num )
{
  register int i, sign;
  static char itoa_buffer[ ITOA_BUFSIZ ];
  static const char maxneg[] = "-2147483648";

  itoa_buffer[ ITOA_BUFSIZ - 1 ] = 0;   /* Insert the end-of-string marker. */
  sign = num;                           /* Save sign. */
  if( num < 0 && num - 1 > 0 )          /* Check for most negative integer */
  {
    for( i = 0; i < sizeof( maxneg ); i += 1 )
    itoa_buffer[ i + 1 ] = maxneg[ i ];
    i = 0;
  }
  else
  {
    if( num < 0 ) num = -num;           /* Make number positive. */
    i = ITOA_BUFSIZ - 2;                /* Location for first ASCII digit. */
    do {
      itoa_buffer[ i ] = num % 10 + '0';/* Insert next digit. */
      num = num / 10;                   /* Remove digit from number. */
      i -= 1;                           /* Move index to next empty position. */
    } while( num > 0 );
    if( sign < 0 )
    {
      itoa_buffer[ i ] = '-';
      i -= 1;
    }
  }
  /* Since the loop always sets the index i to the next empty position,
   * we must add 1 in order to return a pointer to the first occupied position. */
  return( &itoa_buffer[ i + 1 ] );
}


/* Project code goes here */

/*
// TBD
void putPixel (int x, int y){
	if(x<129 && y<64){

	if(y<8){ // Page 0

		//om y = 0 så skickas ~1
			if(y==0){
				int write = ~1;
				display[x] = display[x] & write;
			}
			// Om y != 0 så skickas 2^y in och inverteras till skärmen
			else
			{
				int write = 1;
				int i;
				for (i = 0; i < y; i++)
				{
					write *= 2;
				}
				display[x] = display[x] & (~write);
			}
}

	 if(y>= 8 && y<16){ // Page 1
			y = y - 8;

			if(y==0){
				int write = ~1;
				display[128+x] = display[128+x] & write;
			}
			// Om y != 0 så skickas 2^y in och inverteras till skärmen
			else
			{
				int write = 1;
				int i;
				for (i = 0; i < y; i++)
				{
					write *= 2;
				}
				display[128+x] = display[128+x] & (~write);
			}
		}

		if(y>= 16 && y<24){ // Page 2
 			y = y - 16;

			if(y==0){
				int write = ~1;
				display[256+x] = display[256+x] & write;
			}
			// Om y != 0 så skickas 2^y in och inverteras till skärmen
			else
			{
				int write = 1;
				int i;
				for (i = 0; i < y; i++)
				{
					write *= 2;
				}
				display[256+x] = display[256+x] & (~write);
			}
 		}

		if(y>= 24 && y<32){ // Page 2
 			y = y - 24;

			if(y==0){
				int write = ~1;
				display[384+x] = display[384+x] & write;
			}
			// Om y != 0 så skickas 2^y in och inverteras till skärmen
			else
			{
				int write = 1;
				int i;
				for (i = 0; i < y; i++)
				{
					write *= 2;
				}
				display[384+x] = display[384+x] & (~write);
			}
		}
}


}



// TBD

//Psuedo-kods försök till att visualisera hanteringen av att styra med knapp 1 och 2
void button1(){

int newx;
int newy;
int oldx;
int oldy;

	if(newx>oldx & newy == oldy){

	while("inget nytt knapptryck"){		//sväng uppåt (addera i y-led)
		putPixel(bike1x, bike1y)
		display_image(0, display)
		bike1y++
		oldy = bike1y
		}
	}

	if(newy>oldy & newx == oldx){
	while ("inget nytt knapptryck")	//sväng vänster (subtrahera i x-led)
	putPixel(bike1x, bike1y)
		display_image(0, display)
		bike1x--
		oldx = bike1x
	}

	if(newx<oldx & newy == oldy){
	while ("inget nytt knapptryck")	//sväng nedåt (subtrahera i y-led)
	putPixel(bike1x, bike1y)
		display_image(0, display)
		bike1y--
		oldy = bike1y
	}
	if(newy<oldy & oldx == newx){
	while ("inget nytt knapptryck")	//sväng "höger" (addera i x-led)
	putPixel(bike1x, bike1y)
		display_image(0, display)
		bike1x++
		oldx = bike1x
	}

}


void button2(){
	int newx;
	int newy;
	int oldx;
	int oldy;

	if(newx>oldx & newy == oldy){
	while ("inget nytt knapptryck")	//sväng nedåt (subtrahera i y-led)
	putPixel(bike1x, bike1y)
		display_image(0, display)
		bike1y--
		oldy = bike1y
	}

	if(newy>oldy & newx == oldx){
	while ("inget nytt knapptryck")	//sväng "höger" (addera i x-led)
	putPixel(bike1x, bike1y)
		display_image(0, display)
		bike1x++
		oldx = bike1x
	}

	if(newx<oldx & newy == oldy){
	while("inget nytt knapptryck"){		//sväng uppåt (addera i y-led)
		putPixel(bike1x, bike1y)
		display_image(0, display)
		bike1y++
		oldy = bike1y
	}
	if(newy<newx & oldx == oldy){
	while ("inget nytt knapptryck")	//sväng vänster (subtrahera i x-led)
	putPixel(bike1x, bike1y)
		display_image(0, display)
		bike1x--
		oldx = bike1x
	}

}
*/

/*
	Snake initialization. Puts a new snake in the standard starting position.
*/
void snake_init(void){

	display[166] = 127;
	display[167] = 127;
	display[295] = 254;
	display[294] = 254;

	return;
}

/*
	Initialization of the border around the gamefield.
*/
void border_init(void){
	int row, column;
	int r_offset = 0;
	int c_offset = 0;

	// Draws short sides
	for(row=0; row <= 4; row++){
		display[r_offset] = 0;
		display[127 + r_offset] = 0;
		r_offset += 128;
	}

	// Draws long sides

	for(column=1; column < 127; column++){
		display[column] = 254;
		display[384+column] = 127;
	}

	return;
}

// Helper function that clears the whole display
void clear_display(void){
	int i;
	for (i = 0; i < 512; i++)
		display[i] = 255; // Sets all the bits to 1s to turn of pixels.
	return;
}
