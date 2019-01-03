/*
 * (C) Copyright 2015
 *  Jongshin Park, Nexell Co, <pjsin865@nexell.co.kr>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <mmc.h>
#include <pwm.h>
#include <asm/io.h>
#include <asm/gpio.h>

#include <platform.h>
#include <mach-api.h>
#include <pm.h>

#include <power/pmic.h>

#include <i2c.h>
#include <errno.h>

#include "mp8845c_regulator.h"

#if 0 
static const int vout_uV_list[] = {
	6000,    // 0
	6067,    
	6134,    
	6201,    
	6268,    
	6335,    
	6401,    
	6468,    
	6535,    
	6602,    
	6669,    // 10
	6736,    
	6803,    
	6870,    
	6937,    
	7004,    
	7070,    
	7137,    
	7204,    
	7271,    
	7338,    // 20
	7405,    
	7472,    
	7539,    
	7606,    
	7673,    
	7739,    
	7806,    
	7873,    
	7940,    
	8007,    // 30
	8074,   
	8141,   
	8208,   
	8275,   
	8342,   
	8408,   
	8475,   
	8542,   
	8609,   
	8676,   // 40
	8743,   
	8810,   
	8877,   
	8944,   
	9011,   
	9077,   
	9144,   
	9211,   
	9278,   
	9345,   // 50
	9412,   
	9479,   
	9546,   
	9613,   
	9680,   
	9746,   
	9813,   
	9880,   
	9947,   
	10014,   // 60
	10081,   
	10148,   
	10215,   
	10282,   
	10349,   
	10415,   
	10482,   
	10549,   
	10616,   
	10683,   // 70
	10750,   
	10817,   
	10884,   
	10951,   
	11018,   
	11084,   
	11151,   
	11218,   
	11285,   
	11352,   // 80
	11419,   
	11486,   
	11553,   
	11620,   
	11687,   
	11753,   
	11820,   
	11887,   
	11954,   
	12021,   // 90
	12088,   
	12155,   
	12222,   
	12289,   
	12356,   
	12422,
	12489,
	12556,
	12623,
	12690,   // 100
	12757,
	12824,
	12891,
	12958,
	13025,
	13091,
	13158,
	13225,
	13292,
	13359,   // 110
	13426,
	13493,
	13560,
	13627,
	13694,
	13760,
	13827,
	13894,
	13961,
	14028,   // 120
	14095,
	14162,
	14229,
	14296,
	14363,
	14429,
	14496,
};

static u8 mp8845c_get_voltage_list(int vol)
{
	int i=0;
	for(i=0; i<ARRAY_SIZE(vout_uV_list); i++)
	{
		if(vol <= ri->voltages[i])
			break;
	}
	return (u8)i;
}
#endif

static int mp8845c_i2c_read(struct mp8845c_power *power, u8 reg, u8 *value)
{
	uchar chip = power->i2c_addr;
	u32   addr = (u32)reg & 0xFF;
	int   alen = 1;
	return i2c_read(chip, addr, alen, value, 1);
}

static int mp8845c_i2c_write(struct mp8845c_power *power, u8 reg, u8 value)
{
	uchar chip = power->i2c_addr;
	u32   addr = (u32)reg & 0xFF;
	int   alen = 1;
	return i2c_write(chip, addr, alen, &value, 1);
}

static int mp8845c_i2c_set_bits(struct mp8845c_power *power, int reg, uint8_t bit_mask)
{
	uint8_t reg_val;
	int ret = 0;

	ret = mp8845c_i2c_read(power, reg, &reg_val);
	if (ret)
		return ret;

	if ((reg_val & bit_mask) != bit_mask) {
		reg_val |= bit_mask;
		ret = mp8845c_i2c_write(power, reg, reg_val);
	}
	return ret;
}

#if defined(CONFIG_PMIC_REG_DUMP)
void mp8845c_register_dump(struct mp8845c_power *power)
{
	s32 ret=0;
	u16 i=0;
	u8 value=0;

	printf("##########################################################\n");
	printf("##\e[31m %s()\e[0m \n", __func__);
	printf("----------------------------------------------------------\n");
	printf("       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F\n");

	for(i=0; i<=0x5; i++)
	{
		if(i%16 == 0)
			printf("  %02X:", i);

		if(i%4 == 0)
			printf(" ");

		ret = mp8845c_i2c_read(power, i, &value);
		if(!ret)
			printf("%02x ", value);
		else
			printf("\e[31mxx\e[0m ");

		if((i+1)%16 == 0)
			printf("\n");
	}
	printf("\n##########################################################\n");

}
#endif


#if 0 
static int mp8845c_i2c_clr_bits(struct mp8845c_power *power, int reg, uint8_t bit_mask)
{
	uint8_t reg_val;
	int ret = 0;

	ret = mp8845c_i2c_read(power, reg, &reg_val);
	if (ret)
		return ret;

	if (reg_val & bit_mask) {
		reg_val &= ~bit_mask;
		ret = mp8845c_i2c_write(power, reg, reg_val);
	}
	return ret;
}

static int mp8845c_i2c_update(struct mp8845c_power *power, int reg, uint8_t val, uint8_t mask)
{
	u8 reg_val;
	int ret = 0;

	ret = mp8845c_i2c_read(power, reg, &reg_val);
	if (ret)
		return ret;

	if ((reg_val & mask) != val) {
		reg_val = (reg_val & ~mask) | val;
		ret = mp8845c_i2c_write(power, reg, reg_val);
	}
	return ret;
}
#endif

static int mp8845c_device_setup(struct mp8845c_power *power)
{
	int	bus = power->i2c_bus;
	int ret = 0;
	u8 reg_val = 0;

	PMIC_DBGOUT("%s\n", __func__);

	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	i2c_set_bus_num(bus);

	mp8845c_i2c_set_bits(power, MP8845C_REG_SYSCNTL1, (1 << MP8845C_POS_MODE));

	reg_val = 0xCB; // 1.1018 V
	mp8845c_i2c_set_bits(power, MP8845C_REG_VSEL, reg_val);

#if defined(CONFIG_PMIC_REG_DUMP)
	mp8845c_register_dump(power);
#endif

	return ret;
}


int bd_pmic_init(void)
{
	struct mp8845c_power nxe_power_config = {
		.i2c_addr = MP8845C_I2C_ADDR,
	};
	PMIC_DBGOUT("%s\n", __func__);

	nxe_power_config.i2c_bus = CONFIG_PMIC_I2C_BUSA;
	mp8845c_device_setup(&nxe_power_config);

	nxe_power_config.i2c_bus = CONFIG_PMIC_I2C_BUSB;
	mp8845c_device_setup(&nxe_power_config);
	return 0;
}



