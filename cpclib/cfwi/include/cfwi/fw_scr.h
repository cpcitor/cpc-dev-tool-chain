#ifndef  __FW_SCR_H__
#define __FW_SCR_H__

#include <stdint.h>

void fw_scr_set_mode(unsigned char x);

void fw_scr_initialise(void);
void fw_scr_reset(void);
void fw_scr_clear(void);
void fw_scr_mode_clear(void);
void fw_scr_set_ink( uint8_t pen, uint8_t color1, uint8_t color2 );
void fw_scr_set_border( uint8_t color1, uint8_t color2 );

#endif /* __FW_SCR_H__ */
