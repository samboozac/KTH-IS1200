/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#define TMR2PERIOD ((80000000/ 256)/10)
int mytime = 0x5957;
int timeoutcount = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  // Volatile to avoid compiler optimization
  volatile int * t = (volatile int *) 0xbf886100;
  // masking ls8b
  *t = *t & 0xFF00;
  // Setting bits 11:5 to input (buttons and switches)
  TRISDSET = 0x0FE0;      /* Set */
  // Setting the clock period
  PR2 = TMR2PERIOD;
  // Setting prescale
  T2CONSET = 0x70;
  //Resetting the timer to  0
  TMR2 = 0;

  T2CONSET = 0x8000;                  /* Resetting the times */
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  int switches = getsw();
  int buttons = getbtns();

  if(buttons == 4) {
    mytime = mytime & 0x0FFF;
    mytime = mytime | (switches << 12);
  }
  if(buttons == 2) {
    mytime = mytime & 0xF0FF;
    mytime = mytime | (switches << 8);
  }
  if (buttons == 1){
    mytime = mytime & 0xFF0F;
    mytime = mytime | (switches << 4);
  }
  volatile int * p = (volatile int*) 0xbf886110;

  if (IFS(0) & 0x100) {
    timeoutcount++;
    IFSCLR(0) = 0x100;
  }

  if (timeoutcount == 10) {
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
    *p = *p +1;
    display_image(96, icon);

    timeoutcount = 0;
  }

  return;
}
