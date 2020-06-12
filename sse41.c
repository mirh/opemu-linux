#include "ssse3_priv.h"
//#include <asm/fpu/internal.h>

void blendpd (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128d *src = &this->src.m128d;
    __m128d *dst = &this->dst.m128d;
    __m128d *res = &this->res.m128d;

    *res = ssp_blend_pd_REF(*dst, *src, imm);
}

void blendps (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128 *src = &this->src.m128;
    __m128 *dst = &this->dst.m128;
    __m128 *res = &this->res.m128;

    *res = ssp_blend_ps_REF(*dst, *src, imm);
}

inline void blendvpd (ssse3_t *this)
{
    __m128d imm = (__m128d)(this->udo_imm->lval.uqqword);
    __m128d *src = (__m128d *)(&this->src.m128);
    __m128d *dst = (__m128d *)(&this->dst.m128);
    __m128d *res = (__m128d *)(&this->res.m128);

    *res = ssp_blendv_pd_REF(*dst, *src, imm);
}

inline void dppd (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128d *src = &this->src.m128d;
    __m128d *dst = &this->dst.m128d;
    __m128d *res = &this->res.m128d;

    *res = ssp_dp_pd_REF(*dst, *src, imm);
}

inline void dpps (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128 *src = &this->src.m128;
    __m128 *dst = &this->dst.m128;
    __m128 *res = &this->res.m128;

    *res = ssp_dp_ps_REF(*dst, *src, imm);
}

inline void extractps (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128i *src = &this->src.m128i;
    uint64_t *res = (uint64_t *)&this->res.uint64;

    *res = (uint64_t)ssp_extract_epi32_REF(*src, imm);
}

inline void insertps (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128 *src = &this->src.m128;
    __m128 *dst = &this->dst.m128;
    __m128 *res = &this->res.m128;

    *res = ssp_insert_ps_REF(*dst, *src, imm);
}

/*
inline void movntdqa (ssse3_t *this)
{
    uint64_t  *src = &this->src.uint64;
    uint64_t *res = &this->res.uint64;

    *res[0] = *src[0];
    *res[1] = *src[1];
}
*/

inline void mpsadbw (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_mpsadbw_epu8_REF(*dst, *src, imm);
}

inline void packusdw (ssse3_t *this)
{
    __m128i *src = (__m128i *)(&this->src.m128d);
    __m128i *dst = (__m128i *)(&this->dst.m128d);
    __m128i *res = (__m128i *)(&this->res.m128d);

    *res = ssp_packus_epi32_REF(*dst, *src);
}

inline void pblendvb (ssse3_t *this)
{
    __m128i imm = (__m128i)(this->udo_imm->lval.uqqword);
    __m128i *src = (__m128i *)(&this->src.m128d);
    __m128i *dst = (__m128i *)(&this->dst.m128d);
    __m128i *res = (__m128i *)(&this->res.m128d);

    *res = ssp_blendv_epi8_REF(*dst, *src, imm);
}

inline void pblendw (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_blend_epi16_REF(*dst, *src, imm);
}

inline void pcmpeqq (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cmpeq_epi64_REF(*dst, *src);
}

/*
inline void pextrb (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_extract_epi8_REF(*src, imm);
}

inline void pextrd (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_extract_epi32_REF(*src, imm);
}

inline void pextrq (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_extract_epi64_REF(*src, imm);
}

inline void pextrw (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_extract_epi16_REF(*src, imm);
}
*/

inline void phminposuw (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_minpos_epu16_REF(*src);
}

inline void pinsrb (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128i *dst = &this->dst.m128i;
    int *src = (int *)&this->src.int8;
    __m128i *res = &this->res.m128i;

    *res = ssp_insert_epi8_REF(*dst, *src, imm);
}

inline void pinsrd (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128i *dst = &this->dst.m128i;
    int *src = (int *)&this->src.int32;
    __m128i *res = &this->res.m128i;

    *res = ssp_insert_epi32_REF(*dst, *src, imm);
}

inline void pinsrq (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128i *dst = &this->dst.m128i;
    ssp_s64 *src = (ssp_s64 *)&this->src.int64;
    __m128i *res = &this->res.m128i;

    *res = ssp_insert_epi64_REF(*dst, *src, imm);
}

inline void pmaxsb (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_max_epi8_REF(*dst, *src);
}

inline void pmaxsd (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_max_epi32_REF(*dst, *src);
}

inline void pmaxud (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_max_epu32_REF(*dst, *src);
}

inline void pmaxuw (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_max_epu16_REF(*dst, *src);
}

inline void pminsb (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_min_epi8_REF(*dst, *src);
}

inline void pminsd (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_min_epi32_REF(*dst, *src);
}

inline void pminud (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_min_epu32_REF(*dst, *src);
}

inline void pminuw (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_min_epu16_REF(*dst, *src);
}

inline void pmovsxbd (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepi8_epi32_REF(*src);
}

inline void pmovsxbq (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepi8_epi64_REF(*src);
}

inline void pmovsxbw (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepi8_epi16_REF(*src);
}

inline void pmovsxwd (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepi16_epi32_REF(*src);
}

inline void pmovsxwq (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepi16_epi64_REF(*src);
}

inline void pmovsxdq (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepi32_epi64_REF(*src);
}

inline void pmovzxbd (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepu8_epi32_REF(*src);
}

inline void pmovzxbq (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepu8_epi64_REF(*src);
}

inline void pmovzxbw (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepu8_epi16_REF(*src);
}

inline void pmovzxwd (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepu16_epi32_REF(*src);
}

inline void pmovzxwq (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepu16_epi64_REF(*src);
}

inline void pmovzxdq (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_cvtepu32_epi64_REF(*src);
}

inline void pmuldq (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_mul_epi32_REF(*dst, *src);
}

inline void pmulld (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;
    __m128i *res = &this->res.m128i;

    *res = ssp_mullo_epi32_REF(*dst, *src);
}

inline void ptest (ssse3_t *this)
{
    __m128i *src = &this->src.m128i;
    __m128i *dst = &this->dst.m128i;

//    __asm__("clc;"
//            "clz;"
//            "cla;"
//            "clp;"
//            "clo;");

    if (ssp_testc_si128_REF(*dst, *src))
    {
        __asm__("stc;");
    }

/*    if (ssp_testz_si128_REF(*dst, *src))
    {
        __asm__("stz;");
    }
*/
}

inline void roundpd (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128d *src = &this->src.m128d;
    __m128d *res = &this->res.m128d;

    *res = ssp_round_pd_REF(*src, imm);
}

inline void roundps (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128 *src = &this->src.m128;
    __m128 *res = &this->res.m128;

    *res = ssp_round_ps_REF(*src, imm);
}

inline void roundsd (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128d *src = &this->src.m128d;
    __m128d *dst = &this->dst.m128d;
    __m128d *res = &this->res.m128d;

    *res = ssp_round_sd_REF(*dst, *src, imm);
}

inline void roundss (ssse3_t *this)
{
    const int imm = (const int)(this->udo_imm->lval.sbyte);
    __m128d *src = (__m128d *)(&this->src.m128);
    __m128d *dst = (__m128d *)(&this->dst.m128);
    __m128d *res = (__m128d *)(&this->res.m128);

    *res = ssp_round_sd_REF(*dst, *src, imm);
}
