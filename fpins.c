//
//  fpins.c
//  opemu
//
//  Created by Meowthra on 2019/5/24.
//  Copyright © 2019 Meowthra. All rights reserved.
//  Made in Taiwan.

#include "fpins.h"
#include "ssse3_priv.h"
#include <asm/fpu/types.h>

/**********************************************/
/**  IMM8 or MXCSR Rounding Control          **/
/**********************************************/
//#include <math.h>

//#define ROUND_NEAREST     0x0 //Round to nearest even
//#define ROUND_DOWN        0x1 //Round down
//#define ROUND_UP          0x2 //Round up
//#define ROUND_TOWARD_ZERO 0x3 //Truncate

//float:
//f = roundf(f); // 四捨五入 ROUND_NEAREST 0
//f = floorf(f); // 無條件捨去 ROUND_DOWN 1
//f = ceilf(f);  // 無條件進入 ROUND_UP 2
//f = truncf(f); // 去掉小數點 ROUND_TOWARD_ZERO 3

//double:
//df = round(df); // 四捨五入 ROUND_NEAREST 0
//df = floor(df); // 無條件捨去 ROUND_DOWN 1
//df = ceil(df);  // 無條件進入 ROUND_UP 2
//df = trunc(df); // 去掉小數點 ROUND_TOWARD_ZERO 3

int getmxcsr(void) {
    int mxcsr_rc = 0;
    uint32_t mxcsr = 0;
    __asm__ __volatile__ ("stmxcsr %0" : "=m" (mxcsr));
    mxcsr_rc = (mxcsr >> 13) & 3;
    return mxcsr_rc;
}

float round_fp32(float fp32, int rc)
{
	kernel_fpu_begin();
    switch(rc) {
        case 0: fp32 = round_sf(fp32); break;
        case 1: fp32 = floor_sf(fp32); break;
        case 2: fp32 = ceil_sf(fp32); break;
        case 3: fp32 = trunc_sf(fp32); break;
    }
    kernel_fpu_end();
    return fp32;
}

double round_fp64(double fp64, int rc)
{
    switch(rc) {
        case 0: fp64 = round_df(fp64); break;
        case 1: fp64 = floor_df(fp64); break;
        case 2: fp64 = ceil_df(fp64); break;
        case 3: fp64 = trunc_df(fp64); break;
    }
    return fp64;
}


float round_sf(float fp32) {
	kernel_fpu_begin();
    if ( isValidNumber_f32(fp32) ) {
        if ( fp32 > 0 ) {
            fp32 = fp32 + 0.5;
            fp32 = (float)((int32_t)fp32);
        } else {
            fp32 = fp32 - 0.5;
            fp32 = (float)((int32_t)fp32);
        }
    } else {
        fp32 = SNanToQNaN_f32(fp32);
    }
    kernel_fpu_end();
    return fp32;
}

float floor_sf(float fp32) {
	kernel_fpu_begin();
    if ( isValidNumber_f32(fp32) ) {
        if ( fp32 > 0 ) {
            fp32 = (float)((int32_t)fp32);
        } else {
            fp32 = fp32 - 1;
            fp32 = (float)((int32_t)fp32);
        }
    } else {
        fp32 = SNanToQNaN_f32(fp32);
    }
    kernel_fpu_end();
    return fp32;
}

float ceil_sf(float fp32) {
	kernel_fpu_begin();
    if ( isValidNumber_f32(fp32) ) {
        if ( fp32 > 0 ) {
            fp32 = fp32 + 1;
            fp32 = (float)((int32_t)fp32);
        } else {
            fp32 = (float)((int32_t)fp32);
        }
    } else {
        fp32 = SNanToQNaN_f32(fp32);
    }
    kernel_fpu_end();
    return fp32;
}

float trunc_sf(float fp32) {
	kernel_fpu_begin();
    if ( isValidNumber_f32(fp32) ) {
        if ( fp32 > 0 ) {
            fp32 = (float)((int32_t)fp32);
        } else {
            fp32 = (float)((int32_t)fp32);
        }
    } else {
        fp32 = SNanToQNaN_f32(fp32);
    }
    kernel_fpu_end();
    return fp32;
}

float sqrt_sf(float fp32) {
	kernel_fpu_begin();
    int i = 100;
    float a = fp32;  //IN
    float x = a / 2; //OUT
    
    while(i--) {
        x = (x + a / x) / 2;
    }
    
    kernel_fpu_end();
    return x;
}

double round_df(double fp64) {
	kernel_fpu_begin();
    if ( isValidNumber_f64(fp64) ) {
        if ( fp64 > 0 ) {
            fp64 = fp64 + 0.5;
            fp64 = (double)((int64_t)fp64);
        } else {
            fp64 = fp64 - 0.5;
            fp64 = (double)((int64_t)fp64);
        }
    } else {
        fp64 = SNanToQNaN_f64(fp64);
    }
    kernel_fpu_end();
    return fp64;
}

