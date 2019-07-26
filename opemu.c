/*                                                                  8888888 8888888 8888888 
                                                                      888     888     888   
                                                                      888     888     888   
             .d88b.  88888b.   .d88b.  88888b.d88b.  888  888         888     888     888   
            d88""88b 888 "88b d8P  Y8b 888 "888 "88b 888  888         888     888     888   
            888  888 888  888 88888888 888  888  888 888  888         888     888     888   
            Y88..88P 888 d88P Y8b.     888  888  888 Y88b 888         888     888     888   
             "Y88P"  88888P"   "Y8888  888  888  888  "Y88888       8888888 8888888 8888888 
                     888                                                                    
                     888                                                                    
                     888                                                                    
IM      ~ SINETEK                                                                           
        ~ ANV                                                                               
        ~ BRONZOVKA                                                                         
                                                                                          */
/**
 * This is called the Opcode Emulator: it traps invalid opcode exceptions
 *   and modifies the state of the running thread accordingly.
 * There are two entry points, one for user space exceptions, and another for
 *   exceptions coming from kernel space.
 *
 * STATUS
 *  . RDMSR/WRMSR is implemented.
 *  . SYSENTER is implemented.
 *  . SYSEXIT is implemented
 *  . SSSE3 is implemented.
 *  . SSE42 is partly implemented. (Needs testing)
 *  . SSE3 is implemented. (Needs testing)
 *
 * HISTORY
 *  . SINETEK  Big cleanup, bumping version
 *  . AnV Added sysexit, adapt opemu ktrap
 *  . AnV Added a few SSE42 instructions
 */

#include "opemu.h"

/**
 * Run the opcode emulator on a userspace thread.
 */
int opemu_utrap(struct pt_regs *regs)
{
	uint8_t islongmode = is_saved_state64(regs);
	const uint8_t *code_stream;
	uint8_t bytes_skip = 0;

	ud_t ud_obj;		// disassembler object
	op_t op_obj;

	if (islongmode) {
		code_stream = (const uint8_t*) regs->ip;
	} else {
		uint64_t ip = regs->ip; // prevent warning
		code_stream = (const uint8_t*) ip;
	}

	ud_init(&ud_obj);
	ud_set_input_buffer(&ud_obj, code_stream, 15);	// TODO dangerous
	ud_set_mode(&ud_obj, 64);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);
	ud_set_vendor(&ud_obj, UD_VENDOR_ANY);

	bytes_skip = ud_disassemble(&ud_obj);
	if ( bytes_skip == 0 ) goto bad;
	const uint32_t mnemonic = ud_insn_mnemonic(&ud_obj);
	//printk("OPEMU:  %s\n", ud_insn_asm(&ud_obj));

	int error = 0;
	// fill in the opemu object
	op_obj.state = regs;
	op_obj.state64 = regs;
	op_obj.state32 = regs;
	op_obj.state_flavor = (islongmode) ? SAVEDSTATE_64 : SAVEDSTATE_32;
	op_obj.ud_obj = &ud_obj;
	op_obj.ring0 = 0;

	op_obj.dst64 = op_obj.dst32 = 0 ;

	error |= op_sse3x_run(&op_obj);

	if (error)
	{
		error = 0;
		error |= op_sse3_run(&op_obj);
	}
	
	if (!error) goto cleanexit;

	/** fallthru **/
bad:
	{
		/* Well, now go in and get the asm text at least.. */
		const char *instruction_asm;
		instruction_asm = ud_insn_asm(&ud_obj);

		printk("OPEMU:  %s\n", instruction_asm);
		//i386_exception (EXC_BAD_INSTRUCTION, EXC_I386_INVOP, 0);
		return 0;
	}

cleanexit:
	/*if (islongmode) saved_state64(state)->isf.rip += bytes_skip;
	else*/

	if (op_obj.dst64)
		{
					//printk("oops");

			switch (ud_insn_opr(&ud_obj, 0)->base) {
				case UD_R_RAX:
					regs->ax = op_obj.res64;
					break;
				case UD_R_RSP:
					regs->sp = op_obj.res64;
					break;
				case UD_R_RCX:
					regs->cx = op_obj.res64;
					break;
				case UD_R_RDX:
					regs->dx = op_obj.res64;
					break;
				case UD_R_RBX:
					regs->bx = op_obj.res64;
					break;
			}
		}
		
	if (op_obj.dst32)
		{
					printk("oops32");

			switch (ud_insn_opr(&ud_obj, 0)->base) {
				case UD_R_EAX:
					regs->ax = op_obj.res32;
					break;
				case UD_R_ESP:
					regs->sp = op_obj.res32;
					break;
				case UD_R_ECX:
					regs->cx = op_obj.res32;
					break;
				case UD_R_EDX:
					regs->dx = op_obj.res32;
					break;
				case UD_R_EBX:
					regs->bx = op_obj.res32;
					break;
			}
		}
	regs->ip += bytes_skip;
	//thread_exception_return();
	/** NOTREACHED **/
	//__builtin_unreachable(); // clang extension
	return 1;
}

/**
 * Retrieve a general purpose value register's value
 * @param saved_state: the saved state
 * @param base:  the register type itself
 * @param where: reg's value will be stored there
 * @return: zero if the state could be retrieved
 */
int retrieve_reg(/*const*/ struct pt_regs *regs, const ud_type_t base, uint64_t *where)
{
	const struct pt_regs *ss64 = regs;
	const struct pt_regs *ss32 = regs;

	switch (base) {

	case UD_NONE:
		*where = 0;
		break;

	// TODO what if 32?
	case UD_R_RIP:
		*where = ss64 -> ip;
		break;

	case UD_R_RAX:
		*where = ss64 -> ax;
		break;

	case UD_R_RCX:
		*where = ss64 -> cx;
		break;

	case UD_R_RDX:
		*where = ss64 -> dx;
		break;

	case UD_R_RBX:
		*where = ss64 -> bx;
		break;

	case UD_R_RSP:
		*where = ss64 -> sp;
		break;

	case UD_R_RBP:
		*where = ss64 -> bp;
		break;

	case UD_R_RSI:
		*where = ss64 -> si;
		break;

	case UD_R_RDI:
		*where = ss64 -> di;
		break;

	case UD_R_R8:
		*where = ss64 -> r8;
		break;

	case UD_R_R9:
		*where = ss64 -> r9;
		break;

	case UD_R_R10:
		*where = ss64 -> r10;
		break;

	case UD_R_R11:
		*where = ss64 -> r11;
		break;

	case UD_R_R12:
		*where = ss64 -> r12;
		break;

	case UD_R_R13:
		*where = ss64 -> r13;
		break;

	case UD_R_R14:
		*where = ss64 -> r14;
		break;

	case UD_R_R15:
		*where = ss64 -> r15;
		break;

	/* 32 bit general purpose */

	case UD_R_EAX:
		*where = ss32 -> ax;
		break;

	case UD_R_ECX:
		*where = ss32 -> cx;
		break;

	case UD_R_EDX:
		*where = ss32 -> dx;
		break;

	case UD_R_EBX:
		*where = ss32 -> bx;
		break;

	case UD_R_ESP:
		*where = ss32 -> sp;
		break;

	case UD_R_EBP:
		*where = ss32 -> bp;
		break;

	case UD_R_ESI:
		*where = ss32 -> si;
		break;

	case UD_R_EDI:
		*where = ss32 -> di;
		break;

	default: goto bad;

	}		

    return 0;

    // Only reached if bad
bad:
    return -1;
}

