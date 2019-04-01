#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)
#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)
#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)
#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)
#define UP  1
#define DOWN  0
#define p2_offset 115
#define p1_offset 10

void *stdout = (void *) 0;
char textbuffer[4][16];

// Variables for the game-logic
int padelY1, padelY2;
uint8_t posY = 3;
uint8_t posZ = 8;
int posX = 62;
uint8_t x_dir = 255;
uint8_t y_dir = 255;

static const uint8_t const font[] = {
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 94, 0, 0, 0, 0,
0, 0, 4, 3, 4, 3, 0, 0,
0, 36, 126, 36, 36, 126, 36, 0,
0, 36, 74, 255, 82, 36, 0, 0,
0, 70, 38, 16, 8, 100, 98, 0,
0, 52, 74, 74, 52, 32, 80, 0,
0, 0, 0, 4, 3, 0, 0, 0,
0, 0, 0, 126, 129, 0, 0, 0,
0, 0, 0, 129, 126, 0, 0, 0,
0, 42, 28, 62, 28, 42, 0, 0,
0, 8, 8, 62, 8, 8, 0, 0,
0, 0, 0, 128, 128, 0, 0, 0,
0, 8, 8, 8, 8, 8, 0, 0,
0, 0, 0, 0, 128, 0, 0, 0,
0, 64, 32, 16, 8, 4, 2, 0,
0, 62, 65, 73, 65, 62, 0, 0,
0, 0, 66, 127, 64, 0, 0, 0,
0, 0, 98, 81, 73, 70, 0, 0,
0, 0, 34, 73, 73, 54, 0, 0,
0, 0, 14, 8, 127, 8, 0, 0,
0, 0, 35, 69, 69, 57, 0, 0,
0, 0, 62, 73, 73, 50, 0, 0,
0, 0, 1, 97, 25, 7, 0, 0,
0, 0, 54, 73, 73, 54, 0, 0,
0, 0, 6, 9, 9, 126, 0, 0,
0, 0, 0, 102, 0, 0, 0, 0,
0, 0, 128, 102, 0, 0, 0, 0,
0, 0, 8, 20, 34, 65, 0, 0,
0, 0, 20, 20, 20, 20, 0, 0,
0, 0, 65, 34, 20, 8, 0, 0,
0, 2, 1, 81, 9, 6, 0, 0,
0, 28, 34, 89, 89, 82, 12, 0,
0, 0, 126, 9, 9, 126, 0, 0,
0, 0, 127, 73, 73, 54, 0, 0,
0, 0, 62, 65, 65, 34, 0, 0,
0, 0, 127, 65, 65, 62, 0, 0,
0, 0, 127, 73, 73, 65, 0, 0,
0, 0, 127, 9, 9, 1, 0, 0,
0, 0, 62, 65, 81, 50, 0, 0,
0, 0, 127, 8, 8, 127, 0, 0,
0, 0, 65, 127, 65, 0, 0, 0,
0, 0, 32, 64, 64, 63, 0, 0,
0, 0, 127, 8, 20, 99, 0, 0,
0, 0, 127, 64, 64, 64, 0, 0,
0, 127, 2, 4, 2, 127, 0, 0,
0, 127, 6, 8, 48, 127, 0, 0,
0, 0, 62, 65, 65, 62, 0, 0,
0, 0, 127, 9, 9, 6, 0, 0,
0, 0, 62, 65, 97, 126, 64, 0,
0, 0, 127, 9, 9, 118, 0, 0,
0, 0, 38, 73, 73, 50, 0, 0,
0, 1, 1, 127, 1, 1, 0, 0,
0, 0, 63, 64, 64, 63, 0, 0,
0, 31, 32, 64, 32, 31, 0, 0,
0, 63, 64, 48, 64, 63, 0, 0,
0, 0, 119, 8, 8, 119, 0, 0,
0, 3, 4, 120, 4, 3, 0, 0,
0, 0, 113, 73, 73, 71, 0, 0,
0, 0, 127, 65, 65, 0, 0, 0,
0, 2, 4, 8, 16, 32, 64, 0,
0, 0, 0, 65, 65, 127, 0, 0,
0, 4, 2, 1, 2, 4, 0, 0,
0, 64, 64, 64, 64, 64, 64, 0,
0, 0, 1, 2, 4, 0, 0, 0,
0, 0, 48, 72, 40, 120, 0, 0,
0, 0, 127, 72, 72, 48, 0, 0,
0, 0, 48, 72, 72, 0, 0, 0,
0, 0, 48, 72, 72, 127, 0, 0,
0, 0, 48, 88, 88, 16, 0, 0,
0, 0, 126, 9, 1, 2, 0, 0,
0, 0, 80, 152, 152, 112, 0, 0,
0, 0, 127, 8, 8, 112, 0, 0,
0, 0, 0, 122, 0, 0, 0, 0,
0, 0, 64, 128, 128, 122, 0, 0,
0, 0, 127, 16, 40, 72, 0, 0,
0, 0, 0, 127, 0, 0, 0, 0,
0, 120, 8, 16, 8, 112, 0, 0,
0, 0, 120, 8, 8, 112, 0, 0,
0, 0, 48, 72, 72, 48, 0, 0,
0, 0, 248, 40, 40, 16, 0, 0,
0, 0, 16, 40, 40, 248, 0, 0,
0, 0, 112, 8, 8, 16, 0, 0,
0, 0, 72, 84, 84, 36, 0, 0,
0, 0, 8, 60, 72, 32, 0, 0,
0, 0, 56, 64, 32, 120, 0, 0,
0, 0, 56, 64, 56, 0, 0, 0,
0, 56, 64, 32, 64, 56, 0, 0,
0, 0, 72, 48, 48, 72, 0, 0,
0, 0, 24, 160, 160, 120, 0, 0,
0, 0, 100, 84, 84, 76, 0, 0,
0, 0, 8, 28, 34, 65, 0, 0,
0, 0, 0, 126, 0, 0, 0, 0,
0, 0, 65, 34, 28, 8, 0, 0,
0, 0, 4, 2, 4, 2, 0, 0,
0, 120, 68, 66, 68, 120, 0, 0,
};

