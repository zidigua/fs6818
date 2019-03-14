/*
 * (C) Copyright 2009 Nexell Co.,
 * jung hyun kim<jhkim@nexell.co.kr>
 *
 * Configuation settings for the Nexell board.
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

/*
 * Misc boot support
 */
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <nand.h>
#include <platform.h>
#include <mach-types.h>
#include <mach-api.h>

DECLARE_GLOBAL_DATA_PTR;

typedef void  (IMAGE)(unsigned long, unsigned long);

#if defined(CONFIG_SETUP_MEMORY_TAGS) || \
	defined(CONFIG_CMDLINE_TAG) || \
	defined(CONFIG_INITRD_TAG) || \
	defined(CONFIG_SERIAL_TAG) || \
	defined(CONFIG_REVISION_TAG)
static struct tag *params;
#endif

#if defined(CONFIG_SETUP_MEMORY_TAGS) || \
	defined(CONFIG_CMDLINE_TAG) || \
	defined(CONFIG_INITRD_TAG) || \
	defined(CONFIG_SERIAL_TAG) || \
	defined(CONFIG_REVISION_TAG)
static void setup_start_tag (bd_t *bd)
{
	params = (struct tag *)bd->bi_boot_params;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}
#endif

#ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags(bd_t *bd)
{
	int i;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		params->hdr.tag = ATAG_MEM;
		params->hdr.size = tag_size (tag_mem32);

		params->u.mem.start = bd->bi_dram[i].start;
		params->u.mem.size = bd->bi_dram[i].size;

		params = tag_next (params);
	}
}
#endif

#ifdef CONFIG_CMDLINE_TAG
static void setup_commandline_tag(bd_t *bd, char *commandline)
{
	char *p;

	if (!commandline)
		return;

	/* eat leading white space */
	for (p = commandline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy (params->u.cmdline.cmdline, p);

	params = tag_next (params);
}
#endif

#ifdef CONFIG_INITRD_TAG
static void setup_initrd_tag(bd_t *bd, ulong initrd_start, ulong initrd_end)
{
	/* an ATAG_INITRD node tells the kernel where the compressed
	 * ramdisk can be found. ATAG_RDIMG is a better name, actually.
	 */
	params->hdr.tag = ATAG_INITRD2;
	params->hdr.size = tag_size (tag_initrd);

	params->u.initrd.start = initrd_start;
	params->u.initrd.size = initrd_end - initrd_start;

	params = tag_next (params);
}
#endif

#ifdef CONFIG_SERIAL_TAG
void setup_serial_tag(struct tag **tmp)
{
	struct tag *params = *tmp;
	struct tag_serialnr serialnr;
	void get_board_serial(struct tag_serialnr *serialnr);

	get_board_serial(&serialnr);
	params->hdr.tag = ATAG_SERIAL;
	params->hdr.size = tag_size (tag_serialnr);
	params->u.serialnr.low = serialnr.low;
	params->u.serialnr.high= serialnr.high;
	params = tag_next (params);
	*tmp = params;
}
#endif

#ifdef CONFIG_REVISION_TAG
void setup_revision_tag(struct tag **in_params)
{
	u32 rev = 0;
	u32 get_board_rev(void);

	rev = get_board_rev();
	params->hdr.tag = ATAG_REVISION;
	params->hdr.size = tag_size (tag_revision);
	params->u.revision.rev = rev;
	params = tag_next (params);
}
#endif

#if defined(CONFIG_SETUP_MEMORY_TAGS) || \
	defined(CONFIG_CMDLINE_TAG) || \
	defined(CONFIG_INITRD_TAG) || \
	defined(CONFIG_SERIAL_TAG) || \
	defined(CONFIG_REVISION_TAG)
static void setup_end_tag(bd_t *bd)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}
#endif

#ifdef CONFIG_MMU_ENABLE
extern void disable_mmu(void);
#endif

