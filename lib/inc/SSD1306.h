// header for SSD1306 OLED display
// Simon Walker, char map by Chelsea Walker
// Device variant changes - Diarmid Rendell
// working as of May 19/2022

// private helpers
//void SSD1306_Command8 (unsigned char command);
//void SSD1306_Command16 (unsigned char commandA, unsigned char commandB);
//void SSD1306_Data (unsigned char * data, unsigned int iCount);

#include <avr/pgmspace.h> // defines to place items in flash (program memory)

#ifndef _SSD1306_ADDRESS
#define _SSD1306_ADDRESS 0x3C
#endif

// comment in/out the appropriate size of your display! ****
#ifndef _SSD1306_DisplaySize128x32
#define _SSD1306_DisplaySize128x32
#endif
//#ifndef _SSD1306_DisplaySize128x64
//#define _SSD1306_DisplaySize128x64
//#endif
// comment in/out the appropriate size of your display! ****

// management
void SSD1306_DispInit (void);
void SSD1306_Noise (void);
void SSD1306_Clear (void);
void SSD1306_Render (void);
int SSD1306_IsDirty (void);
void SSD1306_DisplayOn (void);
void SSD1306_DisplayOff (void);

// string
void SSD1306_CharXY (unsigned char iX, unsigned char iY, char disp);
void SSD1306_StringXY (unsigned char iX, unsigned char iY, char * pStr);

// graphics
void SSD1306_SetPixel (int iX, int iY);
void SSD1306_Line (int iXS, int iYS, int iXE, int iYE);
void SSD1306_Circle (int iXS, int iYS, float fRad);

// requires the page data to be in flash
void SSD1306_SetPage (int page, PGM_P buff);