uint8_t Screen [] =
{
0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,

0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

126,18,18,12,0,0,4,126,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,126,18,18,12,0,0,68,98,82,76
};

uint8_t spi_send_recv(uint8_t data) {
  while(!(SPI2STAT & 0x08));
  SPI2BUF = data;
  while(!(SPI2STAT & 0x01));
  return SPI2BUF;
}

void spi_init(){
  SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
  SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
  while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
  OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
  while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
  SYSKEY = 0x0;  /* Lock OSCCON */

  /* Set up output pins */
  AD1PCFG = 0xFFFF;
  ODCE = 0x0;
  TRISECLR = 0xFF;
  PORTE = 0x0;

  /* Output pins for display signals */
  PORTF = 0xFFFF;
  PORTG = (1 << 9);
  ODCF = 0x0;
  ODCG = 0x0;
  TRISFCLR = 0x70;
  TRISGCLR = 0x200;

  /* Set up SPI as master */
  SPI2CON = 0;
  SPI2BRG = 4;
  /* SPI2STAT bit SPIROV = 0; */
  SPI2STATCLR = 0x40;
  /* SPI2CON bit CKP = 1; */
  SPI2CONSET = 0x40;
  /* SPI2CON bit MSTEN = 1; */
  SPI2CONSET = 0x20;
  /* SPI2CON bit ON = 1; */
  SPI2CONSET = 0x8000;
}

// Create a rand-function with the clock-registers
int rand(void) {
  return 70;
}

// Read inputs for buttons
int getbtns(void){
  int buttons = PORTD >> 4;
  buttons = (buttons & 0x0E);
  buttons |= ((PORTF>>1)&0x1);
  return buttons;
}

