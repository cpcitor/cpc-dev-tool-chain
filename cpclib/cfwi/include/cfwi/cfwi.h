#ifndef  __CFWI_H__
#define __CFWI_H__

#ifdef DISTRUST_PRESERVE_REGS
// This can be useful if you encounter a bad behavior and suspect
// registers aren's preserved by the firmware conforming to
// documentation.  Such suspicion happened to me, and this allows to
// quickly assert that it was not the problem.
#define __preserves_regs(...) 
#endif /* DISTRUST_PRESERVE_REGS */

/**

   #### CFWI-specific information: ####

   TL;DR: if a firmware routine has **argument pointer** to RAM,
   ensure it is *not* below 0x4000.

   The information is important for all routines that take **a
   pointer argument in RAM**.  

   Soft968 section 2

   Routines that always access RAM will mention this in the description of the
   routine. Other routines may be assumed to be affected by the ROM state. In
   particular the various data blocks used by the Kernel must lie in the central 32K of
   RAM for the Kernel to be able to use them.
*/

#include "cfwi_txt.h"
#include "fw_cas.h"
#include "fw_gra.h"
#include "fw_jre.h"
#include "fw_kl.h"
#include "fw_km.h"
#include "fw_mc.h"
#include "fw_scr.h"
#include "fw_sound.h"
#include "fw_txt.h"

#endif /* __CFWI_H__ */
