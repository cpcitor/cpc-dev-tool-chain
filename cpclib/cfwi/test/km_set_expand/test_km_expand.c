#include <cfwi/cfwi.h>

#define NL "\015\012"

/* 0x80 redefines "f0" function key */
#define TESTTOKEN 0x80

// Warning: test code assumes TESTSTRING_LENGTH <= 9
#define TESTSTRING "print 42"
#define NUMBER_OF_ARRAY_ELEMENTS(s) (sizeof(s)/sizeof(s[0]))

/* -1 to remove NULL terminator */
#define TESTSTRING_LENGTH (NUMBER_OF_ARRAY_ELEMENTS(TESTSTRING)-1)

// preprocessor black magic
// https://stackoverflow.com/questions/2653214/stringification-of-a-macro-value
#define xstr(a) str(a)
#define str(a) #a

void
main ()
{
	cfwi_txt_str0_output ("Will test" NL "fw_km_set_expand("
			      xstr (TESTTOKEN) ", 0x");
	fw_txt_wr_char (TESTSTRING_LENGTH + '0');
	cfwi_txt_str0_output (", \"" TESTSTRING "\");" NL);

	fw_txt_wr_char (fw_km_set_expand
			(TESTTOKEN, TESTSTRING_LENGTH, TESTSTRING));

	cfwi_txt_str0_output (NL NL "Will test fw_km_get_expand()" NL);
	{
		uint8_t index;

		for (index = 0; index < (TESTSTRING_LENGTH + 3); index++)
		{
			uint16_t value = fw_km_get_expand (TESTTOKEN, index);

			fw_txt_wr_char ('C');
			fw_txt_wr_char (index + '0');
			fw_txt_wr_char (' ');
			fw_txt_wr_char (value >> 8);
			fw_txt_wr_char (value & 0xff);
			fw_txt_output (13);
			fw_txt_output (10);
		}

	}
	cfwi_txt_str0_output ("Press any key to exit C." NL);
	fw_km_wait_char ();
}
