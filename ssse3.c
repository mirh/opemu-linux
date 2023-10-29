/*
             .d8888b.   .d8888b.   .d8888b.  8888888888 .d8888b.
            d88P  Y88b d88P  Y88b d88P  Y88b 888       d88P  Y88b
            Y88b.      Y88b.      Y88b.      888            .d88P
             "Y888b.    "Y888b.    "Y888b.   8888888       8888"
                "Y88b.     "Y88b.     "Y88b. 888            "Y8b.
                  "888       "888       "888 888       888    888
            Y88b  d88P Y88b  d88P Y88b  d88P 888       Y88b  d88P
             "Y8888P"   "Y8888P"   "Y8888P"  8888888888 "Y8888P"
*/

#include "opemu.h"
#include "ssse3_priv.h"
#include <linux/module.h>
#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/uaccess.h>


/**
 * Store xmm register somewhere in memory
 */
inline void _sstore_xmm (const uint8_t n, __uint128_t *where)
{
	switch (n) {
case 0:  storedqu_template(0, where); break;
case 1:  storedqu_template(1, where); break;
case 2:  storedqu_template(2, where); break;
case 3:  storedqu_template(3, where); break;
case 4:  storedqu_template(4, where); break;
case 5:  storedqu_template(5, where); break;
case 6:  storedqu_template(6, where); break;
case 7:  storedqu_template(7, where); break;
case 8:  storedqu_template(8, where); break;
case 9:  storedqu_template(9, where); break;
case 10: storedqu_template(10, where); break;
case 11: storedqu_template(11, where); break;
case 12: storedqu_template(12, where); break;
case 13: storedqu_template(13, where); break;
case 14: storedqu_template(14, where); break;
case 15: storedqu_template(15, where); break;
}}

/**
 * Load xmm register from memory
 */
inline void _sload_xmm (const uint8_t n, const __uint128_t *where)
{
	switch (n) {
case 0:  loaddqu_template(0, where); break;
case 1:  loaddqu_template(1, where); break;
case 2:  loaddqu_template(2, where); break;
case 3:  loaddqu_template(3, where); break;
case 4:  loaddqu_template(4, where); break;
case 5:  loaddqu_template(5, where); break;
case 6:  loaddqu_template(6, where); break;
case 7:  loaddqu_template(7, where); break;
case 8:  loaddqu_template(8, where); break;
case 9:  loaddqu_template(9, where); break;
case 10: loaddqu_template(10, where); break;
case 11: loaddqu_template(11, where); break;
case 12: loaddqu_template(12, where); break;
case 13: loaddqu_template(13, where); break;
case 14: loaddqu_template(14, where); break;
case 15: loaddqu_template(15, where); break;
}}

/**
 * Store mmx register somewhere in memory
 */
inline void _sstore_mmx (const uint8_t n, uint64_t *where)
{
	switch (n) {
case 0:  storeq_template(0, where); break;
case 1:  storeq_template(1, where); break;
case 2:  storeq_template(2, where); break;
case 3:  storeq_template(3, where); break;
case 4:  storeq_template(4, where); break;
case 5:  storeq_template(5, where); break;
case 6:  storeq_template(6, where); break;
case 7:  storeq_template(7, where); break;
}}

/**
 * Load mmx register from memory
 */
inline void _sload_mmx (const uint8_t n, const uint64_t *where)
{
	switch (n) {
case 0:  loadq_template(0, where); break;
case 1:  loadq_template(1, where); break;
case 2:  loadq_template(2, where); break;
case 3:  loadq_template(3, where); break;
case 4:  loadq_template(4, where); break;
case 5:  loadq_template(5, where); break;
case 6:  loadq_template(6, where); break;
case 7:  loadq_template(7, where); break;
}}

inline void _sstore_gpr32 (ud_type_t n, uint32_t *where) {
/*
    FIXME: This function nothing do. Getting regisers value from trash pointer
           and this function write unsigned long in uint32 (unsigned int) ned
           set ifdefs in result implementation for serarate 64 and 32 mashines
           but this functions call only for 32bit and we can just cast type?
    https://elixir.bootlin.com/linux/v6.0/source/arch/x86/include/asm/ptrace.h
    ---------------------------------------------------------------------------
    struct pt_regs *regs;
	switch (n) {
		case UD_R_EAX:
			*where = &regs->ax;
			break;
		case UD_R_ECX:
			*where = &regs->cx;
			break;
		case UD_R_EDX:
			*where = &regs->dx;
			break;
		case UD_R_EBX:
			*where = &regs->bx;
			break;
		case UD_R_ESP:
			*where = &regs->sp;
			break;
		case UD_R_EBP:
			*where = &regs->bp;
			break;
		case UD_R_ESI:
			*where = &regs->si;
			break;
		case UD_R_EDI:
			*where = &regs->di;
			break;
	}
*/
}

