#ifndef _cpcfirmware_H_
#define _cpcfirmware_H_

/** Small wrappers (implemented in assembly) to firmware routines.

    The thin assembly wrapping is to pass arguments and return value
    between compiler convention and actual routine practices. */

extern unsigned char km_wait_key (void);
#endif /* _cpcfirmware_H_ */
