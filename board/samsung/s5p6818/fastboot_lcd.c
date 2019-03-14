/*
 * (C) Copyright 2009
 * jung hyun kim, Nexell Co, <jhkim@nexell.co.kr>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <config.h>
#include <common.h>
#include <pwm.h>
#include <platform.h>
#include <draw_lcd.h>
#include <asm/arch/fastboot.h>

static void lcd_print(int x, int y, const char * fmt, ...)
{
	va_list ap;
	char buf[512];
	int len;

	memset(buf, 0, sizeof(buf));
	va_start(ap, fmt);
	len = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	lcd_draw_text(buf, x, y, 1, 1, 0);
}

void lcd_show_progress(int percent)
{
	static int history = -1;
	unsigned int c = 0xffffff;
	int x, y, w, h;

	if(history == percent)
		return;

	x = 40;
	y = CFG_DISP_PRI_RESOL_HEIGHT - 50;
	w = CFG_DISP_PRI_RESOL_WIDTH - 120;
	h = 8 + 10;

	lcd_line_horizontal(x, y, w + 1, c, 0);
	lcd_line_horizontal(x, y + h, w + 1, c, 0);

	lcd_line_vertical(y, x, h, c, 0);
	lcd_line_vertical(y, x + w, h, c, 0);

	lcd_fill_rectangle(x + 2, y + 3, w - 4, h - 6, 0, 0);
	lcd_fill_rectangle(x + 2, y + 3, (w - 4) * percent / 100, h - 6, c, 0);
	lcd_print(x + w + 8, y + 2, "%3d%%", percent);
}

void lcd_show_message(const char * fmt, ...)
{
	va_list ap;
	char buf[512];
	int len;
	int x, y, w, h;

	memset(buf, 0, sizeof(buf));
	va_start(ap, fmt);
	len = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	x = 40;
	y = CFG_DISP_PRI_RESOL_HEIGHT - 50 - 18;
	w = CFG_DISP_PRI_RESOL_WIDTH - 120;
	h = 16;
	lcd_fill_rectangle(x, y, w, h, 0, 0);	
	lcd_draw_text(buf, x, y, 1, 1, 0);
}

void fboot_lcd_start(void)
{
	lcd_info lcd = {
		.fb_base		= CONFIG_FB_ADDR,
		.bit_per_pixel	= CFG_DISP_PRI_SCREEN_PIXEL_BYTE * 8,
		.lcd_width		= CFG_DISP_PRI_RESOL_WIDTH,
		.lcd_height		= CFG_DISP_PRI_RESOL_HEIGHT,
		.back_color		= 0x000000,
		.text_color		= 0xFFFFFF,
		.alphablend		= 0,
	};
	lcd_debug_init(&lcd);

	/* clear FB */
	memset((void*)CONFIG_FB_ADDR, 0x00,
		CFG_DISP_PRI_RESOL_WIDTH * CFG_DISP_PRI_RESOL_HEIGHT *
		CFG_DISP_PRI_SCREEN_PIXEL_BYTE);

	lcd_fill_rectangle(0, 0, CFG_DISP_PRI_RESOL_WIDTH, CFG_DISP_PRI_RESOL_HEIGHT, 0, 0);
	run_command(CONFIG_CMD_LOGO_UPDATE, 0);
	lcd_show_message("Waiting for update ...");
	lcd_show_progress(0);
}

void fboot_lcd_stop(void)
{
	lcd_fill_rectangle(0, 0, CFG_DISP_PRI_RESOL_WIDTH, CFG_DISP_PRI_RESOL_HEIGHT, 0, 0);
	run_command(CONFIG_CMD_LOGO_WALLPAPERS, 0);
}

void fboot_lcd_part(char *part, char *stat)
{
	lcd_show_message("%s: %s", part, stat);
}

void fboot_lcd_down(int percent)
{
	lcd_show_message("Downloading %3d%% ...",percent);
	lcd_show_progress(percent);
}

void fboot_lcd_flash(char *part, char *stat)
{
	lcd_show_message("Flashing %s: %s",part, stat);
}

void fboot_lcd_status(char *stat)
{
	lcd_show_message("%s", stat);
}

