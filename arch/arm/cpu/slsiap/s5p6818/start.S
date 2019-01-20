/*
 * armboot - Startup Code for NXPxxxx/ARM Cortex CPU-core
 */

#include <asm-offsets.h>
#include <config.h>
#include <version.h>
#include <asm/system.h>
#include <linux/linkage.h>

/*
 *************************************************************************
 *
 * Exception vectors as described in ARM reference manuals
 *
 * replace arm/lib/vectors.S
 *
 *************************************************************************
 */
	.globl _start	
_start:
	b 	reset
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

_undefined_instruction:	.word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:		.word data_abort
_not_used:		.word not_used
_irq:			.word irq
_fiq:			.word fiq

	.balignl 16,0xdeadbeef

/*
 *************************************************************************
 *
 * Text and section base
 *
 *************************************************************************
 */

.globl TEXT_BASE
TEXT_BASE:
	.word	CONFIG_SYS_TEXT_BASE

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start_ofs
_bss_start_ofs:
	.word __bss_start - _start

.globl _bss_end_ofs
_bss_end_ofs:
	.word __bss_end - _start

.globl _end_ofs
_end_ofs:
	.word _end - _start

/*
 *************************************************************************
 *
 * Reset handling
 *
 *************************************************************************
 */

	.globl reset

reset:
	bl	save_boot_params
	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr,r0

	/* disable watchdog */
	ldr	r0, =0xC0019000
	mov	r1, #0
	str	r1, [r0]

	/* the mask ROM code should have PLL and others stable */
#ifndef CONFIG_SKIP_LOWLEVEL_INIT
	bl	cpu_init_cp15
	bl	cpu_init_crit
#endif

#ifdef CONFIG_RELOC_TO_TEXT_BASE
relocate_to_text:
	/*
	 * relocate u-boot code on memory to text base
	 * for nexell arm core (add by jhkim)
	 */
	adr	r0, _start				/* r0 <- current position of code   */
	ldr	r1, TEXT_BASE			/* test if we run from flash or RAM */
	cmp r0, r1              	/* don't reloc during debug         */
	beq clear_bss

	ldr	r2, _bss_start_ofs
	add	r2, r0, r2				/* r2 <- source end address         */
	
copy_loop_text:
	ldmia	r0!, {r3-r10}		/* copy from source address [r0]    */
	stmia	r1!, {r3-r10}		/* copy to   target address [r1]    */
	cmp	r0, r2					/* until source end addreee [r2]    */
	ble	copy_loop_text

	ldr	r1, TEXT_BASE			/* restart at text base */
	mov pc, r1

clear_bss:
#ifdef CONFIG_MMU_ENABLE
	bl	mmu_turn_on
#endif
	ldr	r0, _bss_start_ofs
	ldr	r1, _bss_end_ofs
	ldr	r4, TEXT_BASE			/* text addr */
	add	r0, r0, r4
	add	r1, r1, r4
	mov	r2, #0x00000000			/* clear */

clbss_l:str	r2, [r0]			/* clear loop... */
	add	r0, r0, #4
	cmp	r0, r1
	bne	clbss_l

	ldr	sp, =(CONFIG_SYS_INIT_SP_ADDR)
	bic	sp, sp, #7					/* 8-byte alignment for ABI compliance */
	sub	sp, #GD_SIZE				/* allocate one GD above SP */
	bic	sp, sp, #7					/* 8-byte alignment for ABI compliance */
	mov	r9, sp						/* GD is above SP */
	mov	r0, #0
	bl	board_init_f

	mov	sp, r9						/* SP is GD's base address */
	bic	sp, sp, #7					/* 8-byte alignment for ABI compliance */
	sub	sp, #GENERATED_BD_INFO_SIZE	/* allocate one BD above SP */
	bic	sp, sp, #7					/* 8-byte alignment for ABI compliance */

	mov	r0, r9						/* gd_t *gd */
	ldr r1, TEXT_BASE				/* ulong text */
	mov r2, sp						/* ulong sp */
	bl	gdt_reset

	/* call board_init_r(gd_t *id, ulong dest_addr) */
	mov	r0, r9							/* gd_t */
	ldr	r1,  =(CONFIG_SYS_MALLOC_END)	/* dest_addr for malloc heap end */
	/* call board_init_r */
	ldr	pc, =board_init_r				/* this is auto-relocated! */

#else	/* CONFIG_RELOC_TO_TEXT_BASE */

	bl	_main
#endif

/*------------------------------------------------------------------------------*/

ENTRY(c_runtime_cpu_setup)
/*
 * If I-cache is enabled invalidate it
 */
