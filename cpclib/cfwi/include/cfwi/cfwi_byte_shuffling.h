#ifndef __CFWI_BYTE_SHUFFLING_H__
#define __CFWI_BYTE_SHUFFLING_H__

#define UINT_SELECT_BYTE_0(n) ((uint8_t)( (n) & 0xff ))
#define UINT_SELECT_BYTE_1(n) ((uint8_t)( ((n) >> 8) & 0xff ))
#define UINT_SELECT_BYTE_2(n) ((uint8_t)( ((n) >> 16) & 0xff ))
#define UINT_SELECT_BYTE_3(n) ((uint8_t)( ((n) >> 24) & 0xff ))

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
