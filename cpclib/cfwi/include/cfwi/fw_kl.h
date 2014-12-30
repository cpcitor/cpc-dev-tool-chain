#ifndef  __FW_KL_H__
#define __FW_KL_H__

#include <stdbool.h>

long int fw_kl_time_please (void); /* No need to implement anything for this function: calling at the firmware address already conforms to what SDCC expects: a long in stored in DEHL. */

#endif /* __FW_KL_H__ */
