/*
 * (C) Copyright 2010
 * Jason Kridner <jkridner@beagleboard.org>
 *
 * Based on cmd_led.c patch from:
 * http://www.mail-archive.com/u-boot@lists.denx.de/msg06873.html
 * (C) Copyright 2008
 * Ulf Samuelsson <ulf.samuelsson@atmel.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <config.h>
#include <command.h>

extern void red_led_on(void);
extern void green_led_on(void);
extern void blue_led_on(void);
extern void red_led_off(void);
extern void green_led_off(void);
extern void blue_led_off(void);
int do_led (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	static int value = 0;
	if(value == 0) {
		blue_led_on();
		value = 1;
	} else {
		blue_led_off();
		value = 0;
	}

	return 0;
}

U_BOOT_CMD(
	led, 1, 0, do_led,"", ""
);
