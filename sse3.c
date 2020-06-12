/*
             .d8888b.   .d8888b.  8888888888 .d8888b.
            d88P  Y88b d88P  Y88b 888       d88P  Y88b
            Y88b.      Y88b.      888            .d88P
             "Y888b.    "Y888b.   8888888       8888"
                "Y88b.     "Y88b. 888            "Y8b.
                  "888       "888 888       888    888
            Y88b  d88P Y88b  d88P 888       Y88b  d88P
             "Y8888P"   "Y8888P"  8888888888 "Y8888P"
*/
#include <linux/uaccess.h>
#include "opemu.h"
#include "opemu_math.h"
#include "sse3_priv.h"

/**
 * Load operands from memory/register, store in obj.
 * @return: 0 if success
 */
int sse3_grab_operands(sse3_t *sse3_obj)
{
	if (sse3_obj->ismmx) {
		_store_mmx (sse3_obj->udo_dst->base - UD_R_MM0, &sse3_obj->dst.uint64[0]);
		if (sse3_obj->udo_src->type == UD_OP_REG) {
			_store_mmx (sse3_obj->udo_src->base - UD_R_MM0, &sse3_obj->src.uint64[0]);
		} else {
			// m64 load
			int64_t disp = 0;
			uint8_t disp_size = sse3_obj->udo_src->offset;
			uint64_t address;
			
			if (sse3_obj->udo_src->scale) goto bad; // TODO

			if (retrieve_reg (sse3_obj->op_obj->state,
				sse3_obj->udo_src->base, NULL, &address) != 0) goto bad;

			switch (disp_size) {
			case 8: disp = sse3_obj->udo_src->lval.sbyte; break;
			case 16: disp = sse3_obj->udo_src->lval.sword; break;
			case 32: disp = sse3_obj->udo_src->lval.sdword; break;
			case 64: disp = sse3_obj->udo_src->lval.sqword; break;
			}

			address += disp;

			if (sse3_obj->op_obj->ring0)
				sse3_obj->src.uint64[0] = * ((uint64_t*) (address));
			else copy_from_user((char*) &sse3_obj->src.uint64[0], address, 8);
		}
	} else {
		_store_xmm (sse3_obj->udo_dst->base - UD_R_XMM0, &sse3_obj->dst.uint128);
		if (sse3_obj->udo_src->type == UD_OP_REG) {
			_store_xmm (sse3_obj->udo_src->base - UD_R_XMM0, &sse3_obj->src.uint128);
		} else {
			// m128 load
			int64_t disp = 0;
			uint8_t disp_size = sse3_obj->udo_src->offset;
			uint64_t address;
			
			if (sse3_obj->udo_src->scale) goto bad; // TODO

			if (retrieve_reg (sse3_obj->op_obj->state,
				sse3_obj->udo_src->base, NULL, &address) != 0) goto bad;

			switch (disp_size) {
			case 8: disp = sse3_obj->udo_src->lval.sbyte; break;
			case 16: disp = sse3_obj->udo_src->lval.sword; break;
			case 32: disp = sse3_obj->udo_src->lval.sdword; break;
			case 64: disp = sse3_obj->udo_src->lval.sqword; break;
			}

			address += disp;

			if (sse3_obj->op_obj->ring0)
				sse3_obj->src.uint128 = * ((__uint128_t*) (address));
			else copy_from_user((char*) &sse3_obj->src.uint128, address, 16);
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
int sse3_commit_results(const sse3_t *sse3_obj)
{
	if (sse3_obj->ismmx) {
		_load_mmx (sse3_obj->udo_dst->base - UD_R_MM0, (void*) &sse3_obj->res.uint64[0]);
	} else {
		_load_xmm (sse3_obj->udo_dst->base - UD_R_XMM0, (void*) &sse3_obj->res.uint128);
	}

    return 0;

    // Only reached if bad
//bad:	return -1;
}


/**
 * Main function for the sse3 portion. Check if the offending
 * opcode is part of the sse3 instruction set, if not, quit early.
 * if so, then we build the appropriate context, and jump to the right function.
 * @param op_obj: opemu object
 * @return: zero if an instruction was emulated properly
 */
int op_sse3_run(const op_t *op_obj)
{
	sse3_t sse3_obj;
	sse3_obj.op_obj = op_obj;
	const uint32_t mnemonic = ud_insn_mnemonic(sse3_obj.op_obj->ud_obj);
	sse3_func opf;

    sse3_obj.noxmmstoreres = 0;

	switch (mnemonic) {
	case UD_Iaddsubpd:	opf = addsubpd;	goto sse3_common;
	case UD_Iaddsubps:	opf = addsubps;	goto sse3_common;
    case UD_Ihaddpd:    opf = haddpd; goto sse3_common;
    case UD_Ihaddps:    opf = haddpd; goto sse3_common;
    case UD_Ihsubpd:    opf = hsubpd; goto sse3_common;
    case UD_Ihsubps:    opf = hsubps; goto sse3_common;
    case UD_Ilddqu:     opf = lddqu; goto sse3_common;
    case UD_Imovddup:   opf = movddup; goto sse3_common;
    case UD_Imovshdup:  opf = movshdup; goto sse3_common;
    case UD_Imovsldup:  opf = movsldup; goto sse3_common;
    case UD_Ifisttp:    opf = fisttp; sse3_obj.noxmmstoreres=1; goto good;
    case UD_Imwait:     goto good;
    case UD_Imonitor:   goto good;

sse3_common:
	sse3_obj.udo_src = ud_insn_opr (op_obj->ud_obj, 1);
	sse3_obj.udo_dst = ud_insn_opr (op_obj->ud_obj, 0);
	sse3_obj.udo_imm = ud_insn_opr (op_obj->ud_obj, 2);

	// run some sanity checks,
	if ((sse3_obj.udo_dst->type != UD_OP_REG)
        && (sse3_obj.udo_dst->type != UD_OP_MEM))
        goto bad;
	if ((sse3_obj.udo_src->type != UD_OP_REG)
		&& (sse3_obj.udo_src->type != UD_OP_MEM))
		goto bad;

	// i'd like to know if this instruction is legacy mmx
	if ((sse3_obj.udo_dst->base >= UD_R_MM0)
		&& (sse3_obj.udo_dst->base <= UD_R_MM7)) {
		sse3_obj.ismmx = 1;
	} else sse3_obj.ismmx = 0;
	
	if (sse3_grab_operands(&sse3_obj) != 0) goto bad;

	opf(&sse3_obj);

if (sse3_obj.noxmmstoreres == 0)
    {
        if (sse3_commit_results(&sse3_obj)) goto bad;
    } else if (sse3_obj.udo_dst->type == UD_OP_REG) {
        if (store_reg(sse3_obj.op_obj->state, sse3_obj.udo_dst->base, sse3_obj.res.uint64[0]) != 0) goto bad;
    } else if (sse3_obj.udo_dst->type == UD_OP_MEM) {
        int64_t disp = 0;
        uint8_t disp_size = sse3_obj.udo_dst->offset;
        uint64_t address;

        if (retrieve_reg (sse3_obj.op_obj->state, sse3_obj.udo_dst->base, NULL, &address) != 0) goto bad;

        switch (disp_size) {
            case 8: disp = sse3_obj.udo_dst->lval.sbyte; break;
            case 16: disp = sse3_obj.udo_dst->lval.sword; break;
            case 32: disp = sse3_obj.udo_dst->lval.sdword; break;
            case 64: disp = sse3_obj.udo_dst->lval.sqword; break;
        }

        address += disp;

        copy_from_user ((char*) &disp, address, disp_size);
    }

	goto good;

	default: goto bad;
	}

good:
	return 0;

    // Only reached if bad
bad:
	return -1;
}

void fisttp(sse3_t *this)
{
    const unsigned char *bytep = (const unsigned char *)ud_insn_hex((ud_t *)&this->op_obj->ud_obj);
    float *src = this->src.fa32;
    float *res = this->res.fa32;
    uint8_t islongmode = is_saved_state64(this->op_obj->state);
    int ins_size = 0;
    uint8_t base = 0;
    uint8_t mod = 0;
    int8_t add = 0;
    uint8_t modrm = 0;
    uint64_t address = 0;
    uint64_t reg_sel[16];
    uint8_t modrmmask = 0xF;

    if (islongmode)
    {
        uint8_t modrmmask = 0xF;
        reg_sel[0] = this->op_obj->state64->ax;
        reg_sel[1] = this->op_obj->state64->cx;
        reg_sel[2] = this->op_obj->state64->dx;
        reg_sel[3] = this->op_obj->state64->bx;
        reg_sel[4] = this->op_obj->state64->sp;
        reg_sel[5] = this->op_obj->state64->bp;
        reg_sel[6] = this->op_obj->state64->si;
        reg_sel[7] = this->op_obj->state64->di;
        reg_sel[8] = this->op_obj->state64->r8;
        reg_sel[9] = this->op_obj->state64->r9;
        reg_sel[10] = this->op_obj->state64->r10;
        reg_sel[11] = this->op_obj->state64->r11;
        reg_sel[12] = this->op_obj->state64->r12;
        reg_sel[13] = this->op_obj->state64->r13;
        reg_sel[14] = this->op_obj->state64->r14;
        reg_sel[15] = this->op_obj->state64->r15;
    } else {
        uint8_t modrmmask = 0x7;
        reg_sel[0] = this->op_obj->state32->ax;
        reg_sel[1] = this->op_obj->state32->cx;
        reg_sel[2] = this->op_obj->state32->dx;
        reg_sel[3] = this->op_obj->state32->bx;
        reg_sel[4] = this->op_obj->state32->sp;
        reg_sel[5] = this->op_obj->state32->bp;
        reg_sel[6] = this->op_obj->state32->si;
        reg_sel[7] = this->op_obj->state32->di;
    }
    
    if (*bytep == 0x66)
    {
        bytep++;
        ins_size++;
    }
    
    switch (*bytep)
    {
        case 0xDB:
            bytep++;
            ins_size++;
            
            modrm = *bytep;
            base = modrm & modrmmask;
            mod = (modrm & 0xC0) >> 6;
            
            if (mod == 0)
            {
                address = reg_sel[base];
            } else if (mod == 1) {
                bytep++;
                ins_size++;
                
                add = *bytep;
                address = reg_sel[base] + add;
            } else {
                return;
            }
            
            fisttpl((double *)address);
            //sse3_obj->res.uint64 = *(uint64_t *)address;
            
            ins_size++;
            
            return;
            break;
            
        case 0xDD:
            bytep++;
            ins_size++;
            
            modrm = *bytep;
            base = modrm & modrmmask;
            mod = (modrm & 0xC0) >> 6;
            
            if (mod == 0)
            {
                address = reg_sel[base];
            } else if (mod == 1) {
                bytep++;
                ins_size++;
                
                add = *bytep;
                address = reg_sel[base] + add;
            } else {
                return;
            }
            
            fisttpq((long double *)address);
            //sse3_obj->res.uint64 = *(uint64_t *)address;
            
            ins_size++;
            
            return;
            break;
            
        case 0xDF:
            bytep++;
            ins_size++;
            
            modrm = *bytep;
            base = modrm & modrmmask;
            mod = (modrm & 0xC0) >> 6;
            
            if (mod == 0)
            {
                address = reg_sel[base];
            } else if (mod == 1) {
                bytep++;
                ins_size++;
                
                add = *bytep;
                address = reg_sel[base] + add;
            } else {
                return;
            }
            
            fisttps((float *)address);
            //sse3_obj->res.uint64 = *(uint64_t *)address;
            
            ins_size++;
            
            return;
            break;
    }
}

/*********************************************/
/** AnV - SSE3 instructions implementation  **/
/*********************************************/

void fisttps(float *res)
{
    float value = opemu_truncf(*res);
    __asm__ ("fistps %0" : : "m" (value));
    *res = value;
}

void fisttpl(double *res)
{
    double value = opemu_trunc(*res);
    __asm__ ("fistpl %0" : : "m" (value));
    *res = value;
}

void fisttpq(long double *res)
{
    // AnV - Truncl doesn't work but fistpq has same result in this case... (tested)
    long double value = *res; /*opemu_truncl(*res);*/
    __asm__ ("fistpq %0" : : "m" (value));
    *res = value;
}

void addsubpd(sse3_t *this)
{
    __m128d *src = (__m128d *)(&this->src.m128[0]);
    __m128d *dst = (__m128d *)(&this->dst.m128[0]);
    __m128d *res = (__m128d *)(&this->res.m128[0]);

    *res = ssp_addsub_pd_REF(*dst, *src);
}

void addsubps(sse3_t *this)
{
    __m128 *src = (__m128 *)(&this->src.m128[0]);
    __m128 *dst = (__m128 *)(&this->dst.m128[0]);
    __m128 *res = (__m128 *)(&this->res.m128[0]);

    *res = ssp_addsub_ps_REF(*dst, *src);
}

void haddpd(sse3_t *this)
{
    __m128d *src = (__m128d *)(&this->src.m128[0]);
    __m128d *dst = (__m128d *)(&this->dst.m128[0]);
    __m128d *res = (__m128d *)(&this->res.m128[0]);

    *res = ssp_hadd_pd_REF(*dst, *src);
}

void haddps(sse3_t *this)
{
    __m128 *src = (__m128 *)(&this->src.m128[0]);
    __m128 *dst = (__m128 *)(&this->dst.m128[0]);
    __m128 *res = (__m128 *)(&this->res.m128[0]);

    *res = ssp_hadd_ps_REF(*dst, *src);
}

void hsubpd(sse3_t *this)
{
    __m128d *src = (__m128d *)(&this->src.m128[0]);
    __m128d *dst = (__m128d *)(&this->dst.m128[0]);
    __m128d *res = (__m128d *)(&this->res.m128[0]);

    *res = ssp_hsub_pd_REF(*dst, *src);
}

void hsubps(sse3_t *this)
{
    __m128 *src = (__m128 *)(&this->src.m128[0]);
    __m128 *dst = (__m128 *)(&this->dst.m128[0]);
    __m128 *res = (__m128 *)(&this->res.m128[0]);

    *res = ssp_hsub_ps_REF(*dst, *src);
}

void lddqu(sse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_lddqu_si128_REF(&*src);
}

void movddup(sse3_t *this)
{
    __m128d *src = (__m128d *)(&this->src.m128);
    __m128d *res = (__m128d *)(&this->res.m128);

    *res = ssp_movedup_pd_REF(*src);
}

void movshdup(sse3_t *this)
{
    __m128 *src = &this->src.m128;
    __m128 *res = &this->res.m128;

    *res = ssp_movehdup_ps_REF(*src);
}

void movsldup(sse3_t *this)
{
    __m128 *src = &this->src.m128;
    __m128 *res = &this->res.m128;

    *res = ssp_moveldup_ps_REF(*src);
}