inline void _sstore_gpr64 (ud_type_t n, uint64_t *where) {
/*
    FIXME: This function nothing do. Getting regisers value from trash pointer
           and this function write unsigned long in uint32 (unsigned int) ned
           set ifdefs in result implementation for serarate 64 and 32 mashines
           but this functions call only for 32bit and we can just cast type?
    https://elixir.bootlin.com/linux/v6.0/source/arch/x86/include/asm/ptrace.h
    ---------------------------------------------------------------------------
	struct pt_regs *regs;
	switch (n) {
		case UD_R_RAX:
			*where = &regs->ax;
			break;
		case UD_R_RCX:
			*where = &regs->cx;
			break;
		case UD_R_RDX:
			*where = &regs->dx;
			break;
		case UD_R_RBX:
			*where = &regs->bx;
			break;
		case UD_R_RSP:
			*where = &regs->sp;
			break;
		case UD_R_RBP:
			*where = &regs->bp;
			break;
		case UD_R_RSI:
			*where = &regs->si;
			break;
		case UD_R_RDI:
			*where = &regs->di;
			break;
	}
*/
}

inline void _fstore_xmm (const uint8_t n, float *where)
{
	switch (n) {
case 0:  fstored_template(0, where); break;
case 1:  fstored_template(1, where); break;
case 2:  fstored_template(2, where); break;
case 3:  fstored_template(3, where); break;
case 4:  fstored_template(4, where); break;
case 5:  fstored_template(5, where); break;
case 6:  fstored_template(6, where); break;
case 7:  fstored_template(7, where); break;
case 8:  fstored_template(8, where); break;
case 9:  fstored_template(9, where); break;
case 10: fstored_template(10, where); break;
case 11: fstored_template(11, where); break;
case 12: fstored_template(12, where); break;
case 13: fstored_template(13, where); break;
case 14: fstored_template(14, where); break;
case 15: fstored_template(15, where); break;
}}

inline void _fload_xmm (const uint8_t n, const float *where)
{
	switch (n) {
case 0:  floadd_template(0, where); break;
case 1:  floadd_template(1, where); break;
case 2:  floadd_template(2, where); break;
case 3:  floadd_template(3, where); break;
case 4:  floadd_template(4, where); break;
case 5:  floadd_template(5, where); break;
case 6:  floadd_template(6, where); break;
case 7:  floadd_template(7, where); break;
case 8:  floadd_template(8, where); break;
case 9:  floadd_template(9, where); break;
case 10: floadd_template(10, where); break;
case 11: floadd_template(11, where); break;
case 12: floadd_template(12, where); break;
case 13: floadd_template(13, where); break;
case 14: floadd_template(14, where); break;
case 15: floadd_template(15, where); break;
}}

/**
 * Load operands from memory/register, store in obj.
 * @return: 0 if success
 */

