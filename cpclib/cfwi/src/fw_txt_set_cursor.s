.module fw_txt_set_cursor

_fw_txt_set_cursor::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        inc     hl
        ld      h,(hl)
        ld      l,a
        jp    0xBB75   ; TXT SET CURSOR

        ;; {
        ;;         int x;

        ;;         for ( x = 1; x <= 10; x++ )
        ;;         {
        ;;                 int y;

        ;;                 for ( y = 1; y <= 25; y++ )
        ;;                 {
        ;;                         fw_txt_set_cursor( y, x << 2 );
        ;;                         pr_uint( y );
        ;;                         fw_txt_wr_char( ',' );
        ;;                         pr_uint( x );
        ;;                 }
        ;;         }
        ;; }

