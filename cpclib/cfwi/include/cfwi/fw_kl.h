#ifndef  __FW_KL_H__
#define __FW_KL_H__

#include <stdbool.h>
#include <stdint.h>

/** Can be used to decode output of fw_kl_choke_off(). */
typedef union fw_kl_choke_output_t
{
	struct
	{
		uint8_t rom_select_address_for_a_ram_foreground_program;
		uint8_t rom_select_address_of_current_foreground_rom;
		void *address_foreground_rom_was_entered;
	};
	uint32_t as_uint32_t;
} fw_kl_choke_output_t;

/** #### CFWI-specific information: ####

    This can be useful to easily disable the firmware color event,
    which saves some cycles.  This in turn allows to use
    fw_mc_set_inks__16 to set a full palette easily.

    Use like this:

    fw_kl_choke_output_t kco;
    kco.as_uint32_t = fw_kl_choke_off();



    152: KL CHOKE OFF
    #BCC8
    Reset the Kernel - clears all event queues etc.
    Action:
    This entry completely clears all event queues, the various timer and frame flyback
    lists and so on. The effect is to dispose of any pending synchronous events and to halt
    all timer related functions other than sound generation and keyboard scanning.
    Entry conditions:
    No conditions.
    Exit conditions:
    B contains the ROM select address of the current foreground ROM (if any).
    DE contains the address at which the current foreground ROM was entered.
    C contains the ROM select address for a RAM foreground program.
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    If the current foreground program is in RAM then the ROM select address and entry
    point returned are both zero. i.e. The default ROM (ROM 0) at its entry address.
    KL CHOKE OFF forms part of the close down required before a new RAM
    foreground program is loaded, as is required by MC BOOT PROGRAM.
    The close down must ensure that there are no interrupt or other events active and
    using memory which might be damaged by loading a new program into memory. In
    the complete close down MC BOOT PROGRAM does:
    SOUND RESET
    to kill off sound generation
    an OUT to I/O port #F8FF to reset any external interrupt sources.
    KL CHOKE OFF to kill off events etc.
    KM RESET to reset any keyboard indirections and the break event.
    TXT RESET to reset any Text VDU indirections.
    SCR RESET to reset any screen indirections.
*/
uint32_t fw_kl_choke_off__with_return_value(void) __preserves_regs(iyh, iyl);
void fw_kl_choke_off__ignore_return_value(void) __preserves_regs(iyh, iyl);

uint32_t fw_kl_time_please(void);

void fw_kl_scan_needed(void);
void fw_kl_sync_reset(void);
void fw_kl_event_disable(void);
void fw_kl_event_enable(void);

#endif /* __FW_KL_H__ */