int ssse3_grab_operands(ssse3_t *ssse3_obj)
{
	if (ssse3_obj->udo_dst->type == UD_OP_REG) {
		if (ssse3_obj->ismmx) {
			_sstore_mmx (ssse3_obj->udo_dst->base - UD_R_MM0, &ssse3_obj->dst.uint64[0]);
		}
		else if (ssse3_obj->udo_dst->base >= UD_R_EAX && ssse3_obj->udo_dst->base <= UD_R_R15D) {
			_sstore_gpr32 (ssse3_obj->udo_dst->base, &ssse3_obj->dst.uint32[0]);
		}
		else if (ssse3_obj->udo_dst->base >= UD_R_RAX && ssse3_obj->udo_dst->base <= UD_R_R15) {
			_sstore_gpr64 (ssse3_obj->udo_dst->base, &ssse3_obj->dst.uint64[0]);
		}
		else {
			_sstore_xmm (ssse3_obj->udo_dst->base - UD_R_XMM0, &ssse3_obj->dst.uint128);
		}
	} else {
		printk("mem");
	}

	if (ssse3_obj->udo_src->type == UD_OP_REG) {
		if (ud_insn_mnemonic(ssse3_obj->op_obj->ud_obj) == UD_Iroundss) {
			_fstore_xmm (ssse3_obj->udo_src->base - UD_R_XMM0, &ssse3_obj->src.fa32[0]);
		}
		else if (ssse3_obj->udo_src->base >= UD_R_MM0 && ssse3_obj->udo_src->base <= UD_R_MM7) {
			_sstore_mmx (ssse3_obj->udo_src->base - UD_R_MM0, &ssse3_obj->src.uint64[0]);
		}
		else if (ssse3_obj->udo_src->base >= UD_R_EAX && ssse3_obj->udo_src->base <= UD_R_R15D) {
		printk("eee %s", ud_insn_asm(ssse3_obj->op_obj->ud_obj));
			_sstore_gpr32 (ssse3_obj->udo_src->base, &ssse3_obj->src.uint32[0]);
			printk("ssse3_obj->src.uint32[0]: %u", ssse3_obj->src.uint32[0]);
		}
		else if (ssse3_obj->udo_src->base >= UD_R_RAX && ssse3_obj->udo_src->base <= UD_R_R15) {
			_sstore_gpr64 (ssse3_obj->udo_src->base, &ssse3_obj->src.uint64[0]);
		}
		else {
			_sstore_xmm (ssse3_obj->udo_src->base - UD_R_XMM0, &ssse3_obj->src.uint128);
		}
	} else {
			printk("src mem");
		if (ssse3_obj->udo_src->size == 32) {
			printk("src mem32");
		}
		else if (ssse3_obj->udo_src->size == 64) {
			// m64 load
			int64_t disp = 0;
			uint8_t disp_size = ssse3_obj->udo_src->offset;
			uint64_t address;

			if (ssse3_obj->udo_src->scale) goto bad; // TODO

			if (retrieve_reg (ssse3_obj->op_obj->state,
				ssse3_obj->udo_src->base, NULL, &address) != 0) goto bad;

			switch (disp_size) {
			case 8: disp = ssse3_obj->udo_src->lval.sbyte; break;
			case 16: disp = ssse3_obj->udo_src->lval.sword; break;
			case 32: disp = ssse3_obj->udo_src->lval.sdword; break;
			case 64: disp = ssse3_obj->udo_src->lval.sqword; break;
			}

			address += disp;

			if (ssse3_obj->op_obj->ring0)
				ssse3_obj->src.uint64[0] = * ((uint64_t*) (address));
			else copy_from_user((char*) &ssse3_obj->src.uint64[0], address, 8);
		}
		else if (ssse3_obj->udo_src->size == 128) {
			// m128 load
			int64_t disp = 0;
			uint8_t disp_size = ssse3_obj->udo_src->offset;
			uint64_t address;

			if (ssse3_obj->udo_src->scale) goto bad; // TODO

			if (retrieve_reg (ssse3_obj->op_obj->state,
				ssse3_obj->udo_src->base, NULL, &address) != 0) goto bad;

			switch (disp_size) {
			case 8: disp = ssse3_obj->udo_src->lval.sbyte; break;
			case 16: disp = ssse3_obj->udo_src->lval.sword; break;
			case 32: disp = ssse3_obj->udo_src->lval.sdword; break;
			case 64: disp = ssse3_obj->udo_src->lval.sqword; break;
			}

			address += disp;

			if (ssse3_obj->op_obj->ring0)
				ssse3_obj->src.uint128 = * ((__uint128_t*) (address));
			else copy_from_user((char*) &ssse3_obj->src.uint128, address, 16);
		}
		else {
			printk("src mem else");
		}
	}

    return 0;
    // Only reached if bad
bad:	return -1;
}

/**
 * Store operands from obj to memory/register.
 * @return: 0 if success
 */
