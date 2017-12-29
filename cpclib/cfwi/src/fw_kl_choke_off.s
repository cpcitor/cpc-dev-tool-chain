.module fw_kl_choke_off

_fw_kl_choke_off__with_return_value::
        call    0xbcc8          ; KL CHOKE OFF
        ld      l,c
        ld      h,b
        ret
