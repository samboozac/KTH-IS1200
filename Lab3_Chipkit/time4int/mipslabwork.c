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
int prime = 1234567;    /* start prime-number*/

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{

if(IFS(0) & 0x100){     // Interrupt flag
  timeoutcount++;       // For the 1 second- delay
  IFSCLR(0) = 0x100;    // Resets the flag
  if (timeoutcount == 10) {
    time2string ( textstring, mytime);    /* call function time2string with parameters textstring and mytime*/
    display_string( 3, textstring );
    display_update();
    tick( &mytime );                      /*  Call tick function to add a second to the timer through the address of 'mytime' */
    PORTE++;
    timeoutcount = 0;                     /*  Reset the timeout-counter*/
  }
}

// Check the Switch interrupt bit.
if(IFS(0) & 0x80) {
  // Reset the interrupt bit
  IFSCLR(0) = 0x80;
  //Intcrement PORTE (LED-register)
  PORTE++;
}
                        /* Clearing the flag */
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile int * t = (volatile int *) 0xbf886100;   /* Defining trise pointer 'volatile' you dont want compiler to optimize it*/
  *t = *t & 0xFF00;
  TRISDSET = 0x0FE0;    /* Initialize porte for */
  PR2 = TMR2PERIOD;     /* Set the clock period */
  T2CONSET = 0x70;      /* Setting the prescale */
  TMR2 = 0;             /* Setting the timer to 0 */
  T2CONSET = 0x8000;    /* Set the on-bit */

  // Enabling interupts from Timer2
  IPC(2) = 0x1c;          // Interrupt-priority level
  IEC(0) = 0x100;         // Interrupt Enable Control Register (IEC), Set enable interrupt

  // Inittilize external for SW1
  IECSET(0) = 0x80;     // Enable flag to external interrupt(SW1(INT1, 7th bit)) edge sense, hight-to-low)
  IPC(1) = 0x7;         // Interrupt-priority level
  PORTD = 0x0;          // Start value on D-register
  TRISD = 0x100;        // Setup SW1 input;

  enable_interrupt();     /* Calling interrupt from labwork.S */
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  prime = nextprime ( prime );
  display_string( 0, itoaconv( prime ));
  display_update();
}
