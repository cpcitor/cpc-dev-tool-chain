#ifndef  __CFWI_H__
#define __CFWI_H__

#ifdef DISTRUST_PRESERVE_REGS
// This can be useful if you encounter a bad behavior and suspect
// registers aren's preserved by the firmware conforming to
// documentation.  Such suspicion happened to me, and this allows to
// quickly assert that it was not the problem.
#define __preserves_regs(...) 
#endif /* DISTRUST_PRESERVE_REGS */

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
