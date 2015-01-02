#ifndef  __FW_KL_H__
#define __FW_KL_H__

#include <stdbool.h>

long int fw_kl_time_please (void); /* No need to implement anything for this function: calling at the firmware address already conforms to what SDCC expects: a long in stored in DEHL. */

void fw_kl_scan_needed(void);
void fw_kl_sync_reset(void);
void fw_kl_event_disable(void);
void fw_kl_event_enable(void);

#endif /* __FW_KL_H__ */
void fw_kl_time_please(void);
