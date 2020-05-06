#ifndef __LOG_H__
#define __LOG_H__

/* PLATFORM NATIVE */

#include <platform.h> // defines NL platform_print_formatted_string and platform_print_plain_string

#define dbglog_unconditional(msg) platform_print_plain_string(msg)
#define dbglogf_unconditional(...) platform_print_formatted_string(__VA_ARGS__)

/* #define dbglog_unconditional(msg) */
/* #define dbglogf_unconditional(...) */

#define dbglogf(LOGLEVEL, FORMAT, ...) CDTC_ON_DEBUGLEVEL_GREATER_THAN_ ## LOGLEVEL ( dbglogf_unconditional( FORMAT, ##__VA_ARGS__) )
#define dbglog(LOGLEVEL, MSG) CDTC_ON_DEBUGLEVEL_GREATER_THAN_ ## LOGLEVEL ( dbglog_unconditional( MSG ) )

#define dbgvar(LOGLEVEL, VARNAME, FORMAT) dbglogf(LOGLEVEL, #VARNAME "=" FORMAT " ", VARNAME)
#define dbgvar_d(LOGLEVEL, VARNAME) dbgvar(LOGLEVEL, VARNAME, "%d")
#define dbgvar_c(LOGLEVEL, VARNAME) dbgvar(LOGLEVEL, VARNAME, "%c")
#define dbgvar_u(LOGLEVEL, VARNAME) dbgvar(LOGLEVEL, VARNAME, "%u")
#define dbgvar_lu(LOGLEVEL, VARNAME) dbgvar(LOGLEVEL, VARNAME, "%lu")
#define dbgvar_s(LOGLEVEL, VARNAME) dbgvar(LOGLEVEL, VARNAME, "%s")
#define dbglog_nl(LOGLEVEL) dbglog(LOGLEVEL, NL)

/* Use CDTC_DEBUGLEVEL to get something that parses as a number,
   and CDTC_DEBUGLEVEL_STRINGIFIED to get something that parses as a string. */

#ifdef __GNUC__

#define silmark(LOGLEVEL) dbglogf( LOGLEVEL, NL "== in %s ==\t", __PRETTY_FUNCTION__)
#define silmark_end(LOGLEVEL) dbglogf( LOGLEVEL, NL "== quitting %s ==\t", __PRETTY_FUNCTION__)

#define silmsg(LOGLEVEL, FORMAT, ...) \
        dbglogf( LOGLEVEL, NL "%s: ", __PRETTY_FUNCTION__); \
        dbglogf( LOGLEVEL, FORMAT, ##__VA_ARGS__);      \
        dbglog( LOGLEVEL, NL)

#define silvar(LOGLEVEL, VARNAME, FORMAT) silmsg(#VARNAME "= " #FORMAT NL, VARNAME)

#else /* __GNUC__ */

#define silmark(LOGLEVEL)
#define silmark_end(LOGLEVEL)
#define silmsg(LOGLEVEL, FORMAT, ...) dbglogf(LOGLEVEL, FORMAT, ##__VA_ARGS__ )
#define silvar(LOGLEVEL, VARNAME, FORMAT) silmsg(#VARNAME "= " #FORMAT NL, VARNAME)

#endif /* __GNUC__ */



/* ==================== Internal! "Don't use this." ==================== */

// Cf. https://gcc.gnu.org/onlinedocs/gcc-4.9.0/cpp/Stringification.html
#define __MACRO_STRINGIFICATION_xstr(s) __MACRO_STRINGIFICATION_str(s)
#define __MACRO_STRINGIFICATION_str(s) #s
#define CDTC_DEBUGLEVEL_STRINGIFIED __MACRO_STRINGIFICATION_xstr(CDTC_DEBUGLEVEL)


#ifndef NDEBUG

#if CDTC_DEBUGLEVEL >= 9
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_9(...) __VA_ARGS__
#else
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_9(...)
#endif

#if CDTC_DEBUGLEVEL >= 8
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_8(...) __VA_ARGS__
#else
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_8(...)
#endif

#if CDTC_DEBUGLEVEL >= 7
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_7(...) __VA_ARGS__
#else
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_7(...)
#endif

#if CDTC_DEBUGLEVEL >= 6
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_6(...) __VA_ARGS__
#else
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_6(...)
#endif

#if CDTC_DEBUGLEVEL >= 5
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_5(...) __VA_ARGS__
#else
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_5(...)
#endif

#if CDTC_DEBUGLEVEL >= 4
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_4(...) __VA_ARGS__
#else
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_4(...)
#endif

#if CDTC_DEBUGLEVEL >= 3
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_3(...) __VA_ARGS__
#else
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_3(...)
#endif

#if CDTC_DEBUGLEVEL >= 2
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_2(...) __VA_ARGS__
#else
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_2(...)
#endif

#if CDTC_DEBUGLEVEL >= 1
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_1(...) __VA_ARGS__
#else
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_1(...)
#endif

#if CDTC_DEBUGLEVEL >= 0
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_0(...) __VA_ARGS__
#else
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_0(...)
#endif

#else /* NDEBUG */

#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_9(...)
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_8(...)
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_7(...)
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_6(...)
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_5(...)
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_4(...)
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_3(...)
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_2(...)
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_1(...)
#define CDTC_ON_DEBUGLEVEL_GREATER_THAN_0(...)

#endif /* CDTC_DEBUGLEVEL */

#define CDTC_ON_DEBUGLEVEL_GREATER_THAN(LOGLEVEL, ...) CDTC_ON_DEBUGLEVEL_GREATER_THAN_ ## LOGLEVEL (__VA_ARGS__)

/* ==================== end of "Don't use this." ==================== */

#endif /* __LOG_H__ */
