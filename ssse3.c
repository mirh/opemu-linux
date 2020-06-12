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

	ssse3_obj.noxmmstoreres = 0;

	switch (mnemonic) {
    case UD_Ipcmpestri:	opf = pcmpestri; goto sse42_common;
	case UD_Ipcmpestrm:	opf = pcmpestrm; goto sse42_common;
	case UD_Ipcmpistri:	opf = pcmpistri; goto sse42_common;
    case UD_Ipcmpistrm: opf = pcmpistrm; goto sse42_common;
    case UD_Ipcmpgtq:	opf = pcmpgtq;   goto sse42_common;
    case UD_Ipopcnt:    opf = popcnt;    goto regop;
    case UD_Icrc32:     opf = crc32_op;  goto regop;
    
    case UD_Iblendpd: opf = blendpd; goto sse41_common;
    case UD_Iblendps: opf = blendps; goto sse41_common;
    case UD_Iblendvpd: opf = blendvpd; goto sse41_common;
    case UD_Idppd: opf = dppd; goto sse41_common;
    case UD_Idpps: opf = dpps; goto sse41_common;
    case UD_Iextractps: opf = extractps; ssse3_obj.noxmmstoreres = 1; goto sse41_common;
    case UD_Iinsertps: opf = insertps; goto sse41_common;
    //case UD_Imovntdqa: opf = movntdqa; goto sse41_common;
    case UD_Impsadbw: opf = mpsadbw; goto sse41_common;
    case UD_Ipackusdw: opf = packusdw; goto sse41_common;
    case UD_Ipblendvb: opf = pblendvb; goto sse41_common;
    case UD_Ipblendw: opf = pblendw; goto sse41_common;
    case UD_Ipcmpeqq: opf = pcmpeqq; goto sse41_common;
    //case UD_Ipextrb: opf = pextrb; goto sse41_common;
    //case UD_Ipextrd: opf = pextrd; goto sse41_common;
    //case UD_Ipextrq: opf = pextrq; goto sse41_common;
    //case UD_Ipextrw: opf = pextrw; goto sse41_common;
    case UD_Iphminposuw: opf = phminposuw; goto sse41_common;
    case UD_Ipinsrb: opf = pinsrb; goto sse41_common;
    case UD_Ipinsrd: opf = pinsrd; goto sse41_common;
    case UD_Ipinsrq: opf = pinsrq; goto sse41_common;
    case UD_Ipmaxsb: opf = pmaxsb; goto sse41_common;
    case UD_Ipmaxsd: opf = pmaxsd; goto sse41_common;
    case UD_Ipmaxud: opf = pmaxud; goto sse41_common;
    case UD_Ipmaxuw: opf = pmaxuw; goto sse41_common;
    case UD_Ipminsb: opf = pminsb; goto sse41_common;
    case UD_Ipminsd: opf = pminsd; goto sse41_common;
    case UD_Ipminud: opf = pminud; goto sse41_common;
    case UD_Ipminuw: opf = pminuw; goto sse41_common;
    case UD_Ipmovsxbd: opf = pmovsxbd; goto sse41_common;
    case UD_Ipmovsxbq: opf = pmovsxbq; goto sse41_common;
    case UD_Ipmovsxbw: opf = pmovsxbw; goto sse41_common;
    case UD_Ipmovsxwd: opf = pmovsxwd; goto sse41_common;
    case UD_Ipmovsxwq: opf = pmovsxwq; goto sse41_common;
    case UD_Ipmovsxdq: opf = pmovsxdq; goto sse41_common;
    case UD_Ipmovzxbd: opf = pmovzxbd; goto sse41_common;
    case UD_Ipmovzxbq: opf = pmovzxbq; goto sse41_common;
    case UD_Ipmovzxbw: opf = pmovzxbw; goto sse41_common;
    case UD_Ipmovzxwd: opf = pmovzxwd; goto sse41_common;
    case UD_Ipmovzxwq: opf = pmovzxwq; goto sse41_common;
    case UD_Ipmovzxdq: opf = pmovzxdq; goto sse41_common;
    case UD_Ipmuldq: opf = pmuldq; goto sse41_common;
    case UD_Ipmulld: opf = pmulld; goto sse41_common;
    case UD_Iptest: opf = ptest; goto sse41_common;
    case UD_Iroundpd: opf = roundpd; goto sse41_common;
    case UD_Iroundps: opf = roundps; goto sse41_common;
    case UD_Iroundsd: opf = roundsd; goto sse41_common;
    case UD_Iroundss: opf = roundss; goto sse41_common;

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

sse41_common:
sse42_common:
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
	return 0;

    // Only reached if bad
bad:
	return -1;
}

/**
 * Negate/zero/preserve
 */
void psignb (ssse3_t *this)
{
    int count = (this->ismmx) ? 8 : 16;

    if (count == 16)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_sign_epi8_REF(*dst, *src);
    } else if (count == 8) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_sign_pi8_REF(*dst, *src);
    }
}

/**
 * Negate/zero/preserve
 */
