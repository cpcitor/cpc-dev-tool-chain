#ifndef __fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_h__
#define __fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_h__

#include <stdint.h>

void fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_fill_table()
    __preserves_regs( c, iyh, iyl );

// We do not define it here because it conflicts if you define it yourself in C at a constant address, like:
// uint8_t __at(0x1200) fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table[256];
// this is why the line below is commented out:
// extern uint8_t fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table[256];


uint16_t fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128( uint16_t xy )
    __z88dk_fastcall __preserves_regs( iyh, iyl );

#endif /* __fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_h__ */
