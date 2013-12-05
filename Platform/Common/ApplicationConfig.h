/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	ApplicationConfig.h - application configuration header
 *
 **/

#ifndef APPLICATION_CONFIG_H
#define APPLICATION_CONFIG_H

// uses double precision vectors
#define VECTOR_DOUBLE

#ifdef VECTOR_DOUBLE
#define GL_VECT GL_DOUBLE
#else
#define GL_VECT GL_FLOAT
#endif

// paranoid mode - apply additional checks
#define PARANOID

// maximum number of command line arguments
#define MAX_COMMAND_LINE_ARGUMENTS 256

#endif