// Function from lab, a delay-function
void  quicksleep(int cyc) {
  int i;
  for(i = cyc; i > 0; i--);
}

// Init the display, code from labsession
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

// To display a text on 4 diffrent columns, from lab.
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

// Used to move the ball
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

// Manipulated display_image, to update the screen. GO through!!
void display_Screen(int x, const uint8_t *data){
  int i, j;
  for(i = 0; i < 4; i++) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22); // Setup page start and end
    spi_send_recv(i); // Set all four pages
    spi_send_recv(x & 0xF); // ????
    spi_send_recv(0x10 | ((x >> 4) & 0xF)); // 0001
    DISPLAY_CHANGE_TO_DATA_MODE;
    for(j = 0; j < 128; j++)
      spi_send_recv(data[i*128 + j]);
  }
}

void bounce_indicator() {
  if(posX < 2|| posX > 127) {
    x_dir = (!x_dir);
  }
  if((posY == 0 && posZ == 1) | (posY == 3 && posZ == 128)) {
    y_dir = (!y_dir);
  }
}

void move_ballX() {
  if(x_dir) {
    Screen[posY*128 + posX] = 0;
    posX++;
    Screen[posY*128 + posX] = posZ;

  } else {
    Screen[posY*128 + posX] = 0;
    posX--;
    Screen[posY*128 + posX] = posZ;
  }
  bounce_indicator();
}

void move_ballY() {
  if(y_dir) {
    posZ = posZ >> 1;
    if (posZ == 0) {
      Screen[posY*128 + posX] = 0;
      posY--;
      posZ = 128;
      Screen[posY*128 + posX] = posZ;
      bounce_indicator();
      return;
    } else {
      Screen[posY*128 + posX] = posZ;
    }
  } else {
        posZ = posZ << 1;
        if (posZ == 0) {
          Screen[posY*128 + posX] = 0;
          posY++;
          posZ = 1;
          Screen[posY*128 + posX] = posZ;
          bounce_indicator();
          return;
        } else {
          Screen[posY*128 + posX] = posZ;
        }
  }
  bounce_indicator();
}

int move_padel(int direction, int padel_offset, int padel_page) {
  if(direction) { // UP
    if(padel_page != 3) {
      int i;
      for(i=0; i<3; i++) {
        Screen[padel_page*128 + padel_offset + i] = 0;
      }
      for(i=0; i<3; i++) {
        Screen[(padel_page+1)*128 + padel_offset + i] = 255;
      }
      padel_page++;
    }
  } else {  // DOWN
    if(padel_page != 0) {
      int i;
      for(i=0; i<3; i++){
        Screen[padel_page*128 + padel_offset + i] = 0;
      }
      for(i=0; i<3; i++) {
        Screen[(padel_page-1)*128 + padel_offset + i] = 255;
      }
      padel_page--;
    }
  }
  return padel_page;
}

// Read input
void update() {
  display_Screen(0,Screen);

  if (getbtns() == 0x08) {
    padelY1 = move_padel(UP, p1_offset, padelY1);
  }
  if (getbtns() == 0x04) {
    padelY1 = move_padel(DOWN, p1_offset, padelY1);
  }
  if (getbtns() == 0x02) {
    padelY2 = move_padel(DOWN, p2_offset, padelY2);
  }
  if(getbtns() == 0x01) {
    padelY2 = move_padel(UP, p2_offset, padelY2);
  }
  move_ballY();
  move_ballX();
  // To fast for padels but good speed for ball!
  quicksleep(500000);
}

// Set variables
void variables_init() {
  padelY1 = 0;
  padelY2 = 0;
}

// Setup io
void io_init(){
  TRISDSET = 0xE0;
  TRISFSET = 0x1;
}

int main(void) {
  spi_init();
  display_init();
  variables_init();
  io_init();
  Screen[posY*128 + posX] = posZ;


  while(1) {
    update();
  }

  return 0;
}
