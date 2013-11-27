/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Framework.h - basic include file of framework
 *
 **/

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// Include math library
//
#include "MathLib/Math.h"

// String support
//
#include "String.h"

// Image library
//
#include "ImageLib/ImageLib.h"

// OpenGL backend
//
#include "OpenGLBackend.h"

// TinyXML
//

// Fix my memanager and tinyxml collisions
#undef Alloc
#undef Free

#include <Libs/tinyxml/tinyxml2.h>

#define Alloc(n) Alloc(n,_T(__FILE__),__LINE__)
#define Free(ptr) Free(ptr,_T(__FILE__),__LINE__)

// Config vars
//
#include "ConfigVars.h"


#endif