int ssse3_commit_results(ssse3_t *ssse3_obj)
{
	if (ud_insn_mnemonic(ssse3_obj->op_obj->ud_obj) == UD_Iroundss) {

		_fload_xmm (ssse3_obj->udo_dst->base - UD_R_XMM0, (void*) &ssse3_obj->res.fa32[0]);
	}
	else if (ssse3_obj->ismmx) {
		_sload_mmx (ssse3_obj->udo_dst->base - UD_R_MM0, (void*) &ssse3_obj->res.uint64[0]);
	}
	else if (ssse3_obj->udo_dst->type == UD_OP_REG && ssse3_obj->udo_dst->base >= UD_R_RAX && ssse3_obj->udo_dst->base <= UD_R_R15) {
		//printk("dst64");
		ssse3_obj->dst64 = 1;
		return 0;
	}
	else if (ssse3_obj->udo_dst->type == UD_OP_REG && ssse3_obj->udo_dst->base >= UD_R_EAX && ssse3_obj->udo_dst->base <= UD_R_R15D) {
		ssse3_obj->dst32 = 1;
		return 0;
	}
	else if (ssse3_obj->udo_dst->type == UD_OP_MEM) {
		printk("base: %u size: %u", ssse3_obj->udo_dst->base, ssse3_obj->udo_dst->size);
	}
	else {
		_sload_xmm (ssse3_obj->udo_dst->base - UD_R_XMM0, (void*) &ssse3_obj->res.uint128);
	}

    return 0;

    // Only reached if bad
//bad:	return -1;
}


/**
 * Main function for the ssse3 portion. Check if the offending
 * opcode is part of the ssse3 instruction set, if not, quit early.
 * if so, then we build the appropriate context, and jump to the right function.
 * @param op_obj: opemu object
 * @return: zero if an instruction was emulated properly
 */
