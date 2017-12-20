.module fw_gra_plot_relative

_fw_gra_plot_relative::
        ld      hl,#2
        add     hl,sp
        ld      e,(hl)

        inc     hl
        ld      d,(hl)

        inc     hl
        ld      a,(hl)

        inc     hl
        ld      h,(hl)

        ld      l,a
        jp      0xBBED  ; GRA PLOT RELATIVE
