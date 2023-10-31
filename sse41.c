#include "ssse3_priv.h"
#include <asm/fpu/types.h>

void pmaxud(ssse3_t *this)
{
    int32_t * temp1 = this->src.uint32;
    int32_t * temp2 = this->dst.uint32;

    this->res.int32[0] = (temp1[0] > temp2[0]) ? temp1[0] : temp2[0];
    this->res.int32[1] = (temp1[1] > temp2[1]) ? temp1[1] : temp2[1];
    this->res.int32[2] = (temp1[2] > temp2[2]) ? temp1[2] : temp2[2];
    this->res.int32[3] = (temp1[3] > temp2[3]) ? temp1[3] : temp2[3];
}

void pminud(ssse3_t *this)
{
    int32_t * temp1 = this->src.uint32;
    int32_t * temp2 = this->dst.uint32;

    this->res.int32[0] = (temp1[0] < temp2[0]) ? temp1[0] : temp2[0];
    this->res.int32[1] = (temp1[1] < temp2[1]) ? temp1[1] : temp2[1];
    this->res.int32[2] = (temp1[2] < temp2[2]) ? temp1[2] : temp2[2];
    this->res.int32[3] = (temp1[3] < temp2[3]) ? temp1[3] : temp2[3];
}


void blendpd(ssse3_t *this)
{
    uint8_t  imm   = this->udo_imm->lval.ubyte;
    double * temp1 = this->src.fa64;
    double * temp2 = this->dst.fa64;

    this->res.fa64[0] = ((imm & (1 << 0))) ? temp2[0] = temp1[0] : temp2[0];
    this->res.fa64[1] = ((imm & (1 << 1))) ? temp2[1] = temp1[1] : temp2[1];
}

void blendps(ssse3_t *this)
{
    uint8_t imm   = this->udo_imm->lval.ubyte;
    float * temp1 = this->src.fa32;
    float * temp2 = this->dst.fa32;

    this->res.fa32[0] = ((imm & (1 << 0))) ? temp2[0] = temp1[0] : temp2[0];
    this->res.fa32[1] = ((imm & (1 << 1))) ? temp2[1] = temp1[1] : temp2[1];
    this->res.fa32[2] = ((imm & (1 << 2))) ? temp2[2] = temp1[2] : temp2[2];
    this->res.fa32[3] = ((imm & (1 << 3))) ? temp2[3] = temp1[3] : temp2[3];
}

void pblendw(ssse3_t *this)
{
    uint8_t imm   = this->udo_imm->lval.ubyte;
    uint16_t * temp1 = this->src.uint16;
    uint16_t * temp2 = this->dst.uint16;

    this->res.uint16[0] = ((imm & (1 << 0))) ? temp2[0] = temp1[0] : temp2[0];
    this->res.uint16[1] = ((imm & (1 << 1))) ? temp2[1] = temp1[1] : temp2[1];
    this->res.uint16[2] = ((imm & (1 << 2))) ? temp2[2] = temp1[2] : temp2[2];
    this->res.uint16[3] = ((imm & (1 << 3))) ? temp2[3] = temp1[3] : temp2[3];
    this->res.uint16[4] = ((imm & (1 << 4))) ? temp2[4] = temp1[4] : temp2[4];
    this->res.uint16[5] = ((imm & (1 << 5))) ? temp2[5] = temp1[5] : temp2[5];
    this->res.uint16[6] = ((imm & (1 << 6))) ? temp2[6] = temp1[6] : temp2[6];
    this->res.uint16[7] = ((imm & (1 << 7))) ? temp2[7] = temp1[7] : temp2[7];
}

void pmovsxbw(ssse3_t *this)
{
    int8_t  * temp1 = this->src.uint8;
    int16_t * temp2 = this->dst.uint16;

    this->res.uint16[0] = (temp1[0] > 127) ? temp2[0] = (0xFF00 | temp1[0]) : (temp2[0] = temp1[0]);
    this->res.uint16[1] = (temp1[1] > 127) ? temp2[1] = (0xFF00 | temp1[1]) : (temp2[1] = temp1[1]);
    this->res.uint16[2] = (temp1[2] > 127) ? temp2[2] = (0xFF00 | temp1[2]) : (temp2[2] = temp1[2]);
    this->res.uint16[3] = (temp1[3] > 127) ? temp2[3] = (0xFF00 | temp1[3]) : (temp2[3] = temp1[3]);
    this->res.uint16[4] = (temp1[4] > 127) ? temp2[4] = (0xFF00 | temp1[4]) : (temp2[4] = temp1[4]);
    this->res.uint16[5] = (temp1[5] > 127) ? temp2[5] = (0xFF00 | temp1[5]) : (temp2[5] = temp1[5]);
    this->res.uint16[6] = (temp1[6] > 127) ? temp2[6] = (0xFF00 | temp1[6]) : (temp2[6] = temp1[6]);
    this->res.uint16[7] = (temp1[7] > 127) ? temp2[7] = (0xFF00 | temp1[7]) : (temp2[7] = temp1[7]);
}

