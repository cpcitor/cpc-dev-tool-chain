#ifndef  __FW_TXT_H__
#define __FW_TXT_H__

#include <stdint.h>

void fw_txt_initialise(void);
void fw_txt_reset(void);
void fw_txt_vdu_enable(void);
void fw_txt_vdu_disable(void);
void fw_txt_cur_enable(void);
void fw_txt_cur_disable(void);
void fw_txt_cur_on(void);
void fw_txt_cur_off(void);
void fw_txt_place_cursor(void);
void fw_txt_remove_cursor(void);
void fw_txt_draw_cursor(void);
void fw_txt_undraw_cursor(void);

void fw_txt_set_column(int8_t column);
void fw_txt_set_row(int8_t row);
void fw_txt_set_cursor(int8_t row, int8_t column);
void fw_txt_output(unsigned char c);
void fw_txt_wr_char(unsigned char c);

void fw_txt_set_pen(uint8_t p);
void fw_txt_set_paper(uint8_t p);

void fw_txt_win_enable(unsigned char left, unsigned char right, unsigned char top, unsigned char bottom);
void fw_txt_clear_window();

#endif /* __FW_TXT_H__ */
