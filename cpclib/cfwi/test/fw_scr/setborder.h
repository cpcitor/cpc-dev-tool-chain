#ifndef  __SETBORDER_H__
#define __SETBORDER_H__

void setborder(uint8_t color) __z88dk_fastcall __preserves_regs(a, d, e, h, l, iyh, iyl);

#define setborder_or40(color) setborder( (color) | 0x40 )

#endif /* __SETBORDER_H__ */