void pmovsxbd(ssse3_t *this)
{
    int8_t  * temp1 = this->src.uint8;
    int32_t * temp2 = this->dst.uint32;

    this->res.uint32[0] = (temp1[0] > 127) ? temp2[0] = (0xFFFF00 | temp1[0]) : (temp2[0] = temp1[0]);
    this->res.uint32[1] = (temp1[1] > 127) ? temp2[1] = (0xFFFF00 | temp1[1]) : (temp2[1] = temp1[1]);
    this->res.uint32[2] = (temp1[2] > 127) ? temp2[2] = (0xFFFF00 | temp1[2]) : (temp2[2] = temp1[2]);
    this->res.uint32[3] = (temp1[3] > 127) ? temp2[3] = (0xFFFF00 | temp1[3]) : (temp2[3] = temp1[3]);
}

void pmovsxbq(ssse3_t *this)
{
    int8_t  * temp1 = this->src.uint8;
    int64_t * temp2 = this->dst.uint64;

    this->res.uint64[0] = (temp1[0] > 127) ? temp2[0] = (0xFFFFFFFFFFFFFF00 | temp1[0]) : (temp2[0] = temp1[0]);
    this->res.uint64[1] = (temp1[1] > 127) ? temp2[1] = (0xFFFFFFFFFFFFFF00 | temp1[1]) : (temp2[1] = temp1[1]);
}

void pmovsxwd(ssse3_t *this)
{
    int16_t * temp1 = this->src.uint16;
    int32_t * temp2 = this->dst.uint32;

    this->res.uint32[0] = (temp1[0] > 32767) ? temp2[0] = (0xFFFF0000 | temp1[0]) : (temp2[0] = temp1[0]);
    this->res.uint32[1] = (temp1[1] > 32767) ? temp2[1] = (0xFFFF0000 | temp1[1]) : (temp2[1] = temp1[1]);
    this->res.uint32[2] = (temp1[2] > 32767) ? temp2[2] = (0xFFFF0000 | temp1[2]) : (temp2[2] = temp1[2]);
    this->res.uint32[3] = (temp1[3] > 32767) ? temp2[3] = (0xFFFF0000 | temp1[3]) : (temp2[3] = temp1[3]);
}

void pmovsxwq(ssse3_t *this)
{
	uint16_t* temp1 = this->src.uint128;
	uint64_t* temp2 = this->dst.uint128;

	temp2[0] = temp1[0] > 32767 ? 0xFFFFFFFFFFFF0000 | temp1[0] : (uint64_t) temp1[0];
	temp2[1] = temp1[1] > 32767 ? 0xFFFFFFFFFFFF0000 | temp1[1] : (uint64_t) temp1[1];

	this->res.uint128 = ((__uint128_t*) temp2);
}

void pmovsxdq(ssse3_t *this)
{
	uint32_t* temp1 = this->src.uint128;
	uint64_t* temp2 = this->dst.uint128;

	temp2[0] = temp1[0] > 0x7FFFFFFF ? 0xFFFFFFFF00000000 | temp1[0] : (uint64_t) temp1[0];
	temp2[1] = temp1[1] > 0x7FFFFFFF ? 0xFFFFFFFF00000000 | temp1[1] : (uint64_t) temp1[1];

	this->res.uint128 = ((__uint128_t*) temp2);
}


void pmovzxbw(ssse3_t *this)
{
	int8_t* temp1 = this->src.uint128;
	int16_t* temp2 = this->dst.uint128;


	temp2[0] = temp1[0] > 127 ? 0xFF00 | temp1[0] : temp1[0];
	temp2[1] = temp1[1] > 127 ? 0xFF00 | temp1[1] : temp1[1];
	temp2[2] = temp1[2] > 127 ? 0xFF00 | temp1[2] : temp1[2];
	temp2[3] = temp1[3] > 127 ? 0xFF00 | temp1[3] : temp1[3];
	temp2[4] = temp1[4] > 127 ? 0xFF00 | temp1[4] : temp1[4];
	temp2[5] = temp1[5] > 127 ? 0xFF00 | temp1[5] : temp1[5];
	temp2[6] = temp1[6] > 127 ? 0xFF00 | temp1[6] : temp1[6];
	temp2[7] = temp1[7] > 127 ? 0xFF00 | temp1[7] : temp1[7];

	this->res.uint128 = ((__uint128_t*) temp2);
}

