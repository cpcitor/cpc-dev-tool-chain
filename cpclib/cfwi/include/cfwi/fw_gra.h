#ifndef  __FW_GRA_H__
#define __FW_GRA_H__

void fw_gra_line_absolute(int x, int y);
void fw_gra_plot_absolute(int x, int y);
void fw_gra_move_absolute(int x, int y);
void fw_gra_test_absolute(int x, int y);

void fw_gra_line_relative(int x, int y);
void fw_gra_plot_relative(int x, int y);
void fw_gra_move_relative(int x, int y);
void fw_gra_test_relative(int x, int y);

void fw_gra_set_pen(unsigned char pencolor);

unsigned char fw_gra_get_pen(void);

void fw_gra_set_paper(unsigned char papercolor);

unsigned char fw_gra_get_paper(void);

void fw_gra_wr_char(char character);

void fw_gra_initialise(void);
void fw_gra_reset(void);
void fw_gra_clear_window(void);
void fw_gra_default(void);

void fw_gra_win_width(int xleft, int xright);
void fw_gra_win_height(int ytop, int ybottom);

#endif /* __FW_GRA_H__ */
