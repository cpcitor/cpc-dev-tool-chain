#ifndef  __FW_KM_H__
#define __FW_KM_H__

#include <stdbool.h>

unsigned char fw_km_wait_char (void);
int fw_km_read_char (void);
unsigned char fw_km_wait_key (void);
int fw_km_read_key (void);

void fw_km_initialise(void);
void fw_km_reset(void);
void fw_km_disarm_break(void);
void fw_km_break_event(void);
void fw_km_flush(void);

#endif /* __FW_KM_H__ */
