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

typedef union fw_memory_range_t
{
	struct
	{
		int8_t *lowest_usable_byte;
		int8_t *highest_usable_byte;
	};
	uint32_t as_uint32_t;
} fw_memory_range_t;

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    Since C cannot handle return more than one value and SDCC cannot
    return a struct, we use a trick.  Technically, an integer is
    returned and you can extract values from a union.

    In practice, just use like this:

    // Initial memory range values are supplied by OS as per Soft968 section 10.3
    fw_memory_range_t mem_range = { FIXME };
    mem_range.as_uint32_t = fw_kl_rom_walk(mem_range);
    printf("x=%d, y=%d", mem_range.x, mem_range.y);

    153: KL ROM WALK
    #BCCB
    Find and initialize all background ROMs.
    Action:
    Background ROMs provide support for expansion hardware or augment the software
    facilities of the machine. If the facilities provided by the background ROMs are to be
    available, the foreground program must initialize them. This routine finds and
    initializes all background ROMs.
    Entry conditions:
    DE contains address of the first usable byte of memory (lowest address).
    HL contains address of the last usable bytes of memory (highest address).
    Exit conditions:
    DE contains the address of the new first usable byte of memory.
    HL contains the address of the new last usable byte of memory.
    AF and BC corrupt.
    All other registers preserved.
    Notes:
    When a foreground ROM program is entered it is passed the addresses of the first and
    last bytes in memory which it may use. The area of memory outside this is used to
    store firmware variables, the stack, the jumpblocks and the screen memory. From the
    area available for the foreground program to use, the areas for background programs
    to use must be allocated.
    The foreground program should initialize background ROMs at an early stage, before
    it uses the memory it is given. It may choose whether to enable background ROMs or
    not. KL INIT BACK may be used to initialize a particular background ROM or this
    routine may be used to initialize all available background ROMs.
    KL ROM WALK inspects the ROMs at ROM select addresses in the range 1..7 in
    V1.0 firmware and 0..15 in V1.1 firmware. The power-up initialization entry of each
    background ROM found is called (unless it is the current foreground ROM in V1.1
    firmware). This entry may allocate some memory for the background ROM's use by
    adjusting the values in DE and HL before returning. Once the ROM has been
    initialized the Kernel adds it to the list of external command servers, and notes the
    base of the area which the ROM has allocated at the top of memory (if any).
    Subsequent FAR CALLs to entries in the ROM will automatically set the IY index
    register to point at the ROMs upper memory area.
    See section 10.4 for a full description of background ROMs.
    Related entries:
    KL FIND COMMAND
    KL INIT BACK
    KL LOG EXT
*/
uint32_t fw_kl_rom_walk__fastcall(uint32_t fw_memory_range_t_asint) __preserves_regs(iyh, iyl);

// void fw_kl_scan_needed(void);

/** 167: KL SYNC RESET
    #BCF5
    Clear synchronous event queue.
    Action:
    The synchronous event queue is set empty - any outstanding events are simply
    discarded. The current event priority, used by KL POLL SYNCHRONOUS and KL
    NEXT SYNC to mask out lower priority events, is reset.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    It is the user's responsibility to ensure that discarded events and any currently active
    events are reset. The event count of discarded events will be greater than zero, so any
    further 'kicks' will simply increment the count, but not add the event to the
    synchronous event queue - the events are, therefore, effectively disarmed.
    Related entries:
    KL DEL SYNCHRONOUS
    KL NEXT SYNC
    KL POLL SYNCHRONOUS
    */
void fw_kl_sync_reset(void) __preserves_regs(b, c, d, e, iyh, iyl);

/** 172: KL EVENT DISABLE
    #BD04
    Disable normal synchronous events.
    Action:
    Prevent normal synchronous events from being processed but allow express
    synchronous events to be processed. This is achieved by setting the current event
    priority higher than any possible normal synchronous event priority.
    Entry conditions:
    No conditions.
    Exit conditions:
    HL corrupt.
    All other registers preserved.
    Notes:
    KL EVENT DISABLE does not prevent events from being kicked. The effect is to
    'mask off' all pending normal synchronous events so that they are hidden from the
    foreground program (when KL POLL SYNCHRONOUS or KL NEXT SYNC are
    called) and hence are not processed.
    KL EVENT ENABLE reverses the effect of KL EVENT DISABLE.
    It is not possible to disable synchronous events permanently from inside a
    synchronous event routine as the previous current event priority is restored when the
    event routine returns.
    Related entries:
    KL DISARM EVENT
    KL EVENT ENABLE
    KL NEXT SYNC
    KL POLL SYNCHRONOUS
*/
void fw_kl_event_disable(void) __preserves_regs(b, c, d, e, iyh, iyl);

/** 173: KL EVENT ENABLE
    #BD07
    Enables normal synchronous events.
    Action:
    Allow normal and express synchronous events to be processed.
    Entry conditions:
    No conditions.
    Exit conditions:
    HL corrupt.
    All other registers preserved.
    Notes:
    Events are enabled by default. KL EVENT ENABLE reverses the effect of KL
    EVENT DISABLE.
    It is not possible to disable synchronous events permanently from inside a
    synchronous event routine as the current event priority which is used to disable events
    is restored when the event routine returns.
    Related entries:
    KL EVENT DISABLE
    KL NEXT SYNC
    KL POLL SYNCHRONOUS
*/
void fw_kl_event_enable(void) __preserves_regs(b, c, d, e, iyh, iyl);

/** 175: KL TIME PLEASE
    #BD0D
    Ask the elapsed time.
    Action:
    The Kernel maintains a count which it increments on each time interrupt. The count,
    therefore, measures time in 1/300th of a second units. This routine returns the current
    count.
    Entry conditions:
    No conditions.
    Exit conditions:
    DEHL contains the four byte count (D contains the most significant byte and L the
    least significant byte).
    All other registers preserved.
    Notes:
    The count is zeroized when the machine is turned on or reset. The count may be set to
    another starting value by KL TIME SET.
    The count is not kept up to date if interrupts are disabled for long periods, such as
    while reading and writing the cassette.
    The four byte count overflows after approximately:
    14,316,558 Seconds
    = 238,609 Minutes
    = 3,977 Hours
    = 166 Days
    This routine enables interrupts.
    Related entries:
    KL TIME SET
*/
uint32_t fw_kl_time_please(void) __preserves_regs(a, b, c, iyh, iyl);

#endif /* __FW_KL_H__ */
