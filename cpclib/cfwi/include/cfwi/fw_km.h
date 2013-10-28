#ifndef  __FW_KM_H__
#define __FW_KM_H__

#include <stdbool.h>

unsigned char fw_km_wait_key (void);
bool fw_km_read_char (void);

#endif /* __FW_KM_H__ */
