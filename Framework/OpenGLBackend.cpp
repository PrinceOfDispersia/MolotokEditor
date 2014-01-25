/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	OpenGLBackend.cpp - OpenGL backend classes
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

namespace ME_Framework
{
	namespace ME_OpenGLBackend
	{

		typedef struct oglState_s
		{
			bool bLocked;
			bool bEnabled;
			GLenum glState;
		}oglState_t;

		oglState_t g_stateTable[] = 
		{
			{false,false,GL_BLEND},
			{false,false,GL_TEXTURE_2D},
			{false,false,GL_DEPTH_TEST},
		};

		typedef struct oglScissorState_s
		{
			vec_t l,t,r,b;
			bool enabled;
		}oglScissorState_t;

		oglScissorState_t g_ScissorState;

		void GL_EnableScissorTest()
		{
			if (g_ScissorState.enabled) return;

			glEnable(GL_SCISSOR_TEST);
			g_ScissorState.enabled = true;
		}
		
		void GL_DisableScissorTest()
		{
			if (!g_ScissorState.enabled) return;

			glDisable(GL_SCISSOR_TEST);
			g_ScissorState.enabled = false;
		}

		void GL_SetScissor(vec_t l,vec_t t,vec_t w,vec_t h)
		{
			auto ext = g_pPlatform->GetClientAreaExtents();
			glScissor(l,ext.y - t - h,w,h);
		}

		void GL_SynchBackend()
		{
			for(int i = 0 ; i < ARRAY_SIZE(g_stateTable); i++)
			{
				g_stateTable[i].bEnabled = (glIsEnabled(g_stateTable[i].glState) == GL_TRUE);
			}

			g_ScissorState.enabled = glIsEnabled(GL_SCISSOR_TEST) == GL_TRUE;
		}

		void GL_EnableState(GLStateList state)
		{
			if (g_stateTable[state].bLocked) return;
			if (g_stateTable[state].bEnabled) return;

			glEnable(g_stateTable[state].glState);
			g_stateTable[state].bEnabled = true;
		}

		void GL_DisableState(GLStateList state)
		{
			if (g_stateTable[state].bLocked) return;
			if (g_stateTable[state].bEnabled == false) return;

			glDisable(g_stateTable[state].glState);
			g_stateTable[state].bEnabled = false;
		}

		void GL_StateLock(GLStateList state,bool lock)
		{
			g_stateTable[state].bLocked = lock;
		}

	}
}