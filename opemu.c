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
	//const uint32_t mnemonic = ud_insn_mnemonic(&ud_obj);
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
                default:
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
                default:
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
 * @param length: reg's size will be stored there
 * @param where: reg's value will be stored there
 * @return: zero if the state could be retrieved
 */
int retrieve_reg(/*const*/ struct pt_regs *regs, const ud_type_t base, uint8_t *length, uint64_t *where)
{
	const struct pt_regs *ss64 = regs;
	const struct pt_regs *ss32 = regs;
	uint8_t islongmode = is_saved_state64(regs);

	switch (base) {

	case UD_NONE:
		*where = 0;
        if (length != NULL)
        {
            *length = 0;
        }
		break;

	case UD_R_RIP:
		*where = ss64 -> ip;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_RAX:
		*where = ss64 -> ax;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_RCX:
		*where = ss64 -> cx;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_RDX:
		*where = ss64 -> dx;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_RBX:
		*where = ss64 -> bx;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_RSP:
		*where = ss64 -> sp;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_RBP:
		*where = ss64 -> bp;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_RSI:
		*where = ss64 -> si;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_RDI:
		*where = ss64 -> di;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_R8:
		*where = ss64 -> r8;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_R9:
		*where = ss64 -> r9;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_R10:
		*where = ss64 -> r10;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_R11:
		*where = ss64 -> r11;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_R12:
		*where = ss64 -> r12;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_R13:
		*where = ss64 -> r13;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_R14:
		*where = ss64 -> r14;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	case UD_R_R15:
		*where = ss64 -> r15;
        if (length != NULL)
        {
            *length = 8;
        }
		break;

	/* 32 bit general purpose */

	case UD_R_EAX:
        if (islongmode)
        {
            *where = ss64 -> ax & 0xFFFFFFFF;
        } else {
            *where = ss32 -> ax;
        }
        if (length != NULL)
        {
            *length = 4;
        }
		break;

	case UD_R_ECX:
        if (islongmode)
        {
            *where = ss64 -> cx & 0xFFFFFFFF;
        } else {
            *where = ss32 -> cx;
        }
        if (length != NULL)
        {
            *length = 4;
        }
		break;

	case UD_R_EDX:
        if (islongmode)
        {
            *where = ss64 -> dx & 0xFFFFFFFF;
        } else {
            *where = ss32 -> dx;
        }
        if (length != NULL)
        {
            *length = 4;
        }
		break;

	case UD_R_EBX:
        if (islongmode)
        {
            *where = ss64 -> bx & 0xFFFFFFFF;
        } else {
            *where = ss32 -> bx;
        }
        if (length != NULL)
        {
            *length = 4;
        }
		break;

	case UD_R_ESP:
        if (islongmode)
        {
            *where = ss64 -> sp & 0xFFFFFFFF;
        } else {
            *where = ss32 -> sp;
        }
        if (length != NULL)
        {
            *length = 4;
        }
		break;

	case UD_R_EBP:
        if (islongmode)
        {
            *where = ss64 -> bp & 0xFFFFFFFF;
        } else {
            *where = ss32 -> bp;
        }
        if (length != NULL)
        {
            *length = 4;
        }
		break;

	case UD_R_ESI:
        if (islongmode)
        {
            *where = ss64 -> si & 0xFFFFFFFF;
        } else {
            *where = ss32 -> si;
        }
        if (length != NULL)
        {
            *length = 4;
        }
		break;

	case UD_R_EDI:
        if (islongmode)
        {
            *where = ss64 -> di & 0xFFFFFFFF;
        } else {
            *where = ss32 -> di;
        }
        if (length != NULL)
        {
            *length = 4;
        }
		break;

    /* 16 bit general purpose */

    case UD_R_AX:
        if (islongmode)
        {
            *where = ss64 -> ax & 0xFFFF;
        } else {
            *where = (ss32->ax & 0xFFFF);
        }
        if (length != NULL)
        {
            *length = 2;
        }
        break;

    case UD_R_CX:
        if (islongmode)
        {
            *where = ss64 -> cx & 0xFFFF;
        } else {
            *where = (ss32->cx & 0xFFFF);
        }
        if (length != NULL)
        {
            *length = 2;
        }
        break;

    case UD_R_DX:
        if (islongmode)
        {
            *where = ss64 -> dx & 0xFFFF;
        } else {
            *where = (ss32->dx & 0xFFFF);
        }
        if (length != NULL)
        {
            *length = 2;
        }
        break;

    case UD_R_BX:
        if (islongmode)
        {
            *where = ss64 -> bx & 0xFFFF;
        } else {
            *where = (ss32->bx & 0xFFFF);
        }
        if (length != NULL)
        {
            *length = 2;
        }
        break;

    case UD_R_SP:
        if (islongmode)
        {
            *where = ss64 -> sp & 0xFFFF;
        } else {
            *where = (ss32->sp & 0xFFFF);
        }
        if (length != NULL)
        {
            *length = 2;
        }
        break;

    case UD_R_BP:
        if (islongmode)
        {
            *where = ss64 -> bp & 0xFFFF;
        } else {
            *where = (ss32->bp & 0xFFFF);
        }
        if (length != NULL)
        {
            *length = 2;
        }
        break;

    case UD_R_SI:
        if (islongmode)
        {
            *where = ss64 -> si & 0xFFFF;
        } else {
            *where = (ss32->si & 0xFFFF);
        }
        if (length != NULL)
        {
            *length = 2;
        }
            break;

    case UD_R_DI:
        if (islongmode)
        {
            *where = ss64 -> di & 0xFFFF;
        } else {
            *where = (ss32->di & 0xFFFF);
        }
        if (length != NULL)
        {
            *length = 2;
        }
        break;

    /* 8 bit general purpose */

    case UD_R_AL:
        if (islongmode)
        {
            *where = ss64 -> ax & 0xFF;
        } else {
            *where = (ss32->ax & 0xFF);
        }
        if (length != NULL)
        {
            *length = 1;
        }
        break;

    case UD_R_CL:
        if (islongmode)
        {
            *where = ss64 -> cx & 0xFF;
        } else {
            *where = (ss32->cx & 0xFF);
        }
        if (length != NULL)
        {
            *length = 1;
        }
        break;

     case UD_R_DL:
        if (islongmode)
        {
            *where = ss64 -> dx & 0xFF;
        } else {
            *where = (ss32->dx & 0xFF);
        }
        if (length != NULL)
        {
            *length = 1;
        }
        break;

     case UD_R_BL:
        if (islongmode)
        {
            *where = ss64 -> bx & 0xFF;
        } else {
            *where = (ss32->bx & 0xFF);
        }
        if (length != NULL)
        {
            *length = 1;
        }
        break;

     case UD_R_SPL:
        if (islongmode)
        {
            *where = ss64 -> sp & 0xFF;
        } else {
            *where = (ss32->sp & 0xFF);
        }
        if (length != NULL)
        {
            *length = 1;
        }
        break;

     case UD_R_BPL:
        if (islongmode)
        {
            *where = ss64 -> bp & 0xFF;
        } else {
            *where = (ss32->bp & 0xFF);
        }
        if (length != NULL)
        {
            *length = 1;
        }
        break;

    case UD_R_SIL:
        if (islongmode)
        {
            *where = ss64 -> si & 0xFF;
        } else {
            *where = (ss32->si & 0xFF);
        }
        if (length != NULL)
        {
            *length = 1;
        }
        break;

    case UD_R_DIL:
        if (islongmode)
        {
            *where = ss64 -> di & 0xFF;
        } else {
            *where = (ss32->di & 0xFF);
        }
        if (length != NULL)
        {
            *length = 1;
        }
        break;

	default: goto bad;

	}

    return 0;

    // Only reached if bad
bad:
    return -1;
}

