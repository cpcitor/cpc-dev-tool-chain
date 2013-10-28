#ifndef  __FW_GRA_H__
#define __FW_GRA_H__

void fw_gra_line_absolute(int x, int y);

void fw_gra_plot_absolute(int x, int y);

void fw_gra_move_absolute(int x, int y);

void fw_gra_set_pen(unsigned char pencolor);

unsigned char fw_gra_get_pen(void);

#endif /* __FW_GRA_H__ */
