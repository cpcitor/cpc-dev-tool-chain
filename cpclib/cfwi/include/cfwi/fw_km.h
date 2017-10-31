#ifndef  __FW_KM_H__
#define __FW_KM_H__

#include <stdbool.h>

void fw_km_initialise(void);
void fw_km_reset(void);

unsigned char fw_km_wait_char (void);
int fw_km_read_char (void);

unsigned char fw_km_wait_key (void);

/** Contrary to the firmware which sets carry to tell if return value is valid, C wrapper return 0 if no valid value. */
int fw_km_read_key (void);

void fw_km_disarm_break(void);
void fw_km_break_event(void);
void fw_km_flush(void);

#endif /* __FW_KM_H__ */
