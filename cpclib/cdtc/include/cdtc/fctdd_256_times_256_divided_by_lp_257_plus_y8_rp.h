#ifndef __fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_h__
#define __fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_h__

#include <stdint.h>

void fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_fill_table()
    __preserves_regs( iyh, iyl );

// We do not define it here because it conflicts if you define it yourself in C at a constant address, like:
// uint8_t __at(0x1200) fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_table[256];
// this is why the line below is commented out:
// extern uint8_t fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_table[256];

uint8_t
fctdd_256_times_256_divided_by_lp_257_plus_y8_rp( uint16_t xy ) __z88dk_fastcall
    __preserves_regs( c, iyh, iyl );

#endif /* __fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_h__ */
