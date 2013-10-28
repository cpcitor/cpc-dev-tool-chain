/* A Graphics library for the AMSTRAD CPC range of computers
   Uses firmware to realize graphics output..
   2003 H. Hansen
*/

#ifndef  __amsgraph_h__
#define __amsgraph_h__

void draw(int x, int y);
void plot(int x, int y);
void move(int x, int y);
void gpen(unsigned char pencolor);
unsigned char getgpen(void);

#endif /* __amsgraph_h__ */