#ifndef CONFIG_SYS_ICACHE_OFF
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache
#ifndef CONFIG_MACH_S5P6818
	mcr     p15, 0, r0, c7, c10, 4	@ DSB
	mcr     p15, 0, r0, c7, c5, 4	@ ISB
#endif
#endif
/*
 * Move vector table
 */
	/* Set vector address in CP15 VBAR register */
	ldr     r0, =_start
	mcr     p15, 0, r0, c12, c0, 0  @Set VBAR

	bx	lr

ENDPROC(c_runtime_cpu_setup)

/*************************************************************************
 *
 * void save_boot_params(u32 r0, u32 r1, u32 r2, u32 r3)
 *	__attribute__((weak));
 *
 * Stack pointer is not yet initialized at this moment
 * Don't save anything to stack even if compiled with -O0
 *
 *************************************************************************/
ENTRY(save_boot_params)
	bx	lr			@ back to my caller
ENDPROC(save_boot_params)
	.weak	save_boot_params

/*************************************************************************
 *
 * cpu_init_cp15
 *
 * Setup CP15 registers (cache, MMU, TLBs). The I-cache is turned on unless
 * CONFIG_SYS_ICACHE_OFF is defined.
 *
 *************************************************************************/
ENTRY(cpu_init_cp15)
	/*
	 * Invalidate L1 I/D
	 */
	mov	r0, #0			@ set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache
	mcr	p15, 0, r0, c7, c5, 6	@ invalidate BP array
#ifndef CONFIG_MACH_S5P6818
	mcr p15, 0, r0, c7, c10, 4	@ DSB
	mcr p15, 0, r0, c7, c5, 4	@ ISB
#endif

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	@ clear bits 13 (--V-)
	bic	r0, r0, #0x00000007	@ clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00000002	@ set bit 1 (--A-) Align
	orr	r0, r0, #0x00000800	@ set bit 11 (Z---) BTB
#ifdef CONFIG_SYS_ICACHE_OFF
	bic	r0, r0, #0x00001000	@ clear bit 12 (I) I-cache
#else
	orr	r0, r0, #0x00001000	@ set bit 12 (I) I-cache
#endif
	mcr	p15, 0, r0, c1, c0, 0
	mov	pc, lr			@ back to my caller
ENDPROC(cpu_init_cp15)

#ifndef CONFIG_SKIP_LOWLEVEL_INIT
/*************************************************************************
 *
 * CPU_init_critical registers
 *
 * setup important registers
 * setup memory timing
 *
 *************************************************************************/
ENTRY(cpu_init_crit)
	/*
	 * Jump to board specific initialization...
	 * The Mask ROM will have already initialized
	 * basic memory. Go here to bump up clock rate and handle
	 * wake up conditions.
	 */
	b	lowlevel_init		@ go setup pll,mux,memory
ENDPROC(cpu_init_crit)
#endif

/*************************************************************************
 *
 *  LED show status r5 and r6 will be modified
 *
 *************************************************************************/
ENTRY(blue_led_on)
led_on:
	ldr r5, =0xC001B020
	ldr r6, [r5]
	bic r6, r6, #(0x3<<24)
	orr r6, r6, #(0x2<<24)	// 设置为普通GPIO功能
	str r6, [r5]

	
	ldr r5, =0xC001B004
	ldr r6, [r5]
	orr r6, r6, #(1<<12)	// 设置为输出模式
	str r6, [r5]

	ldr r5, =0xC001B000
	ldr r6, [r5]
	orr r6, #(1<<12)	// 输出高电平
	str r6, [r5]
	mov pc, lr
ENDPROC(blue_led_on)

blue_led_off:
	ldr r5, =0xC001B020
	ldr r6, [r5]
	bic r6, r6, #(0x3<<24)
	orr r6, r6, #(0x2<<24)	// 设置为普通GPIO功能
	str r6, [r5]

	
	ldr r5, =0xC001B004
	ldr r6, [r5]
	orr r6, r6, #(1<<12)	// 设置为输出模式
	str r1, [r0]

	ldr r5, =0xC001B000
	ldr r6, [r5]
	and r6, #(~(1<<12))	// 输出高电平
	str r6, [r5]
	mov pc, lr


/*
 *  vectors - Generic ARM exception table code
 */

/*
 *************************************************************************
 *
 * Interrupt handling
 *
 *************************************************************************
 */

/* IRQ stack memory (calculated at run-time) + 8 bytes */
.globl _IRQ_STACK_START_IN_
_IRQ_STACK_START_IN_:
	.word 0x0badc0de

#ifdef CONFIG_USE_IRQ
/* IRQ stack memory (calculated at run-time) */
.globl _IRQ_STACK_START
_IRQ_STACK_START:
	.word 0x0badc0de

