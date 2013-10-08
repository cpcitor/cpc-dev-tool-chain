#include <stdio.h>
#include "hello.h"
#include "cpcfirmware.h"
#include "fillscreen.h"

// __at (0xbd19) void mc_wait_flyback();

int
main ()
{
	// Simplest : show some chars.
	{
		unsigned char a;

		for (a = 32; a != 0; a++)
		{
			putchar (a);
		}
	}

	printf ("\r\n\r\nHello world by cpcitor.\r\n");
	printf ("Parts in C, parts in assembly.\r\n");
	printf ("Compiled with SDCC.\r\n");

	polite_wait ("Let's use VDU control codes for credits.");

	printf ("%c%cChanged to mode 1.\r\n", 4, 1);

	printf ("Credits.\r\n");
	printf ("%c%c%cKevin Thacker%c for arnoldemu, sharing knowledge and many more.\r\n", 15, 1, 24, 24);
	printf ("%c%c%cMochilote%c for tutorial on cpcmania.com.\r\n", 15, 2,
		24, 24);
	printf ("%c%c%cNorecess%c for tutorial on norecess.net.\r\n", 15, 3,
		24, 24);
	printf ("%c%cAll cpcwiki community.\r\n", 15, 0);

	polite_wait ("Let's use VDU control codes for credits.");

	printf ("%c%cChanged to mode 0.\r\n", 4, 0);

	printf ("%c%c%cChanged border color.\r\n", 29, 15, 15);

	polite_wait ("Next step is show some colors.");
	{
		unsigned char a;

		for (a = 0; a < 16; a++)
		{
			putchar (14);
			putchar (a);
			putchar (18);
			putchar (13);
			putchar (10);
		}
	}
	printf ("%c%c%c%c", 15, 1, 14, 0);
	polite_wait ("Color end.");

	polite_wait ("Next step go back to mode 1.");

	printf ("%c%cChanged to mode 1.\r\n", 4, 1);

	polite_wait ("Next step is printing control chars test.");
	{
		unsigned char a;

		for (a = 0; a < 32; a++)
		{
			putchar (1);
			putchar (a);
		}
	}
	polite_wait ("Printing control char test end.");

	polite_wait ("Next step is printf with decimal argument test.");
	printf ("Answer to life, the universe and everything: %d.\r\n", 42);

	polite_wait ("Next step is using km_wait_key for keyboard input.");
	{
		unsigned char n = 10;

		printf ("Type something. The next %d keypresses will be echoed.\r\n", n);

		while (n-- > 0)
		{
			unsigned char c = km_wait_key ();

			printf ("Key yields char number %d which is '%c'\r\n",
				c, c);
		}
	}
	polite_wait ("I'll fill the screen by calling from C a function written in pure assembly.\r\n");
	{
		unsigned char c = 0xff;

		fillscreen (c);
		polite_wait ("I'll now fill the screen repeatedly.\r\n");
		while (c != 0)
		{
			c = c >> 1;
			fillscreen (c);
		}
	}
	printf ("Goodbye world!\r\n");
	printf ("(press any key)\r\n");
	km_wait_key ();
	return 0;
}

void
polite_wait (char *msg)
{
	printf ("\r\n");
	printf (msg);
	printf ("\r\n====== Press any key to move on ======\r\n");
	km_wait_key ();
}