int do_goImage (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong addr = 0;
	long  machtype = machine_arch_type;
#ifdef CONFIG_CMDLINE_TAG
	char *commandline = getenv("bootargs");
#endif

	void (*entry)(unsigned long, unsigned long) = NULL;

	if (argc < 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	/* get machine type */
	if (argc == 3)
		machtype = simple_strtol(argv[2], NULL, 10);	/* get interger machine type */

	/* get start address */
	addr = simple_strtoul(argv[1], NULL, 16);

#if defined(CONFIG_SETUP_MEMORY_TAGS) || \
	defined(CONFIG_CMDLINE_TAG) || \
	defined(CONFIG_INITRD_TAG) || \
	defined(CONFIG_SERIAL_TAG) || \
	defined(CONFIG_REVISION_TAG)

	setup_start_tag(gd->bd);

	#ifdef CONFIG_SERIAL_TAG
	setup_serial_tag(&params);
	#endif
	#ifdef CONFIG_CMDLINE_TAG
	setup_commandline_tag(gd->bd, commandline);
	#endif
	#ifdef CONFIG_REVISION_TAG
	setup_revision_tag(&params);
	#endif
	#ifdef CONFIG_SETUP_MEMORY_TAGS
	setup_memory_tags(gd->bd);
	#endif
	#ifdef CONFIG_INITRD_TAG
	if (images->rd_start && images->rd_end)
		setup_initrd_tag(gd->bd, images->rd_start,
		images->rd_end);
	#endif
	setup_end_tag(gd->bd);
#endif
	printf ("## Starting Image at 0x%08X with machine type %d ...\n",
		(u_int)addr, (u_int)machtype);

	entry = (IMAGE*)addr;

#ifdef CONFIG_MMU_ENABLE
	disable_mmu();
#endif

	entry(addr, machtype);
	return 0;
}

U_BOOT_CMD(
	goimage, 3, 1,	do_goImage,
	"start Image at address 'addr'",
	"addr\n"
	"    - start Image at address 'addr' with default machine type\n"
	"addr type\n"
	"    - start Image at address 'addr' with machine type integer\n"
);

int do_new_cmd (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char *cmd;

	if (argc < 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	cmd = argv[1];

	if (strcmp(cmd, "delay") == 0) {
		struct clk *clk = clk_get(NULL, "pclk");
		long rate;
		int delay, i = 0;
		if (3 != argc) {
			printf("no delay time second\n");
			return 0;
		}
		delay = simple_strtol(argv[2], NULL, 10);	/* get interger machine type */
		rate  = clk_get_rate(clk);
		printf("start delay %d sec (pclk=%ld)\n", delay, rate);
		for (i = 0; delay > i; i++) {
			mdelay(i*1000);
			printf("delay %3d sec\n", i);
		}
		printf("done\n");
		return 0;
	}

#if defined(CONFIG_CMD_NAND) && defined (CONFIG_MTD_NAND)
	if (strcmp(cmd, "nandbad") == 0) {
		int i, n;
		int count = 0;

		if (3 == argc)
			count = simple_strtol(argv[2], NULL, 10);	/* get interger machine type */

		if (! count)
			count = 1;

		for (n = 0; count > n; n++) {
			for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE; i++) {
				if (nand_info[i].name) {
		    		struct mtd_info  *mtd  = &nand_info[i];
    				struct nand_chip *chip = mtd->priv;
					loff_t off;

					if (chip->bbt) {
						kfree(chip->bbt);
						chip->bbt = NULL;
					}
					chip->options &= ~NAND_BBT_SCANNED;
    				chip->scan_bbt(mtd);
					chip->options |=  NAND_BBT_SCANNED;

					printf("\n[cnt : %6d] Device %d rescan bad blocks:\n", n, i);
					for (off = 0; off < mtd->size; off += mtd->erasesize)
						if (nand_block_isbad(mtd, off))
							printf("  %08llx\n", (unsigned long long)off);

					if (ctrlc())
						return 0;
    			}
    		}
		}
		return 0;
	}
#endif
	printf(" end command : %s\n", cmd);
	return 0;
}

U_BOOT_CMD(
	cmd, CONFIG_SYS_MAXARGS, 1,	do_new_cmd,
	"cmd [command] options...",
	"nandbad cnt\n"
	"    - rescan bad block for cnt\n"
);