/**
 * Store to general purpose register
 * @param saved_state: the saved state
 * @param base:  the register type itself
 * @param what: value that will be stored
 * @return: zero if the state could be stored
 */
int store_reg(/*const*/ struct pt_regs *regs, const ud_type_t base, uint64_t what)
{
	struct pt_regs *ss64 = regs;
	struct pt_regs *ss32 = regs;
	uint8_t islongmode = is_saved_state64(regs);

	switch (base) {

        case UD_R_RIP:
            ss64->ip = what;
            break;

        case UD_R_RAX:
            ss64->ax = what;
            break;

        case UD_R_RCX:
            ss64->cx = what;
            break;

        case UD_R_RDX:
            ss64->dx = what;
            break;

        case UD_R_RBX:
            ss64->bx = what;
            break;

        case UD_R_RSP:
            ss64->sp = what;
            break;

        case UD_R_RBP:
            ss64->bp = what;
            break;

        case UD_R_RSI:
            ss64->si = what;
            break;

        case UD_R_RDI:
            ss64->di = what;
            break;

        case UD_R_R8:
            ss64->r8 = what;
            break;

        case UD_R_R9:
            ss64->r9 = what;
            break;

        case UD_R_R10:
            ss64->r10 = what;
            break;

        case UD_R_R11:
            ss64->r11 = what;
            break;

        case UD_R_R12:
            ss64->r12 = what;
            break;

        case UD_R_R13:
            ss64->r13 = what;
            break;

        case UD_R_R14:
            ss64->r14 = what;
            break;

        case UD_R_R15:
            ss64->r15 = what;
            break;

            /* 32 bit general purpose */

        case UD_R_EAX:
            if (islongmode)
            {
                ss64->ax = what & 0xFFFFFFFF;
            } else {
                ss32->ax = (uint32_t)what;
            }
            break;

        case UD_R_ECX:
            if (islongmode)
            {
                ss64->cx = what & 0xFFFFFFFF;
            } else {
                ss32->cx = (uint32_t)what;
            }
            break;

        case UD_R_EDX:
            if (islongmode)
            {
                ss64->dx = what & 0xFFFFFFFF;
            } else {
                ss32->dx = (uint32_t)what;
            }
            break;

        case UD_R_EBX:
            if (islongmode)
            {
                ss64->bx = what & 0xFFFFFFFF;
            } else {
                ss32->bx = (uint32_t)what;
            }
            break;

        case UD_R_ESP:
            if (islongmode)
            {
                ss64->sp = what & 0xFFFFFFFF;
            } else {
                ss32->sp = (uint32_t)what;
            }
            break;

        case UD_R_EBP:
            if (islongmode)
            {
                ss64->bp = what & 0xFFFFFFFF;
            } else {
                ss32->bp = (uint32_t)what;
            }
            break;

        case UD_R_ESI:
            if (islongmode)
            {
                ss64->si = what & 0xFFFFFFFF;
            } else {
                ss32->si = (uint32_t)what;
            }
            break;

        case UD_R_EDI:
            if (islongmode)
            {
                ss64->di = what & 0xFFFFFFFF;
            } else {
                ss32->di = (uint32_t)what;
            }
            break;

            /* 16 bit general purpose */

        case UD_R_AX:
            if (islongmode)
            {
                ss64->ax = what & 0xFFFF;
            } else {
                ss32->ax = (uint32_t)(what & 0xFFFF);
            }
            break;

        case UD_R_CX:
            if (islongmode)
            {
                ss64->cx = what & 0xFFFF;
            } else {
                ss32->cx = (uint32_t)(what & 0xFFFF);
            }
            break;

        case UD_R_DX:
            if (islongmode)
            {
                ss64->dx = what & 0xFFFF;
            } else {
                ss32->dx = (uint32_t)(what & 0xFFFF);
            }
            break;

        case UD_R_BX:
            if (islongmode)
            {
                ss64->bx = what & 0xFFFF;
            } else {
                ss32->bx = (uint32_t)(what & 0xFFFF);
            }
            break;

        case UD_R_SP:
            if (islongmode)
            {
                ss64->sp = what & 0xFFFF;
            } else {
                ss32->sp = (uint32_t)(what & 0xFFFF);
            }
            break;

        case UD_R_BP:
            if (islongmode)
            {
                ss64->bp = what & 0xFFFF;
            } else {
                ss32->bp = (uint32_t)(what & 0xFFFF);
            }
            break;

        case UD_R_SI:
            if (islongmode)
            {
                ss64->si = what & 0xFFFF;
            } else {
                ss32->si = (uint32_t)(what & 0xFFFF);
            }
            break;

        case UD_R_DI:
            if (islongmode)
            {
                ss64->di = what & 0xFFFF;
            } else {
                ss32->di = (uint32_t)(what & 0xFFFF);
            }
            break;

            /* 8 bit general purpose */

        case UD_R_AL:
            if (islongmode)
            {
                ss64->ax = what & 0xFF;
            } else {
                ss32->ax = (uint32_t)(what & 0xFF);
            }
            break;

        case UD_R_CL:
            if (islongmode)
            {
                ss64->cx = what & 0xFF;
            } else {
                ss32->cx = (uint32_t)(what & 0xFF);
            }
            break;

        case UD_R_DL:
            if (islongmode)
            {
                ss64->dx = what & 0xFF;
            } else {
                ss32->dx = (uint32_t)(what & 0xFF);
            }
            break;

        case UD_R_BL:
            if (islongmode)
            {
                ss64->bx = what & 0xFF;
            } else {
                ss32->bx = (uint32_t)(what & 0xFF);
            }
            break;

        case UD_R_SPL:
            if (islongmode)
            {
                ss64->sp = what & 0xFF;
            } else {
                ss32->sp = (uint32_t)(what & 0xFF);
            }
            break;

        case UD_R_BPL:
            if (islongmode)
            {
                ss64->bp = what & 0xFF;
            } else {
                ss32->bp = (uint32_t)(what & 0xFF);
            }
            break;

        case UD_R_SIL:
            if (islongmode)
            {
                ss64->si = what & 0xFF;
            } else {
                ss32->si = (uint32_t)(what & 0xFF);
            }
            break;

        case UD_R_DIL:
            if (islongmode)
            {
                ss64->di = what & 0xFF;
            } else {
                ss32->di = (uint32_t)(what & 0xFF);
            }
            break;

        default: goto bad;
	}

    return 0;

    // Only reached if bad
bad:
    return -1;
}