int op_sse3x_run(op_t *op_obj)
{
	ssse3_t ssse3_obj;
	ssse3_obj.op_obj = op_obj;
	const uint32_t mnemonic = ud_insn_mnemonic(ssse3_obj.op_obj->ud_obj);
	ssse3_func opf;
	switch (mnemonic) {
    case UD_Ipcmpestri:	opf = pcmpestri; goto sse42_common;
	case UD_Ipcmpestrm:	opf = pcmpestrm; goto sse42_common;
	case UD_Ipcmpistri:	opf = pcmpistri; goto sse42_common;
    case UD_Ipcmpistrm: opf = pcmpistrm; goto sse42_common;
    case UD_Ipcmpgtq:	opf = pcmpgtq;   goto sse42_common;
    case UD_Ipopcnt:    opf = popcnt;    goto regop;
    case UD_Icrc32:     opf = crc32_op;  goto regop;

    //SSE 4.1
    case UD_Ipmaxud: opf = pmaxud;	goto ssse3_common;
    case UD_Ipminud: opf = pminud;	goto ssse3_common;
    //case UD_Iblendpd: opf = blendpd;	goto ssse3_common;
    //case UD_Iblendps: opf = blendps;	goto ssse3_common;
    //case UD_Ipblendw: opf = pblendw;	goto ssse3_common;
    //case UD_Ipmovsxbd: opf = pmovsxbd;	goto ssse3_common;
    //case UD_Ipmovsxbq: opf = pmovsxbq;	goto ssse3_common;
    //case UD_Ipmovsxbw: opf = pmovsxbw;	goto ssse3_common;
    //case UD_Ipmovsxdq: opf = pmovsxdq;	goto ssse3_common;
    //case UD_Ipmovsxwd: opf = pmovsxwd;	goto ssse3_common;
    //case UD_Ipmovsxwq: opf = pmovsxwq;	goto ssse3_common;
    //case UD_Ipmovzxbd: opf = pmovzxbd;	goto ssse3_common;
    //case UD_Ipmovzxbq: opf = pmovzxbq;	goto ssse3_common;
    //case UD_Ipmovzxbw: opf = pmovzxbw;	goto ssse3_common;
    //case UD_Ipmovzxdq: opf = pmovzxdq;	goto ssse3_common;
    //case UD_Ipmovzxwd: opf = pmovzxwd;	goto ssse3_common;
    //case UD_Ipmovzxwq: opf = pmovzxwq;	goto ssse3_common;
    //
    case UD_Iroundss: opf = roundss;	goto ssse3_common;
    case UD_Ipextrb: opf = pextrb;	goto ssse3_common;
    case UD_Ipextrd: opf = pextrd;	goto ssse3_common;
    case UD_Ipextrq: opf = pextrq;	goto ssse3_common;
    case UD_Iptest: opf = ptest;	goto ssse3_common;
    case UD_Ipinsrb: opf = pinsrb;	goto ssse3_common;
    case UD_Ipinsrd: opf = pinsrd;	goto ssse3_common;
    case UD_Ipinsrq: opf = pinsrq;	goto ssse3_common;

sse42_common:

	goto ssse3_common;


	case UD_Ipsignb:	opf = psignb;	goto ssse3_common;
	case UD_Ipsignw:	opf = psignw;	goto ssse3_common;
	case UD_Ipsignd:	opf = psignd;	goto ssse3_common;

	case UD_Ipabsb:		opf = pabsb;	goto ssse3_common;
	case UD_Ipabsw:		opf = pabsw;	goto ssse3_common;
	case UD_Ipabsd:		opf = pabsd;	goto ssse3_common;
	case UD_Ipalignr:	opf = palignr;	goto ssse3_common;
	case UD_Ipshufb:	opf = pshufb;	goto ssse3_common;

	case UD_Ipmulhrsw:	opf = pmulhrsw;	goto ssse3_common;

	case UD_Ipmaddubsw:	opf = pmaddubsw;	goto ssse3_common;

	case UD_Iphsubw:	opf = phsubw;	goto ssse3_common;
	case UD_Iphsubd:	opf = phsubd;	goto ssse3_common;

	case UD_Iphsubsw:	opf = phsubsw;	goto ssse3_common;

	case UD_Iphaddw:	opf = phaddw;	goto ssse3_common;
	case UD_Iphaddd:	opf = phaddd;	goto ssse3_common;

	case UD_Iphaddsw:	opf = phaddsw;	goto ssse3_common;
ssse3_common:

	ssse3_obj.udo_src = ud_insn_opr (op_obj->ud_obj, 1);
	ssse3_obj.udo_dst = ud_insn_opr (op_obj->ud_obj, 0);
	ssse3_obj.udo_imm = ud_insn_opr (op_obj->ud_obj, 2);
	// run some sanity checks,
	if ((ssse3_obj.udo_dst->type != UD_OP_REG) && (ssse3_obj.udo_dst->type != UD_OP_MEM)) goto bad;
	if ((ssse3_obj.udo_src->type != UD_OP_REG) && (ssse3_obj.udo_src->type != UD_OP_MEM)) goto bad;

	// i'd like to know if this instruction is legacy mmx
	if ((ssse3_obj.udo_dst->base >= UD_R_MM0) && (ssse3_obj.udo_dst->base <= UD_R_MM7)) {
		ssse3_obj.ismmx = 1;
	} else ssse3_obj.ismmx = 0;

	ssse3_obj.dst64 = ssse3_obj.dst32 = 0;

	if (ssse3_grab_operands(&ssse3_obj) != 0) goto bad;
	opf(&ssse3_obj);
	if (ssse3_commit_results(&ssse3_obj)) goto bad;

	goto good;

	default: goto bad;
	}

    // Regular (non-SSE regs) handling
regop:
    ssse3_obj.udo_src = ud_insn_opr (op_obj->ud_obj, 1);
	ssse3_obj.udo_dst = (ud_operand_t *)ud_insn_opr (op_obj->ud_obj, 0);
	ssse3_obj.udo_imm = ud_insn_opr (op_obj->ud_obj, 2);

    opf(&ssse3_obj);

good:
	if (ssse3_obj.dst64) {

		//printk("OPEMUq:  %s\n", ud_insn_asm(op_obj->ud_obj));
		op_obj->dst64 = (uint8_t) 1;
		op_obj->res64 = (uint64_t) ssse3_obj.res.uint64[0];
	}
	if (ssse3_obj.dst32) {
		op_obj->dst32 = (uint8_t) 1;
		op_obj->res32 = (uint32_t) ssse3_obj.res.uint32[0];
	}


	//uint64_t ek;
	//asm __volatile__ ("movq %%rcx, %0" : "=m" (ek) :);
	//printk("good rcx: %u", ek);
	return 0;

    // Only reached if bad
bad:
	return -1;
}

#define SATSW(x) ((x > 32767)? 32767 : ((x < -32768)? -32768 : x) )

/**
 * Negate/zero/preserve
 */
void psignb (ssse3_t *this)
{
	const int8_t *src = &this->src.int8[0];
	const int8_t *dst = &this->dst.int8[0];
	int8_t *res = &this->res.int8[0];

	int count = (this->ismmx) ? 8 : 16;
	int i;
	for (i = 0; i < count; ++ i) {
		if (*src < 0) *res = - *dst;
		else if (*src == 0) *res = 0;
		else if (*src > 0) *res = *dst;

		++res; ++src; ++dst;
	}
}

