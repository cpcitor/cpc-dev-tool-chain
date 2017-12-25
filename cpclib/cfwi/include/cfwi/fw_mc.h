#ifndef  __FW_MC_H__
#define __FW_MC_H__

/** 177: MC BOOT PROGRAM
    #BD13
    Load and run a program.
    Action:
    Shut down as much of the system as possible then load a program into RAM and run
    it. If the load fails then the previous foreground program is restarted.
    Entry conditions:
    HL contains the address of the routine to call to load the program.
    Exit conditions:
    Does not exit!
    Notes:
    The system is partially reset before attempting to load the program. External
    interrupts are disabled, as are all timer, frame flyback and keyboard break events.
    Sound generation is turned off, indirections are set to their default routines and the
    stack is reset to the default system stack. This process ensures that no memory outside
    the firmware variables area is in use when loading the program. Overwriting an active
    event block or indirection routine could otherwise have unfortunate consequences.
    The partial system reset does not change the ROM state or ROM selection. The
    routine run to load the program must be in accessible RAM or an enabled ROM. Note
    that the firmware jumpblock normally enables the lower ROM and disables the upper
    ROM and so the routine must normally be in RAM above #4000 or in the lower ROM.
    The routine run to load the program is free to use any store from #0040 up to the base
    of the firmware variables area (#B100) and may alter indirections and arm external
    device interrupts as required. It should obey the following
    Exit conditions:
    If the program loaded successfully:
    Carry true.
    HL contains the program entry point.
    If the program failed to load:
    Carry false.
    HL corrupt.
    Always:
    A, BC, DE, IX, IY and other flags corrupt.

    After a successful load the firmware is completely initialized (as at EMS) and the
    program is entered at the entry address returned by the load routine. Returning from
    the program will reset the system (perform RST 0).
    After an unsuccessful load an appropriate error message is printed and the previous
    foreground program is restarted. If the previous foreground program was itself a RAM
    program then the default ROM is entered instead as the program may have been
    corrupted during the failed loading.
    Related entries:
    CAS IN DIRECT
    KL CHOKE OFF
    MC START PROGRAM
*/
void fw_mc_boot_program(void *loader) __z88dk_fastcall;

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    178: MC START PROGRAM
    #BD16
    Run a foreground program.
    Action:
    Fully initialize the system and enter a program.
    Entry conditions:
    HL contains the entry point address.
    C contains the required ROM selection.
    Exit conditions:
    Never exits!
    Notes:
    HL and C comprise the 'far address' of the entry point of the foreground program (see
    section 2).
    When entering a foreground program in ROM the ROM selection should be that
    required to select the appropriate ROM. When entering a foreground program in RAM
    the ROM selection should be used to enable or disable ROMs as the RAM program
    requires (ROM select addresses #FC..#FF).
    This routine carries out a full EMS initialization of the firmware before entering the
    program. Returning from the program will reset the system (perform RST 0).
    MC START PROGRAM is intended for running programs in ROM or programs that
    have already been loaded into RAM. To load and run a RAM program use MC BOOT
    PROGRAM.
    Related entries:
    MC BOOT PROGRAM
    RESET ENTRY (RST 0)
*/
void fw_mc_start_program(uint8_t rom_selection, void *entry);

void fw_mc_wait_flyback(void);
void fw_mc_reset_printer(void);

#endif /* __FW_MC_H__ */
