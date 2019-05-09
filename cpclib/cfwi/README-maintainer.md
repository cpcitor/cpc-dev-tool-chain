# C-firmware interface cheat sheet

## How to add support for an entry

* Locate an unsupported or incomplete call, by looking at `coverage.html` or https://rawgit.com/cpcitor/cpc-dev-tool-chain/master/cpclib/cfwi/coverage.html
* Get information on http://www.cpcwiki.eu/index.php/Soft968:_CPC_464/664/6128_Firmware
* In particular, check if the documentation mentions preserved registers and which ones.

The next step depends on the situation.

### Case of entry requiring no parameter, having no output: no wrapper, just a declaration and a symbol.

Example: `SCR RESET` is at `BC02` and preserves `IY`.

In `./src/fw_nowrapperneeded.s` add a line

    _fw_scr_reset == 0xBC02

In `./include/cfwi/fw_scr.h` add a line

    void fw_scr_reset(void) __preserves_regs(iyh, iyl);

Prepend that line with a paragraph copy-pasted from official documentation.

### Other cases needing no wrapper

If the firmware routine expects parameters or return a value in a way that happens to match a SDCC calling convention, there is still no need for a wrapper.

This is based on SDCC manual:

> 4.3.3 Calling convention
>
> By default, all parameters are passed on the stack,
> right-to-left. 8-bit return values are passed in l, 16-bit values in
> hl, 32-bit values in dehl. Except for the GBZ80, where 8-bit values
> are passed in e, 16-bit values in de, 32-bit values in hlde. Larger
> return values are passed in memory in a location specified by the
> caller through a hidden pointer argument.
>
> There are three other calling conventions supported, which can be
> specified using the keywords __smallc, __z88dk_fastcall (not on
> GBZ80) and __z88dk_callee. They are primarily intended for
> compability with libraries written for other compilers. For
> __smallc, the parameters are passed on the stack, left-to-right, but
> variable ar- guments are currently not supported. For
> __z88dk_fastcall, there may be only one parameter of at most 32
> bits, which is passed the same way as the return value. For
> __z88dk_callee, parameters are passed on the stack, but the stack is
> not adjusted for the parameters after the call (thus the callee has
> to do this instead). __z88dk_callee can be combined with __smallc.

#### Example: `SCR SET OFFSET` expects a 16-bit argument in `HL`, returns nothing.

This is what SDCC provides if the prototype is prepended with `__z88dk_fastcall`.

So, do like previous case but with a `__z88dk_fastcall` added on the prototype:

    void fw_scr_set_offset(uint16_t offset) __z88dk_fastcall __preserves_regs(b, c, d, e, iyh, iyl);

#### Entries that return a few integer values

C doesn't allow to return several values.  But a function can return an integer (2 bytes, 4 bytes) that can be split into individual byte return values.

Example: `KM GET STATE` expects no parameter and returns 16-bit in `HL`.

This is another "no wrapper needed" case.

    uint16_t fw_km_get_state(void) __preserves_regs(b, c, d, e, iyh, iyl);

Some other cases are handled without wrapper, although one could be implemented.

For example `KM GET DELAY` returns actually two 8-bit values and is declared like this:

    uint16_t fw_km_get_delay(void);

This makes the C-level API slightly less straightforward but still pretty much usable:

    uint16_t returned_value = fw_km_get_delay();
    uint8_t startup_delay = UINT_SELECT_BYTE_1(returned_value);
    uint8_t repeat_speed = UINT_SELECT_BYTE_0(returned_value);

#### No others?

In theory, some other cases than a 16-bit argument in `HL` could fit.  For example: 8-bit argument in `L`, or 32-bit argument in `DEHL`.  In practice CPC firmware routines that require a 8-bit argument expect it in `A`, not `L`.  I do not know any that expect a 32-bit argument.

### Entry needing a "light" ASM wrapper.

Entries that expect at most one argument can be declared like this example in `include/cfwi/fw_gra.h`.

    void fw_gra_set_pen(uint8_t pencolor) __z88dk_fastcall __preserves_regs(b, c, d, e, h, l, iyh, iyl);

Then implement call with a file `./src/fw_gra_set_pen.s` with this content:

    .module fw_gra_set_pen

    _fw_gra_set_pen::
            ld      a,l
            jp      0xBBDE   ; GRA SET PEN

As you can see, it just copies the value that SDCC provides in `L` into the register `A` expected by the firwmare, and jumps (instead of `CALL` + `RET`).

### Calls needing a "heavy" ASM wrapper.

In the most general (and most expensive time + code size) case, the wrapper will :

* fetch arguments from the stack
* stuff them in the registers expected by the firmware entry
* call firmware entry
* stuff the return values to `L`,`H`,`E`,`D` so that SDCC-generated code can pick them back.

Those wrapper are mostly unique, handwritten.

Sometimes a few optimizations are possible.

Some calls could benefit from `__z88dk_callee` handling.

### Calls handled both with and without wrapper.

Some entries take several values which happen to somehow fit the SDCC fastcall convention if we consider them one big (16-bit or 32-bit) integer.  In this case we sometimes provide two variants: regular and suffixed with `__fastcall`.

Example:

	`GRA PLOT ABSOLUTE`
	`GRA PLOT RELATIVE`
	`GRA TEST ABSOLUTE`
	`GRA TEST RELATIVE`
	`GRA LINE ABSOLUTE`
	`GRA LINE RELATIVE`
	`TXT WIN ENABLE`
	`TXT SET CURSOR`

## How to update coverage file

    cd cpc-dev-tool-chain/cpclib/cfwi
    make
    bash firmware_coverage.sh