/**
 * Negate/zero/preserve
 */
void psignw (ssse3_t *this)
{
	const int16_t *src = &this->src.int16[0];
	const int16_t *dst = &this->dst.int16[0];
	int16_t *res = &this->res.int16[0];

	int count = (this->ismmx) ? 4 : 8;
	int i;
	for (i = 0; i < count; ++ i) {
		if (*src < 0) *res = - *dst;
		else if (*src == 0) *res = 0;
		else if (*src > 0) *res = *dst;

		++res; ++src; ++dst;
	}
}

/**
 * Negate/zero/preserve
 */
void psignd (ssse3_t *this)
{
	const int32_t *src = &this->src.int32[0];
	const int32_t *dst = &this->dst.int32[0];
	int32_t *res = &this->res.int32[0];

	int count = (this->ismmx) ? 2 : 4;
	int i;
	for (i = 0; i < count; ++ i) {
		if (*src < 0) *res = - *dst;
		else if (*src == 0) *res = 0;
		else if (*src > 0) *res = *dst;

		++res; ++src; ++dst;
	}
}

/**
 * Absolute value
 */
void pabsb (ssse3_t *this)
{
	const int8_t *src = &this->src.int8[0];
	const int8_t *dst = &this->dst.int8[0];
	int8_t *res = &this->res.int8[0];

	int count = (this->ismmx) ? 8 : 16;
	int i;
	for (i = 0; i < count; ++ i) {
		if (*src < 0) *res = - *src;
		else *res = *src;

		++res; ++src; ++dst;
	}
}

/**
 * Absolute value
 */
void pabsw (ssse3_t *this)
{
	const int16_t *src = &this->src.int16[0];
	const int16_t *dst = &this->dst.int16[0];
	int16_t *res = &this->res.int16[0];

	int count = (this->ismmx) ? 4 : 8;
	int i;
	for (i = 0; i < count; ++ i) {
		if (*src < 0) *res = - *src;
		else *res = *src;

		++res; ++src; ++dst;
	}
}

/**
 * Absolute value
 */
void pabsd (ssse3_t *this)
{
	const int32_t *src = &this->src.int32[0];
	const int32_t *dst = &this->dst.int32[0];
	int32_t *res = &this->res.int32[0];

	int count = (this->ismmx) ? 2 : 4;
	int i;
	for (i = 0; i < count; ++ i) {
		if (*src < 0) *res = - *src;
		else *res = *src;

		++res; ++src; ++dst;
	}
}

/**
 * Concatenate and shift
 */
void palignr (ssse3_t *this)
{
	uint8_t imm = this->udo_imm->lval.ubyte;

	if (this->ismmx) {
		__uint128_t temp1 = this->dst.uint64[0];
		temp1 <<= 64;
		temp1 |= this->src.uint64[0];
		temp1 >>= (imm * 8);
		this->res.uint128 = temp1;
	} else {
        // AnV - Cast fixed
		__uint128_t temp1[2];
		uint8_t *shiftp; // that type matters for pointer arithmetic
        uint64_t shiftpaddr;
		temp1[0] = this->src.uint128;
		temp1[1] = this->dst.uint128;
		shiftp = (uint8_t*) &temp1[0];
		shiftp += imm;
        shiftpaddr = (uint64_t)shiftp;
		this->res.uint128 = ((__uint128_t*) shiftpaddr);
	}
}

/**
 * Shuffle Bytes
 */
void pshufb (ssse3_t *this)
{
	const uint8_t *src = &this->src.uint8[0];
	uint8_t *res = &this->res.uint8[0];

	int count = (this->ismmx) ? 8 : 16;
	uint8_t mask = (this->ismmx) ? 0b0111 : 0b1111;
	int i;
	for (i = 0; i < count; ++ i) {
		if (*src & 0x80) *res = 0;
		else *res = this->dst.uint8[ *src & mask ];

		++res; ++src;
	}
}

/**
 * Multiply high with round and scale
 */
