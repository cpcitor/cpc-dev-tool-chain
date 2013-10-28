/* A Graphics library for the AMSTRAD CPC range of computers
   Uses firmware to realize graphics output..
   2003 H. Hansen
*/

#ifndef  __conio_h__
#define __conio_h__

; void clrscr (void);
; unsigned char kbhit (void);
; void gotox (unsigned char x);
; void gotoy (unsigned char y);
; void gotoxy (unsigned char x, unsigned char y)
; unsigned char wherex (void);
; unsigned char wherey (void);
; void cputc (char c);
; void cputcxy (unsigned char x, unsigned char y, char c)
; void cputs (const char* s);
; void cputsxy (unsigned char x, unsigned char y, const char* s);
; int cprintf (const char* format, ...);
; int vcprintf (const char* format, va_list ap);
; char cgetc (void);
; unsigned char cursor (unsigned char onoff);
; unsigned char revers (unsigned char onoff);
; unsigned char textcolor (unsigned char color);
; unsigned char bgcolor (unsigned char color);
; unsigned char bordercolor (unsigned char color);
; void chline (unsigned char length);
; void chlinexy (unsigned char x, unsigned char y, unsigned char length);
; void cvline (unsigned char length);
; void cvlinexy (unsigned char x, unsigned char y, unsigned char length);
; void cclear (unsigned char length);
; void cclearxy (unsigned char x, unsigned char y, unsigned char length);
; void screensize (unsigned char* x, unsigned char* y);
; void cputhex8 (unsigned char val);
; void cputhex16 (unsigned val);

#endif /* __conio_h__ */
