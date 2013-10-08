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

	polite_wait ("Let's use VDU control codes for credits.");

	// \0xx is standard C: it's octal code to insert arbitrary characters
	// in a string.

	printf ("\r\n\n\017\002Hello \017\003world \017\001by cpcitor\.\r\n\n");
	printf ("\030Parts in C\030, parts in assembly.\r\n");
	printf ("Compiled with SDCC.\r\n");

	printf ("Credits.\r\n\n\016\003");
	printf ("\017\001\030Kevin Thacker\030 for arnoldemu, sharing knowledge and many more.\022\r\n");
	printf ("\017\002\030Mochilote\030 for tutorial on cpcmania.com.\022\r\n");

	// \000 would insert a byte zero, but that's the string terminator in C.
	// So, we use printf %c instead.
	printf ("\017%c\030Norecess\030 for tutorial on norecess.net.\022\r\n", 0);
	printf ("\017\001All cpcwiki community.\022\r\n");

	printf ("\r\n\n\016%c\017\001", 0);

	polite_wait ("Let's use VDU control chars to display many colors.");

	printf ("\004%cChanged to mode 0.\r\n", 0);

	printf ("\035\017\017Changed border color to orange.\r\n");

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
	printf ("\017\001\016%c", 0);
	polite_wait ("Color show end. Next step go back to mode 1.");

	printf ("\004\001Changed to mode 1.\r\n");
	printf ("\035\001\001Changed border color back to blue.\r\n");

	printf ("Oh, we have forgotten to show control chars.\r\n\n");
	{
		unsigned char a;

		for (a = 0; a < 32; a++)
		{
			putchar (1);
			putchar (a);
		}
	}
	printf ("\r\n\nShowing control char end.\r\n\n");

	polite_wait ("Let's switch to mode 2.");

	printf ("\004\002Changed to mode 2.\r\n");

	polite_wait ("Next step is C's printf() with formatting a decimal argument.");
	printf ("Answer to life, the universe and everything: %d.\r\n", 42);

	polite_wait ("Next step is use C's printf() to show char and number from keyboard input.");
	{
		unsigned char n = 10;

		printf ("Type something. The next %d keypresses will be echoed.\r\n", n);

		while (n-- > 0)
		{
			unsigned char c = km_wait_key ();

			printf ("Key yields char number %d which is '\001%c'\r\n",
				c, c);
		}
	}
	polite_wait("I'll go back to mode 1 then fill the screen\r\n"
		    "by calling from C a fillscreen() function written in pure assembly.\r\n");
	printf ("\004\001Changed to mode 1.\r\n");
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
	printf ("Press any key. Depending on the way the program was called, it will return or reset the CPC.\r\n");
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