void pmovzxbd(ssse3_t *this)
{
	uint8_t* temp1 = this->src.uint128;
	uint32_t* temp2 = this->dst.uint128;

	temp2[0] = temp1[0] > 127 ? 0xFFFF00 | temp1[0] : (uint32_t) temp1[0];
	temp2[1] = temp1[1] > 127 ? 0xFFFF00 | temp1[1] : (uint32_t) temp1[1];
	temp2[2] = temp1[2] > 127 ? 0xFFFF00 | temp1[2] : (uint32_t) temp1[2];
	temp2[3] = temp1[3] > 127 ? 0xFFFF00 | temp1[3] : (uint32_t) temp1[3];

	this->res.uint128 = ((__uint128_t*) temp2);
}

void pmovzxbq(ssse3_t *this)
{
	uint8_t* temp1 = this->src.uint128;
	uint64_t* temp2 = this->dst.uint128;

	temp2[0] = 0x00000000000000FF & temp1[0];
	temp2[1] = 0x00000000000000FF & temp1[1];

	this->res.uint128 = ((__uint128_t*) temp2);
}

void pmovzxwd(ssse3_t *this)
{
	uint16_t* temp1 = this->src.uint128;
	uint32_t* temp2 = this->dst.uint128;

	temp2[0] = 0x0000FFFF & temp1[0];
	temp2[1] = 0x0000FFFF & temp1[1];
	temp2[2] = 0x0000FFFF & temp1[2];
	temp2[3] = 0x0000FFFF & temp1[3];

	this->res.uint128 = ((__uint128_t*) temp2);
}

void pmovzxwq(ssse3_t *this)
{
	uint16_t* temp1 = this->src.uint128;
	uint64_t* temp2 = this->dst.uint128;

	temp2[0] = 0x000000000000FFFF & temp1[0];
	temp2[1] = 0x000000000000FFFF & temp1[1];

	this->res.uint128 = ((__uint128_t*) temp2);
}

void pmovzxdq(ssse3_t *this)
{
	uint32_t* temp1 = this->src.uint128;
	uint64_t* temp2 = this->dst.uint128;

	temp2[0] = 0x00000000FFFFFFFF & temp1[0];
	temp2[1] = 0x00000000FFFFFFFF & temp1[1];

	this->res.uint128 = ((__uint128_t*) temp2);
}

void pextrb(ssse3_t *this)
{
	uint8_t sel = this->udo_imm->lval.ubyte & 0xF;
	uint8_t temp1 = this->src.uint8[sel];
    uint8_t islongmode = is_saved_state64(this->op_obj->state);

	if (this->udo_dst->type == UD_OP_MEM) {
		this->res.uint8[0] = temp1;
		printk("pextrb this->udo_dst->type == UD_OP_MEM this->res.uint8[0]: %hhu", temp1);
	}
	else if (islongmode && this->udo_dst->size == 64) {
		this->res.uint64[0] = 0;
		this->res.uint8[0] = temp1;
		printk("pextrb islongmode && this->udo_dst->size == 64 this->res.uint64[0]: %hhu", temp1);
	}
	else {
		this->res.uint32[0] = 0;
		this->res.uint8[0] = temp1;
		printk("pextrb this->res.uint32[0]: %hhu", temp1);
	}
	printk("OPEMU:  %s\n", ud_insn_asm(this->op_obj->ud_obj));

}

void pextrd(ssse3_t *this)
{
	uint8_t sel = this->udo_imm->lval.ubyte & 3;
    this->res.uint32[0] = 0;
    this->res.uint32[0] = this->src.uint32[sel];
    //printk("temp1: %hhu", this->res.uint64[0]);
    //printk("2temp1: %hhu", this->res.uint128);
	printk("OPEMU: pextrd %s\n", ud_insn_asm(this->op_obj->ud_obj));
    //printk("e %u", this->res.uint64[0]);
}