double floor_df(double fp64) {
	kernel_fpu_begin();
    if ( isValidNumber_f64(fp64) ) {
        if ( fp64 > 0 ) {
            fp64 = (double)((int64_t)fp64);
        } else {
            fp64 = fp64 - 1;
            fp64 = (double)((int64_t)fp64);
        }
    } else {
        fp64 = SNanToQNaN_f64(fp64);
    }
    kernel_fpu_end();
    return fp64;
}

double ceil_df(double fp64) {
	kernel_fpu_begin();
    if ( isValidNumber_f64(fp64) ) {
        if ( fp64 > 0 ) {
            fp64 = fp64 + 1;
            fp64 = (double)((int64_t)fp64);
        } else {
            fp64 = (double)((int64_t)fp64);
        }
    } else {
        fp64 = SNanToQNaN_f64(fp64);
    }
    kernel_fpu_end();
    return fp64;
}

double trunc_df(double fp64) {
 	kernel_fpu_begin();
   if ( isValidNumber_f64(fp64) ) {
        if ( fp64 > 0 ) {
            fp64 = (double)((int64_t)fp64);
        } else {
            fp64 = (double)((int64_t)fp64);
        }
    } else {
        fp64 = SNanToQNaN_f64(fp64);
    }
    kernel_fpu_end();
    return fp64;
}

double sqrt_df(double fp64) {
 	kernel_fpu_begin();
   int i = 100;
    double a = fp64;  //IN
    double x = a / 2; //OUT
    
    while(i--) {
        x = (x + a / x) / 2;
    }
    
    kernel_fpu_end();
    return x;
}

/****************************************************/
int isValidNumber_f32(float fp32) {
	kernel_fpu_begin();
    sse_reg_t TMP;
    TMP.fa32[0] = fp32;
    int32_t val = TMP.a32[0];
    
    if ( (val & 0x7F800000) == 0x7F800000 ) {
        return 0;
    }
    
   kernel_fpu_end();
   return 1;
}

int isValidNumber_f64(double fp64) {
	kernel_fpu_begin();
    sse_reg_t TMP;
    TMP.fa64[0] = fp64;
    int64_t val = TMP.a64[0];
    
    if ( (val & 0x7FF0000000000000ll) == 0x7FF0000000000000ll )
    {
        return 0;
    }
    
    kernel_fpu_end();
    return 1;
}

float SNanToQNaN_f32(float fp32) {
	kernel_fpu_begin();
    sse_reg_t TMP;
    int32_t retval;
    
    TMP.fa32[0] = fp32;
    int32_t val = TMP.a32[0];
    
    // Check if the value is already a QNaN
    if ( (val & 0x00400000) != 0x00400000 )
    {
        // Check if the value is + or - infinitie
        if ( (val | 0x7F800000) != 0x7F800000 )
        {
            // Convert SNan To QNaN
            retval = val | 0x00400000;
            TMP.a32[0] = retval;
            fp32 = TMP.fa32[0];
        }
    }
    kernel_fpu_end();
    return fp32;
}

double SNanToQNaN_f64(double fp64) {
	kernel_fpu_begin();
    sse_reg_t TMP;
    int64_t retval;
    
    TMP.fa64[0] = fp64;
    int64_t val = TMP.a64[0];
    
    // Check if the value is already a QNaN
    if ( (val & 0x0008000000000000ll) != 0x0008000000000000ll )
    {
        // Check if the value is + or - infinitie
        if ( (val | 0x7FF0000000000000ll) != 0x7FF0000000000000ll )
        {
            // Convert SNan To QNaN
            retval = val | 0x0008000000000000ll;
            TMP.a64[0] = retval;
            fp64 = TMP.fa64[0];
        }
    }
    return fp64;
}

int isNaN_f64(double fp64) {
    sse_reg_t tmp;
    tmp.fa64[0] = fp64;
    int64_t val = tmp.a64[0];
    
    if ( (val & 0x7FF0000000000000ll) == 0x7FF0000000000000ll ) {
        if( (val & 0xFFFFFFFFFFFFF) != 0) {
            return 1;
        }
    }
    
    return 0;
}
int isNaN_f32(float fp32) {
 	kernel_fpu_begin();
   sse_reg_t tmp;
    tmp.fa32[0] = fp32;
    int32_t val = tmp.a32[0];
    
    if ( (val & 0x7F800000) == 0x7F800000 ) {
        if( (val & 0x7FFFFF) != 0) {
            return 1;
        }
    }
    
    kernel_fpu_end();
    return 0;
}

