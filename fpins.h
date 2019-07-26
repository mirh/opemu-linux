//
//  fpins.h
//  opemu
//
//  Created by Meowthra on 2019/5/24.
//  Copyright © 2019 Meowthra. All rights reserved.
//  Made in Taiwan.

#ifndef fpins_h
#define fpins_h

#include "opemu.h"
#include "ssse3_priv.h"

int getmxcsr(void);

float round_fp32(float fp32, int rc);
double round_fp64(double fp64, int rc);

float round_sf(float fp32);
float floor_sf(float fp32);
float ceil_sf(float fp32);
float trunc_sf(float fp32);
float sqrt_sf(float fp32);

double round_df(double fp64);
double floor_df(double fp64);
double ceil_df(double fp64);
double trunc_df(double fp64);
double sqrt_df(double fp64);

int isValidNumber_f32(float fp32);
int isValidNumber_f64(double fp64);

float SNanToQNaN_f32(float fp32);
double SNanToQNaN_f64(double fp64);

int isNaN_f64(double fp64);
int isNaN_f32(float fp32);


#endif /* fpins_h */
