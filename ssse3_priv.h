#pragma once

#include "opemu.h"
#include "libudis86/extern.h"
#include "SSEPlus/SSEPlus_REF.h"
#include "fpins.h"
#include <asm/fpu/internal.h>

// log function debug
#define LF	D("%s\n", __PRETTY_FUNCTION__);
#define D	printk

/**
 * 128-bit Register proper for ssse3
 * For 64-bit operations, use the same register type, and ignore the high values
 */
union __attribute__((__packed__)) sse_reg {
	int8_t		int8[16];
	int16_t		int16[8];
	int32_t		int32[4];
	int64_t		int64[2];
    __int128_t	int128;
    float       fa32[4];
	uint8_t		uint8[16];
	uint16_t	uint16[8];
	uint32_t	uint32[4];
	uint64_t	uint64[2];
    double      fa64[2];
	__uint128_t	uint128;
    ssp_m128    ssp_m128;
    __m128i     m128i;
    __m128d     m128d;
    __m128      m128;
    __m64       m64[2];
};
typedef union sse_reg sse_reg_t;

/**
 * Print said register to screen. Useful for debugging
 * @param  uint128
 */
#define print128(x)	printk("0x%016llx%016llx", ((uint64_t*)(&(x)))[1], ((uint64_t*)(&(x)))[0] );

/**
 * ssse3 object
 */
struct ssse3 {
	uint8_t	extended;	// bool type

	sse_reg_t		dst, src;
	sse_reg_t		res;

	// operands
	const ud_operand_t	*udo_src, *udo_dst, *udo_imm;

	// objects
	op_t		*op_obj;

	uint8_t	dst64;
	uint8_t	dst32;

	uint64_t	res64;
	uint32_t	res32;

	// legacy mmx flag
	uint8_t 		ismmx;

	// Don't store result in mm/xmm
    uint8_t         noxmmstoreres;
};
typedef struct ssse3 ssse3_t;



/**
 * Instruction emulation function type.
 */
typedef void (*ssse3_func)(ssse3_t*);


#define fstored_template(n, where)					\
	do {								\
	kernel_fpu_begin(); \
	asm __volatile__ ("movss %%xmm" #n ", %0" : "=m" (*(where)));	\
	kernel_fpu_end(); \
	} while (0);

#define floadd_template(n, where)					\
	do {		 \
	kernel_fpu_begin(); \
	asm __volatile__ ("movss %0, %%xmm" #n :: "m" (*(where)));	\
	kernel_fpu_end(); \
	} while (0);
	
#define storedqu_template(n, where)					\
	do {								\
	asm __volatile__ ("movdqu %%xmm" #n ", %0" : "=m" (*(where)));	\
	} while (0);

#define loaddqu_template(n, where)					\
	do {		 \
	asm __volatile__ ("movdqu %0, %%xmm" #n :: "m" (*(where)));	\
	} while (0);

#define storeq_template(n, where)					\
	do {								\
	asm __volatile__ ("movq %%mm" #n ", %0" : "=m" (*(where)));	\
	} while (0);

#define loadq_template(n, where)					\
	do {								\
	asm __volatile__ ("movq %0, %%mm" #n :: "m" (*(where)));	\
	} while (0);
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
}

inline void _sstore_gpr64 (ud_type_t n, uint64_t *where) {
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

inline int ssse3_grab_operands(ssse3_t*);
inline int ssse3_commit_results(ssse3_t*);
inline int op_sse3x_run(op_t*);

inline void psignb	(ssse3_t*);
inline void psignw	(ssse3_t*);
inline void psignd	(ssse3_t*);
inline void pabsb	(ssse3_t*);
inline void pabsw	(ssse3_t*);
inline void pabsd	(ssse3_t*);
inline void palignr	(ssse3_t*);
inline void pshufb	(ssse3_t*);
inline void pmulhrsw	(ssse3_t*);
inline void pmaddubsw	(ssse3_t*);
inline void phsubw	(ssse3_t*);
inline void phsubd	(ssse3_t*);
inline void phsubsw	(ssse3_t*);
inline void phaddw	(ssse3_t*);
inline void phaddd	(ssse3_t*);
inline void phaddsw	(ssse3_t*);

/*** SSE4.1 TODO move this somewhere else ***/
inline void blendpd (ssse3_t*);
inline void blendps (ssse3_t*);
inline void blendvpd (ssse3_t*);
inline void dppd (ssse3_t*);
inline void dpps (ssse3_t*);
inline void extractps (ssse3_t*);
inline void insertps (ssse3_t*);
//inline void movntdqa (ssse3_t*);
inline void mpsadbw (ssse3_t*);
inline void packusdw (ssse3_t*);
inline void pblendvb (ssse3_t*);
inline void pblendw (ssse3_t*);
inline void pcmpeqq (ssse3_t*);
//inline void pextrb (ssse3_t*);
//inline void pextrd (ssse3_t*);
//inline void pextrq (ssse3_t*);
//inline void pextrw (ssse3_t*);
inline void phminposuw (ssse3_t*);
inline void pinsrb (ssse3_t*);
inline void pinsrd (ssse3_t*);
inline void pinsrq (ssse3_t*);
inline void pmaxsb (ssse3_t*);
inline void pmaxsd (ssse3_t*);
inline void pmaxud (ssse3_t*);
inline void pmaxuw (ssse3_t*);
inline void pminsb (ssse3_t*);
inline void pminsd (ssse3_t*);
inline void pminud (ssse3_t*);
inline void pminuw (ssse3_t*);
inline void pmovsxbd (ssse3_t*);
inline void pmovsxbq (ssse3_t*);
inline void pmovsxbw (ssse3_t*);
inline void pmovsxwd (ssse3_t*);
inline void pmovsxwq (ssse3_t*);
inline void pmovsxdq (ssse3_t*);
inline void pmovzxbd (ssse3_t*);
inline void pmovzxbq (ssse3_t*);
inline void pmovzxbw (ssse3_t*);
inline void pmovzxwd (ssse3_t*);
inline void pmovzxwq (ssse3_t*);
inline void pmovzxdq (ssse3_t*);
inline void pmuldq (ssse3_t*);
inline void pmulld (ssse3_t*);
inline void ptest (ssse3_t*);
inline void roundpd (ssse3_t*);
inline void roundps (ssse3_t*);
inline void roundsd (ssse3_t*);
inline void roundss (ssse3_t*);

/*** SSE4.2 TODO move this somewhere else ***/
inline void pcmpistri	(ssse3_t*);
inline void pcmpestri	(ssse3_t*);
inline void pcmpestrm	(ssse3_t*);
inline void pcmpistrm	(ssse3_t*);
inline void pcmpgtq     (ssse3_t*);
inline void popcnt      (ssse3_t*);
inline void crc32_op    (ssse3_t*);
