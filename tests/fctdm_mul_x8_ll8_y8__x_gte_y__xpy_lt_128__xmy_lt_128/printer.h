#ifndef __MULBENCH_TEST_PRINT_H__
#define __MULBENCH_TEST_PRINT_H__ foo

#include "stdint.h"

void printer_uint16( uint16_t n );
void printer_uint8_as_hex_with_prefix( uint8_t v ) __z88dk_fastcall;
void printer_uint16_as_hex_with_prefix( uint16_t v ) __z88dk_fastcall;
void printer_uint32_as_hex_with_prefix( uint32_t v ) __z88dk_fastcall;

#endif /* __MULBENCH_TEST_PRINT_H__ */

