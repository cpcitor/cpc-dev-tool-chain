#ifndef __CDTC_ASSERT_H__
#define __CDTC_ASSERT_H__

/** This file defines a platform-independent assert(), while letting
    you some leeway as how the runtime will handle messages.  It only
    uses "puts".

    SDCC environment happens to define puts, but you can divert it via
    a #define, e.g.

#define puts(str) my_text_output(str)
#include "cdtc_assert.h"

   You might also instruct SDCC not to include or link their
   implementation.

   By choosing a particular function handling text, you may direct
   assertion failure messages to:

    * the screen, via cfwi
    * the screen, via your own routines
    * the CPC parallel port, via a loop on fw_mc_send_printer(uchar c).  This is handy as some emulators just log this into a text file, which is a nice and easy way to get information directly from an emulated CPC.

    */

#undef assert

#ifdef NDEBUG

/* Debugging disabled -- do not evaluate assertions. */
#define assert(x) ((void)0)

#else

/* [Stringification - The C Preprocessor](https://gcc.gnu.org/onlinedocs/cpp/Stringification.html#Stringification) */
#define xstr(s) str(s)
#define str(s) #s

/* Debugging enabled -- verify assertions at run time. */

#if ASSERT_CONTINUES == 1
#define assert(x) do { if ((x) == 0) { puts("Assertion failed: " NL #x NL "File: " __FILE__ NL "Line: " xstr(__LINE__) NL); } } while (0)
#else
#define assert(x) do { if ((x) == 0) { puts("Assertion failed: " NL #x NL "File: " __FILE__ NL "Line: " xstr(__LINE__) NL); while (1); } } while (0)
#endif

#endif

#endif /* __CDTC_ASSERT_H__ */
