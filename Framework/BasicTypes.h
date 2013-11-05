/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	BasicTypes.h - basic types definitions
 *
 **/

// Byte definition
//
typedef unsigned char byte;

// Colors
//
typedef struct color32_s
{
	byte r,g,b,a;
}color32_t;

typedef struct color24_s 
{
	byte r,g,b;
}color24_t;

#ifdef VECTOR_DOUBLE
	typedef double vec_t;
#else
	typedef float vec_t;
#endif