void pmulhrsw (ssse3_t *this)
{
	const int16_t *src = &this->src.int16[0];
	const int16_t *dst = &this->dst.int16[0];
	int16_t *res = &this->res.int16[0];

	int count = (this->ismmx) ? 4 : 8;
	int i;
	for (i = 0; i < count; ++i) {
		int32_t temp1 = (*dst) * (*src);
		temp1 >>= 14;
		temp1++;
		temp1 >>= 1;
		*res = temp1 & 0xFFFF;

		++res; ++src; ++dst;
	}
}

/**
 * Multiply and add
 */
void pmaddubsw (ssse3_t *this)
{
	const int8_t *src = &this->src.int8[0];
	const uint8_t *dst = &this->dst.uint8[0];
	int16_t *res = &this->res.int16[0];

	int count = (this->ismmx) ? 4 : 8;
	int i;
	for (i = 0; i < count; ++i) {
		int64_t temp1 = (src[0] * dst[0]) + (src[1] * dst[1]);
		*res = SATSW(temp1);

		++res;
		src += 2;
		dst += 2;
	}
}

/**
 * Horizontal subtract
 */
void phsubw (ssse3_t *this)
{
	const int16_t *src = &this->src.int16[0];
	const int16_t *dst = &this->dst.int16[0];
	int16_t *res = &this->res.int16[0];

	int count = (this->ismmx) ? 2 : 4;
	int i;
	for (i = 0; i < count; ++ i) {
		*res = (dst[0]) - (dst[1]);

		++res;
		dst += 2;
	}
	for (i = 0; i < count; ++ i) {
		*res = (src[0]) - (src[1]);

		++res;
		src += 2;
	}
}

/**
 * Horizontal subtract
 */
void phsubd (ssse3_t *this)
{
	const int32_t *src = &this->src.int32[0];
	const int32_t *dst = &this->dst.int32[0];
	int32_t *res = &this->res.int32[0];

	int count = (this->ismmx) ? 1 : 2;
	int i;
	for (i = 0; i < count; ++ i) {
		*res = (dst[0]) - (dst[1]);

		++res;
		dst += 2;
	}
	for (i = 0; i < count; ++ i) {
		*res = (src[0]) - (src[1]);

		++res;
		src += 2;
	}
}

/**
 * Horizontal subtract and saturate
 */
void phsubsw (ssse3_t *this)
{
	const int16_t *src = &this->src.int16[0];
	const int16_t *dst = &this->dst.int16[0];
	int16_t *res = &this->res.int16[0];

	int count = (this->ismmx) ? 2 : 4;
	int i;
	for (i = 0; i < count; ++ i) {
		*res = SATSW((dst[0]) - (dst[1]));

		++res;
		dst += 2;
	}
	for (i = 0; i < count; ++ i) {
		*res = SATSW((src[0]) - (src[1]));

		++res;
		src += 2;
	}
}

/**
 * Horizontal add
 */
void phaddw (ssse3_t *this)
{
	const int16_t *src = &this->src.int16[0];
	const int16_t *dst = &this->dst.int16[0];
	int16_t *res = &this->res.int16[0];

	int count = (this->ismmx) ? 2 : 4;
	int i;
	for (i = 0; i < count; ++ i) {
		*res = dst[0] + dst[1];

		++res;
		dst += 2;
	}
	for (i = 0; i < count; ++ i) {
		*res = src[0] + src[1];

		++res;
		src += 2;
	}
}

/**
 * Horizontal add
 */
void phaddd (ssse3_t *this)
{
	const int32_t *src = &this->src.int32[0];
	const int32_t *dst = &this->dst.int32[0];
	int32_t *res = &this->res.int32[0];

	int count = (this->ismmx) ? 1 : 2;
	int i;
	for (i = 0; i < count; ++ i) {
		*res = dst[0] + dst[1];

		++res;
		dst += 2;
	}
	for (i = 0; i < count; ++ i) {
		*res = src[0] + src[1];

		++res;
		src += 2;
	}
}

/**
 * Horizontal add and saturate
 */
void phaddsw (ssse3_t *this)
{
	const int16_t *src = &this->src.int16[0];
	const int16_t *dst = &this->dst.int16[0];
	int16_t *res = &this->res.int16[0];

	int count = (this->ismmx) ? 2 : 4;
	int i;
	for (i = 0; i < count; ++ i) {
		*res = SATSW(dst[0] + dst[1]);

		++res;
		dst += 2;
	}
	for (i = 0; i < count; ++ i) {
		*res = SATSW(src[0] + src[1]);

		++res;
		src += 2;
	}
}

