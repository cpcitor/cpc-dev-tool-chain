#ifndef  __FW_JRE_H__
#define __FW_JRE_H__

/** 189: JUMP RESTORE
    #BD37
    Restore the standard jumpblock.
    Action:
    Set the main firmware jumpblock to its standard state as described in sections 14.1
    and 15.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    This routine may be used to restore the jumpblock to its standard routines after the
    user has changed entries in it. The whole of the jumpblock is set up so care must be
    taken if other programs, such as AMSDOS, have patched it.
    The indirections jumpblock is set up piecemeal by the various packs' initialization and
    reset routines. JUMP RESTORE does not set up the indirections.
    Related entries:
    GRA RESET
    KM RESET
    MC RESET PRINTER
    SCR RESET
    TXT RESET
*/
void fw_jre_jump_restore(void);

#endif /* __FW_JRE_H__ */