void psignw (ssse3_t *this)
{
    int count = (this->ismmx) ? 4 : 8;

    if (count == 8)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_sign_epi16_REF(*dst, *src);
    } else if (count == 4) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_sign_pi16_REF(*dst, *src);
    }
}

/**
 * Negate/zero/preserve
 */
void psignd (ssse3_t *this)
{
    int count = (this->ismmx) ? 2 : 4;

    if (count == 4)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_sign_epi32_REF(*dst, *src);
    } else if (count == 2) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_sign_pi32_REF(*dst, *src);
    }
}

/**
 * Absolute value
 */
void pabsb (ssse3_t *this)
{
    int count = (this->ismmx) ? 8 : 16;

    if (count == 8)
    {
        __m128i *src = &this->src.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_abs_epi8_REF(*src);
    } else if (count == 4) {
        __m64 *src = &this->src.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_abs_pi8_REF(*src);
    }
}

/**
 * Absolute value
 */
void pabsw (ssse3_t *this)
{
    int count = (this->ismmx) ? 4 : 8;

    if (count == 8)
    {
        __m128i *src = &this->src.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_abs_epi16_REF(*src);
    } else if (count == 4) {
        __m64 *src = &this->src.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_abs_pi16_REF(*src);
    }
}

/**
 * Absolute value
 */
void pabsd (ssse3_t *this)
{
    int count = (this->ismmx) ? 2 : 4;

    if (count == 4)
    {
        __m128i *src = &this->src.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_abs_epi32_REF(*src);
    } else if (count == 2) {
        __m64 *src = &this->src.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_abs_pi32_REF(*src);
    }
}

/**
 * Concatenate and shift
 */ 
void palignr (ssse3_t *this)
{
	const int imm = (const int)(this->udo_imm->lval.sbyte);

	if (this->ismmx) {
        __m64 *src = &this->dst.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_alignr_pi8_REF(*dst, *src, imm);
	} else {
        __m128i *src = &this->dst.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_alignr_epi8_REF(*dst, *src, imm);
    }
}

/**
 * Shuffle Bytes
 */
void pshufb (ssse3_t *this)
{
    int count = (this->ismmx) ? 8 : 16;

    if (count == 16)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_shuffle_epi8_REF(*dst, *src);
    } else if (count == 8) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_shuffle_pi8_REF(*dst, *src);
    }
}

/**
 * Multiply high with round and scale
 */
void pmulhrsw (ssse3_t *this)
{
    int count = (this->ismmx) ? 4 : 8;

    if (count == 8)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_mulhrs_epi16_REF(*dst, *src);
    } else if (count == 4) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_mulhrs_pi16_REF(*dst, *src);
    }
}

/**
 * Multiply and add
 */
void pmaddubsw (ssse3_t *this)
{
    int count = (this->ismmx) ? 4 : 8;

    if (count == 8)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_maddubs_epi16_REF(*dst, *src);
    } else if (count == 4) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_maddubs_pi16_REF(*dst, *src);
    }
}

/**
 * Horizontal subtract
 */
void phsubw (ssse3_t *this)
{
    int count = (this->ismmx) ? 2 : 4;

    if (count == 4)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_hsub_epi16_REF(*dst, *src);
    } else if (count == 2) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_hsub_pi16_REF(*dst, *src);
    }
}

/**
 * Horizontal subtract
 */
void phsubd (ssse3_t *this)
{
    int count = (this->ismmx) ? 1 : 2;

    if (count == 2)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_hsub_epi32_REF(*dst, *src);
    } else if (count == 1) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_hsub_pi32_REF(*dst, *src);
    }
}

/**
 * Horizontal subtract and saturate
 */
void phsubsw (ssse3_t *this)
{
    int count = (this->ismmx) ? 2 : 4;

    if (count == 4)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_hsubs_epi16_REF(*dst, *src);
    } else if (count == 2) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_hsubs_pi16_REF(*dst, *src);
    }
}

/**
 * Horizontal add
 */
void phaddw (ssse3_t *this)
{
    int count = (this->ismmx) ? 2 : 4;

    if (count == 4)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_hadd_epi16_REF(*dst, *src);
    } else if (count == 2) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_hadd_pi16_REF(*dst, *src);
    }
}

/**
 * Horizontal add
 */
void phaddd (ssse3_t *this)
{
    int count = (this->ismmx) ? 1 : 2;

    if (count == 2)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_hadd_epi32_REF(*dst, *src);
    } else if (count == 1) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->res.m64[0];

        *res = ssp_hadd_pi32_REF(*dst, *src);
    }
}

/**
 * Horizontal add and saturate
 */
void phaddsw (ssse3_t *this)
{
    int count = (this->ismmx) ? 2 : 4;

    if (count == 4)
    {
        __m128i *src = &this->src.m128i;
        __m128i *dst = &this->dst.m128i;
        __m128i *res = &this->res.m128i;

        *res = ssp_hadds_epi16_REF(*dst, *src);
    } else if (count == 2) {
        __m64 *src = &this->src.m64[0];
        __m64 *dst = &this->dst.m64[0];
        __m64 *res = &this->dst.m64[0];

        *res = ssp_hadds_pi16_REF(*dst, *src);
    }
}
