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

int mytime = 0x5957;


char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile int * t = (volatile int *) 0xbf886100;
  *t = *t & 0xFF00;
  TRISDSET = 0x0FE0;
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  delay( 1000 );
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

  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  *p = *p + 1;
  display_image(96, icon);
}
