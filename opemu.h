#pragma once

#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include "libudis86/extern.h"

static inline int
is_saved_state64(struct pt_regs *regs)
{
    uint16_t REG_CS = regs->cs;
    if ((REG_CS & 0x10) == 0x10)
        return 1;
    else
        return 0;
}


struct op {
	// one of either. order here matters.
	union {
		struct pt_regs *state64;
		struct pt_regs *state32;
	};

	enum {
		SAVEDSTATE_64,
		SAVEDSTATE_32,
	} state_flavor;

	// just another version of the above
	struct pt_regs *state;

	// disassembly object
	ud_t		*ud_obj;

	uint8_t	dst64;
	uint8_t	dst32;

	uint64_t	res64;
	uint32_t	res32;
	// boolean flag
	uint8_t		ring0;
};
typedef struct op op_t;

/**
 * Trap handlers, analogous to a program's entry point
 * @param state: xnu's trap.c saved thread state
 */
int opemu_utrap(struct pt_regs *regs);


/**
 * Forward declarations of private xnu functions
 
extern void mach_call_munger(x86_saved_state_t *state);
extern void unix_syscall(x86_saved_state_t *);
extern void mach_call_munger64(x86_saved_state_t *state);
extern void unix_syscall64(x86_saved_state_t *);
*/
int retrieve_reg(/*const*/ struct pt_regs *regs, const ud_type_t, uint64_t *);

/**
 * Entry points for the "plugins"
 */
extern int op_sse3x_run(op_t*);
extern int op_sse3_run(const op_t*);

