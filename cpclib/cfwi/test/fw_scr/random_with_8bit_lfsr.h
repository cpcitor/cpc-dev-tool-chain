#ifndef  __RANDOM_WITH_8BIT_LFSR_H__
#define __RANDOM_WITH_8BIT_LFSR_H__

uint8_t random_with_8bit_lfsr( uint8_t *state ) __z88dk_fastcall __preserves_regs( b, c, d, e, iyh, iyl );
uint8_t random_with_16bit_lfsr( uint16_t *state ) __z88dk_fastcall __preserves_regs( b, d, e, iyh, iyl );

#endif /* __RANDOM_WITH_8BIT_LFSR_H__ */
