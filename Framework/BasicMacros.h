/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	BasicMacros.h - basic macros defs
 *
 **/

#ifndef BASICMACROS_H
#define BASICMACROS_H

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#ifndef min
	#define min(a,b) ((a) > (b) ? (b) : (a))
#endif 

#ifndef max
	#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#define clamp(x,a,b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

#endif