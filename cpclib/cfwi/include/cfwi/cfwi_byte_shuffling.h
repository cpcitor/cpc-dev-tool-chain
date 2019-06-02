#ifndef __CFWI_BYTE_SHUFFLING_H__
#define __CFWI_BYTE_SHUFFLING_H__

/**
Optimization

* Pass one 16bit or 32bit value instead of several via stack?  Indeed it avoids copy.  But stuffing the values is cumbersome and voids any benefit.  In other words, do it *only* if you already have a struct that makes sense.

* When a function returns a 16bit value, stuff it into a union *only* if you already have a struct that makes sense.

* When a function returns a 32bit value, *do* stuff it into a union, because accessing higher bytes of a 32bit int causes inefficient code.

 */

/** UINT_SELECT_BYTE_0 is fast. */
#define UINT_SELECT_BYTE_0(n) ((uint8_t)(n))
/** UINT_SELECT_BYTE_1 is decently fast. */
#define UINT_SELECT_BYTE_1(n) ((uint8_t)( (( (uint16_t) (n) ) >> 8) & 0xff) )

/** UINT32_SELECT_UINT16 is decently fast. */
#define UINT32_SELECT_UINT16(n) ((uint16_t) (n) )

/** UINT_SELECT_BYTE_2 is slow. SDCC 3.6.0 generates a loop! */
//#define UINT_SELECT_BYTE_2(n) ((uint8_t)( ((n) >> 16) & 0xff ))
/** UINT_SELECT_BYTE_3 is slow. SDCC 3.6.0 generates a loop! */
//#define UINT_SELECT_BYTE_3(n) ((uint8_t)( ((n) >> 24) & 0xff ))

#define UINT_AND_BYTE_0(n) ( (n) & 0x000000ff )
#define UINT_AND_BYTE_1(n) ( (n) & 0x0000ff00 )
#define UINT_AND_BYTE_2(n) ( (n) & 0x00ff0000 )
#define UINT_AND_BYTE_3(n) ( (n) & 0xff000000 )

/** Some functions expect or return a byte with only two possible values. */
enum fw_byte_all_or_nothing
{
	fw_byte_all = 0xff,
	fw_byte_nothing = 0x00
};


#endif /* __CFWI_BYTE_SHUFFLING_H__ */
