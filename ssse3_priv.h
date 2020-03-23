#pragma once

#include "opemu.h"
#include "libudis86/extern.h"
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
	uint8_t		uint8[16];
	uint16_t	uint16[8];
	uint32_t	uint32[4];
	uint64_t	uint64[2];
    int32_t a32[4];
    int64_t a64[2];
    double fa64[2];
    float fa32[4];
	__uint128_t	uint128;
	__float128 fa128;
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
};
typedef struct ssse3 ssse3_t;


static inline void _load_maddr_from_xmm (uint64_t rmaddrs, sse_reg_t *where, uint16_t rm_size, struct pt_regs *regs) {
    if (is_saved_state64(regs)) {
        if (rm_size == 128) {
            __uint128_t M128RES = ((sse_reg_t*)where)->uint128;
            *((__uint128_t*)&rmaddrs) = M128RES;
        } else if (rm_size == 64) {
            uint64_t M64RES = ((sse_reg_t*)where)->uint64[0];
            *((uint64_t*)&rmaddrs) = M64RES;
        } else if (rm_size == 32) {
            uint32_t M32RES = ((sse_reg_t*)where)->uint32[0];
            *((uint32_t*)&rmaddrs) = M32RES;
        }
    } else {
        uint32_t rmaddrs32 = rmaddrs & 0xffffffff;
        if (rm_size == 128) {
            __uint128_t M128RES = ((sse_reg_t*)where)->uint128;
            *((__uint128_t*)&rmaddrs32) = M128RES;
        } else if (rm_size == 64) {
            uint64_t M64RES = ((sse_reg_t*)where)->uint64[0];
            *((uint64_t*)&rmaddrs32) = M64RES;
        } else if (rm_size == 32) {
            uint32_t M32RES = ((sse_reg_t*)where)->uint32[0];
            *((uint32_t*)&rmaddrs) = M32RES;
        }
    }
}





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

//SSE 4.1
inline void blendpd	(ssse3_t*);
inline void blendps	(ssse3_t*);
inline void pblendw	(ssse3_t*);
inline void pextrb	(ssse3_t*);
inline void pmovsxbd	(ssse3_t*);
inline void pmovsxbq	(ssse3_t*);
inline void pmovsxbw	(ssse3_t*);
inline void pmovsxdq	(ssse3_t*);
inline void pmovsxwd	(ssse3_t*);
inline void pmovsxwq	(ssse3_t*);
inline void pmovzxbd	(ssse3_t*);
inline void pmovzxbq	(ssse3_t*);
inline void pmovzxbw	(ssse3_t*);
inline void pmovzxdq	(ssse3_t*);
inline void pmovzxwd	(ssse3_t*);
inline void pmovzxwq	(ssse3_t*);
inline void roundss	(ssse3_t*);
inline void pextrb	(ssse3_t*);
inline void pextrd	(ssse3_t*);
inline void pextrq	(ssse3_t*);
inline void ptest	(ssse3_t*);
inline void pinsrb	(ssse3_t*);
inline void pinsrd	(ssse3_t*);
inline void pinsrq	(ssse3_t*);


/*** SSE4.2 TODO move this somewhere else ***/
inline void pcmpistri	(ssse3_t*);
inline void pcmpestri	(ssse3_t*);
inline void pcmpestrm	(ssse3_t*);
inline void pcmpistrm	(ssse3_t*);
inline void pcmpgtq     (ssse3_t*);
inline void popcnt      (ssse3_t*);
inline void crc32_op    (ssse3_t*);