void pextrq(ssse3_t *this)
{
	uint8_t sel = this->udo_imm->lval.ubyte & 1;
	uint64_t temp1 = this->src.uint64[sel];
    uint8_t islongmode = is_saved_state64(this->op_obj->state);

	if (this->udo_dst->type == UD_OP_MEM) {
		this->res.uint128 = temp1;
		printk("pextrq this->udo_dst->type == UD_OP_MEM this->res.uint128: %hhu", temp1);
	}
	else if (islongmode && this->udo_dst->size == 128) {
		this->res.uint128 = 0;
		this->res.uint128 = temp1;
		printk("pextrq islongmode && this->udo_dst->size == 128 this->res.uint128: %hhu", temp1);
	}
	else if (islongmode && this->udo_dst->size == 64) {
		this->res.uint64[0] = 0;
		this->res.uint64[0] = temp1;
		//printk("pextrq islongmode && this->udo_dst->size == 64 this->res.uint64[0]: %hhu", temp1);
	}
	else {
		this->res.uint32[0] = 0;
		this->res.uint32[0] = temp1;
		printk("pextrq this->res.uint32[0]: %hhu", temp1);
	}
	//printk("OPEMU:  %s\n", ud_insn_asm(this->op_obj->ud_obj));

}

void roundss(ssse3_t *this)
{
	kernel_fpu_begin();
	uint8_t imm = this->udo_imm->lval.ubyte;
    int rc;
    int msi = (imm >> 2) & 1;
    if (msi == 0) {
        rc = imm & 3;
    } else {
        //get mxcsr round control
        rc = getmxcsr();
    }
	this->res.fa32[0] = round_fp32(this->src.fa32[0], rc);
	//printk("roundss this->res.uint32[0]: %hhu", this->res.uint32[0]);
	//printk("roundss this->res.uint32[1]: %hhu", this->res.uint32[1]);
	//printk("roundss this->res.uint32[2]: %hhu", this->res.uint32[2]);
	//printk("roundss this->res.uint32[3]: %hhu", this->res.uint32[3]);
	//printk("roundss temp1: %hhu", temp1);
	//printk("roundss this->src.uint32[0]: %hhu", this->src.uint32[0]);
	kernel_fpu_end();
}



void ptest(ssse3_t *this)
{
	struct pt_regs *regs;
    uint64_t FLAGS;
    FLAGS = regs->flags;

    sse_reg_t AND1, AND2;

    AND1.uint128 = this->src.uint128 & this->dst.uint128;
    AND2.uint128 = this->src.uint128 & ~(this->dst.uint128);

    if (AND1.uint128 == 0) {
        FLAGS |= 0x00000040; //set ZF = 1
    } else {
        FLAGS = (FLAGS | 0x00000040) ^ 0x00000040;  //set ZF = 0
    }

    if (AND2.uint128 == 0) {
        FLAGS |= 0x00000001; //set CF = 1
    } else {
        FLAGS = (FLAGS | 0x00000001) ^ 0x00000001;  //set CF = 0
    }

    FLAGS = (FLAGS | 0x00000800) ^ 0x00000800;  //set OF = 0
    FLAGS = (FLAGS | 0x00000010) ^ 0x00000010;  //set AF = 0
    FLAGS = (FLAGS | 0x00000080) ^ 0x00000080;  //set SF = 0
    FLAGS = (FLAGS | 0x00000004) ^ 0x00000004;  //set PF = 0

    regs->flags = FLAGS;

}

void pinsrb(ssse3_t *this)
{
	uint8_t sel = this->udo_imm->lval.ubyte & 0xF;
	//uint32_t mask = 0xFFFFFFFF << (sel * 32);
    //printk("1temp1: %hhu", this->dst.uint128);
	//uint32_t res = &this->res.uint128;
	this->dst.uint8[sel] = this->src.uint8[0];
	this->res.uint128 = this->dst.uint128;
	printk("OPEMU: pinsrb %s\n", ud_insn_asm(this->op_obj->ud_obj));

}

void pinsrd(ssse3_t *this)
{
	uint8_t sel = this->udo_imm->lval.ubyte & 3;
	//uint32_t mask = 0xFFFFFFFF << (sel * 32);
    //printk("1temp1: %hhu", this->dst.uint128);
	//uint32_t res = &this->res.uint128;
	__uint128_t mask = 0xFFFFFFFF << (sel * 32);
	this->res.uint128 = (this->dst.uint128 & ~mask) | ((this->src.uint128 << (sel * 32)) & mask);
}

void pinsrq(ssse3_t *this)
{
	uint8_t sel = this->udo_imm->lval.ubyte & 1;
	//uint32_t mask = 0xFFFFFFFF << (sel * 32);
    //printk("1temp1: %hhu", this->dst.uint128);
	//uint32_t res = &this->res.uint128;
	this->dst.uint64[sel] = this->src.uint64[0];
	this->res.uint128 = this->dst.uint128;
	printk("OPEMU: pinsrq %s\n", ud_insn_asm(this->op_obj->ud_obj));

}
