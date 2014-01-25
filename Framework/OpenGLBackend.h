/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	OpenGLBackend.h - OpenGL frontend classes
 *
 **/

#ifndef OPENGLBACKEND_H
#define OPENGLBACKEND_H

namespace ME_Framework
{
	namespace ME_OpenGLBackend
	{
		enum GLStateList
		{
			GLS_BLEND,
			GLS_TEXTURE_2D,
			GLS_DEPTH_TEST
		};

		void GL_SynchBackend();

		void GL_EnableState(GLStateList state);
		void GL_DisableState(GLStateList state);
		void GL_StateLock(GLStateList state,bool lock);

		void GL_EnableScissorTest();
		void GL_DisableScissorTest();
		void GL_SetScissor(vec_t l,vec_t t,vec_t w,vec_t h);

	}
}

#endif