#include <stdio.h>

void fillscreen (unsigned char a);

int
main ()
{
	//printf ("Hello world!\n");
	{
		unsigned char c=255;

		while(--c>0)
		{
			fillscreen (c);
		}
	}
	/* for (a=32; a<256; a++) { putc(a, stdout); } */
	return 0;
}

void
fillscreen (unsigned char a)
{
	unsigned char *p = (unsigned char *)0xc000;
	unsigned short l;

	for (l = 0; l < 16384; l++)
	{
		*p = a;
		p++;
	}
}
