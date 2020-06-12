//
// Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
// This software is subject to the Apache v2.0 License.
//
#ifndef __SSEPLUS_ARITHMETIC_REF_H__
#define __SSEPLUS_ARITHMETIC_REF_H__


/** @addtogroup supplimental_REF   
 *  @{ 
 *  @name Arithmetic Operations
 */


/** */
SSP_FORCEINLINE __m128 ssp_arithmetic_hadd4_dup_ps_REF( __m128 a )      // Sum all 4 values
{
    ssp_m128 A;
    ssp_f32 t;
    
    A.f = a;
    t   = A.f32[0] + 
          A.f32[1] + 
          A.f32[2] + 
          A.f32[3];

    A.f32[0] = t;
    A.f32[1] = t;
    A.f32[2] = t;
    A.f32[3] = t;
    return A.f;
}  

SSP_FORCEINLINE __m128i ssp_arithmetic_hadd4_epu16_REF( __m128i a )      // Sum 2 sets of 4 values, dest in 0, and 4
{
    ssp_m128 A;
    A.i = a;

    A.u16[0] = A.u16[0] + A.u16[1] + + A.u16[2] + + A.u16[3];
    A.u16[4] = A.u16[4] + A.u16[5] + + A.u16[6] + + A.u16[7];

    return A.i;
}  

//@}
//@}

#define __HI(x) *(1+(int*)&x)
#define __LO(x) *(int*)&x

static const ssp_f64 huge = 1.0e300;

static
ssp_f64 ssp_ceil(ssp_f64 x)
{
	int i0,i1,j0;
	unsigned i,j;
	i0 =  __HI(x);
	i1 =  __LO(x);
	j0 = ((i0>>20)&0x7ff)-0x3ff;
	if(j0<20) {
		if(j0<0) {	/* raise inexact if x != 0 */
			if(huge+x>0.0) {/* return 0*sign(x) if |x|<1 */
				if(i0<0) {
					i0=0x80000000;
					i1=0;
				} else if((i0|i1)!=0) {
					i0=0x3ff00000;
					i1=0;
				}
			}
		} else {
			i = (0x000fffff)>>j0;
			if(((i0&i)|i1)==0)
				return x; /* x is integral */
			if(huge+x>0.0) {	/* raise inexact flag */
				if(i0>0)
					i0 += (0x00100000)>>j0;
				i0 &= (~i); i1=0;
			}
		}
	} else if (j0>51) {
		if(j0==0x400)
			return x+x;	/* inf or NaN */
		else
			return x;		/* x is integral */
	} else {
		i = ((unsigned)(0xffffffff))>>(j0-20);
		if((i1&i)==0)
			return x;	/* x is integral */
		if(huge+x>0.0) {		/* raise inexact flag */
			if(i0>0) {
				if(j0==20)
					i0+=1;
				else {
					j = i1 + (1<<(52-j0));
					if(j<i1)
						i0+=1;	/* got a carry */
					i1 = j;
				}
			}
			i1 &= (~i);
		}
	}
	__HI(x) = i0;
	__LO(x) = i1;
	return x;
}

static
ssp_f64 ssp_floor(ssp_f64 x)
{
	int i0,i1,j0;
	unsigned i,j;
	i0 =  __HI(x);
	i1 =  __LO(x);
	j0 = ((i0>>20)&0x7ff)-0x3ff;
	if(j0<20) {
		if(j0<0) {	/* raise inexact if x != 0 */
			if(huge+x>0.0) {/* return 0*sign(x) if |x|<1 */
				if(i0>=0)
					i0=i1=0;
				else if(((i0&0x7fffffff)|i1)!=0) {
					i0=0xbff00000;
					i1=0;
				}
			}
		} else {
			i = (0x000fffff)>>j0;
			if(((i0&i)|i1)==0)
				return x; /* x is integral */
			if(huge+x>0.0) {	/* raise inexact flag */
				if(i0<0)
					i0 += (0x00100000)>>j0;
				i0 &= (~i); i1=0;
			}
		}
	} else if (j0>51) {
		if(j0==0x400)
			return x+x;	/* inf or NaN */
		else
			return x;		/* x is integral */
	} else {
		i = ((unsigned)(0xffffffff))>>(j0-20);
		if((i1&i)==0)
			return x;	/* x is integral */
		if(huge+x>0.0) {		/* raise inexact flag */
			if(i0<0) {
				if(j0==20)
					i0+=1;
				else {
					j = i1+(1<<(52-j0));
					if(j<i1)
						i0 +=1 ;	/* got a carry */
					i1=j;
				}
			}
			i1 &= (~i);
		}
	}
	__HI(x) = i0;
	__LO(x) = i1;
	return x;
}

#endif // __SSEPLUS_ARITHMETIC_REF_H__

