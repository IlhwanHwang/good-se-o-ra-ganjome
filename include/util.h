#ifndef UTIL
#define UTIL

#define CONST_PIDOUBLE 6.2831853072
#define CONST_PI 3.1415629535
#define CONST_PIHALF 1.570796326

#define FLOAT2 ((void *)0x40000000)
#define FLOAT3 ((void *)0x40400000)
#define FLOAT4 ((void *)0x40800000)
#define FLOAT6 ((void *)0x40c00000)
#define FLOAT8 ((void *)0x41000000)
#define FLOATNEG1 ((void *)0xbf800000)
#define FLOATNEG2 ((void *)0xc0000000)
#define FLOATNEG3 ((void *)0xc0400000)
#define FLOATNEG4 ((void *)0xc0800000)
#define FLOATNEG8 ((void *)0xc1000000)
#define FLOAT45RAD ((void *)0x3f490fd8)
#define FLOAT315RAD ((void *)0xbf490fd8)
#define FLOAT225RAD ((void *)0x407b53ce)
#define FLOAT135RAD ((void *)0x4016cbd1)

int clamp(int x, int a, int b);
float fclamp(float x, float a, float b);
float flerp(float a, float b, float x);
void swap(int *x, int *y);
void order(int *x, int *y);

inline int absi(int x);

float floor(float x);
float mod(float x, float f);

float Q_rsqrt(float number);
float sqrt(float x);

float sin(float x);
float cos(float x);

#endif