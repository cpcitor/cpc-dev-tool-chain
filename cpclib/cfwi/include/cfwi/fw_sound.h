#ifndef  __FW_SOUND_H__
#define __FW_SOUND_H__

/** 141: SOUND RESET
    #BCA7
    Reset the Sound Manager.
    Action:
    Re-initialize the Sound Manager - shut the sound chip up and clear all queues.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The sound queues are cleared. Any current sound is stopped. The sound generator
    chip is silenced.
    This routine enables interrupts.
    Related entries:
    SOUND HOLD
     */
void fw_sound_reset(void);
void fw_sound_continue(void);

/** 147: SOUND CONTINUE
    #BCB9
    Restart sounds after they have all been held.
    Action:
    Allows sounds that have been held by calling SOUND HOLD to continue.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and IX corrupt.
    All other registers preserved.
    Notes:
    If no sounds are held then no action is taken.
    This routine may enable interrupts.
    Related entries:
    SOUND HOLD
    SOUND RELEASE
*/

#endif /* __FW_SOUND_H__ */