/* IRQ stack memory (calculated at run-time) */
.globl _FIQ_STACK_START
_FIQ_STACK_START:
	.word 0x0badc0de

#endif /* CONFIG_USE_IRQ */

@
@ IRQ stack frame.
@
#define S_FRAME_SIZE	72

#define S_OLD_R0	68
#define S_PSR		64
#define S_PC		60
#define S_LR		56
#define S_SP		52

#define S_IP		48
#define S_FP		44
#define S_R10		40
#define S_R9		36
#define S_R8		32
#define S_R7		28
#define S_R6		24
#define S_R5		20
#define S_R4		16
#define S_R3		12
#define S_R2		8
#define S_R1		4
#define S_R0		0

#define MODE_SVC 0x13
#define I_BIT	 0x80

/*
 * use bad_save_user_regs for abort/prefetch/undef/swi ...
 * use irq_save_user_regs / irq_restore_user_regs for IRQ/FIQ handling
 */

	.macro	bad_save_user_regs
	@ carve out a frame on current user stack
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}	@ Save user registers (now in svc mode) r0-r12
	ldr	r2, _IRQ_STACK_START_IN_
	@ get values for "aborted" pc and cpsr (into parm regs)
	ldmia	r2, {r2 - r3}
	add	r0, sp, #S_FRAME_SIZE		@ grab pointer to old stack
	add	r5, sp, #S_SP
	mov	r1, lr
	stmia	r5, {r0 - r3}	@ save sp_SVC, lr_SVC, pc, cpsr
	mov	r0, sp		@ save current stack into r0 (param register)
	.endm

	.macro	irq_save_user_regs
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}			@ Calling r0-r12
	@ !!!! R8 NEEDS to be saved !!!! a reserved stack spot would be good.
	add	r8, sp, #S_PC
	stmdb	r8, {sp, lr}^		@ Calling SP, LR
	str	lr, [r8, #0]		@ Save calling PC
	mrs	r6, spsr
	str	r6, [r8, #4]		@ Save CPSR
	str	r0, [r8, #8]		@ Save OLD_R0
	mov	r0, sp
	.endm

	.macro	irq_restore_user_regs
	ldmia	sp, {r0 - lr}^			@ Calling r0 - lr
	mov	r0, r0
	ldr	lr, [sp, #S_PC]			@ Get PC
	add	sp, sp, #S_FRAME_SIZE
	subs	pc, lr, #4		@ return & move spsr_svc into cpsr
	.endm

	.macro get_bad_stack
	ldr	r13, _IRQ_STACK_START_IN_		@ setup our mode stack

	str	lr, [r13]	@ save caller lr in position 0 of saved stack
	mrs	lr, spsr	@ get the spsr
	str	lr, [r13, #4]	@ save spsr in position 1 of saved stack
	mov	r13, #MODE_SVC	@ prepare SVC-Mode
	@ msr	spsr_c, r13
	msr	spsr, r13	@ switch modes, make sure moves will execute
	mov	lr, pc		@ capture return pc
	movs	pc, lr		@ jump to next instruction & switch modes.
	.endm

	.macro get_irq_stack			@ setup IRQ stack
	ldr	sp, _IRQ_STACK_START
	.endm

	.macro get_fiq_stack			@ setup FIQ stack
	ldr	sp, _FIQ_STACK_START
	.endm

/*
 * exception handlers
 */

	.align  5
	.globl undefined_instruction
undefined_instruction:
	get_bad_stack
	bad_save_user_regs
	bl	do_undefined_instruction

	.align	5
	.globl software_interrupt
software_interrupt:
	get_bad_stack
	bad_save_user_regs
	bl	do_software_interrupt

	.align	5
	.globl prefetch_abort
prefetch_abort:
	get_bad_stack
	bad_save_user_regs
	bl	do_prefetch_abort

	.align	5
	.globl data_abort
data_abort:
	get_bad_stack
	bad_save_user_regs
	bl	do_data_abort

	.align	5
	.globl not_used
not_used:
	get_bad_stack
	bad_save_user_regs
	bl	do_not_used

#ifdef CONFIG_USE_IRQ

	.align	5
	.globl irq
irq:
	get_irq_stack
	irq_save_user_regs
	bl	do_irq
	irq_restore_user_regs

	.align	5
	.globl fiq
fiq:
	get_fiq_stack
	/* someone ought to write a more effiction fiq_save_user_regs */
	irq_save_user_regs
	bl	do_fiq
	irq_restore_user_regs

#else

	.align	5
	.globl irq
irq:
	get_bad_stack
	bad_save_user_regs
	bl	do_irq

	.align	5
	.globl fiq
fiq:
	get_bad_stack
	bad_save_user_regs
	bl	do_fiq

#endif /* CONFIG_USE_IRQ */

