#ifndef __uint16_t_as_uint8_t_pair_t_h__
#define __uint16_t_as_uint8_t_pair_t_h__

typedef union uint16_t_as_uint8_t_pair_t
{
        uint16_t as_uint16_t;
        struct
        {
                uint8_t low;
                uint8_t high;
        } as_uint8_pair;
} uint16_t_as_uint8_t_pair_t;

#endif /*  __uint16_t_as_uint8_t_pair_t_h__ */
