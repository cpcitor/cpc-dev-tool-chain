#include <stdio.h>

int
fillscreen (unsigned char a)
{
	unsigned char *p = 0xc000;
	unsigned short l;

	for (l = 0; l < 16384; l++) {
		*p = a;
		p++;
	}
}

int
main (int argc, char **argv)
{
	printf ("Hello world\n");

	if (2+2==4)
	{
		printf ("Ok.\n");
	} else
	{
		printf ("Err.\n");
	}

	{
		char c;

		for (c = 0; c <= 255; c++) {
			fillscreen (c);
		}
	}
	/* for (a=32; a<256; a++) { putc(a, stdout); } */
	return 0;
}
