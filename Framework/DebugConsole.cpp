/*
 *	Molotok Editor, 2013-2014 (C) PrinceOfDispersia
 *	DebugConsole.cpp - console for debugging
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_Console;
using namespace ME_Framework::ME_OpenGLBackend;
using namespace ME_Framework::ME_XGUI;

extern XGUI_Tesselator * g_pTesselator;
extern XGUI_Manager * g_pGUIManager;

#pragma warning(disable:4267)

XGUI_Font * g_pConsoleFont;

/*
 *	Console data structure
 **/
typedef struct conState_s
{
	std::vector<convar_s> m_vVarsAndProcs;
	TCHAR m_strConBuffer[32768];
	size_t m_conBufferOffset;

	bool m_bVisible;

	TCHAR m_strConPrompt;
	TCHAR m_strInputBuffer[1024];
	size_t m_InputCarretOffset;

}conState_t;

conState_t g_Console;

/*
 *	Draws console
 **/
void ME_Console::Draw()
{
	if (!g_Console.m_bVisible) return;

	// Background

	ME_Math::Vector2D ext = g_pPlatform->GetClientAreaExtents();
	
	g_pTesselator->SetTranslation(ME_Math::Vector2D(0,0));
	color32_t c = {0,0,0,192};
	g_pTesselator->DefaultColor(c);
	
	xgRect_t r;
	r.pos = ME_Math::Vector2D(0,0);
	r.ext = ext;
	r.ext.y /= 2;
	
	XGUI_DrawSheetGlyph(sprWhite,r);

	g_pTesselator->ResetDefaultColor();

	xgRect_t r2 = r;
	r2.pos.x = 2;
	r2.ext.x -= 4;
	r2.ext.y -= 10;
	g_pConsoleFont->DrawMultilineTextInRect(r2,g_Console.m_strConBuffer);

	r.pos.y = (ext.y / 2) - 4;
	r.ext.y = 4;

	color32_t c2 = {255,0,0,255};
	g_pTesselator->DefaultColor(c2);

	XGUI_DrawSheetGlyph(sprWhite,r);
	
	g_pTesselator->ResetDefaultColor();	
	g_pConsoleFont->DrawTextWithCarret(2,ext.y / 2 - 20,&g_Console.m_strConPrompt,g_Console.m_InputCarretOffset+1);
	
}

/*
 *	Console event handler
 **/
bool ME_Console::HandleEvent(ME_Framework::appEvent_t & ev)
{
	if (ev.eventid != eventTypes::EV_KB_KEY_DOWN && ev.eventid != eventTypes::EV_KB_CHAR) return false;

	unsigned int key = ev.uParam1;

	if (key == KBD_CONSOLE)
	{
		g_Console.m_bVisible = !g_Console.m_bVisible;
	}

	if (!g_Console.m_bVisible) 
		return false;

	if (ev.eventid == eventTypes::EV_KB_CHAR)
	{
		if (g_Console.m_InputCarretOffset < (ARRAY_SIZE(g_Console.m_strInputBuffer)-1) && ev.uParam1 >= 32 && ev.uParam1 != '`')
		{
			size_t endOfs = _tcslen(g_Console.m_strInputBuffer);

			for(int i = endOfs+1; i >= g_Console.m_InputCarretOffset && i>=0 ; i--)
			{
				g_Console.m_strInputBuffer[i+1] = g_Console.m_strInputBuffer[i];
			}

			g_Console.m_strInputBuffer[g_Console.m_InputCarretOffset] = ev.uParam1;
			g_Console.m_InputCarretOffset++;
		}		
	}

	if (ev.eventid == eventTypes::EV_KB_KEY_DOWN && ev.uParam1 == KBD_LEFT)
	{
		if (g_Console.m_InputCarretOffset > 0)
			g_Console.m_InputCarretOffset--;
	}

	if (ev.eventid == eventTypes::EV_KB_KEY_DOWN && ev.uParam1 == KBD_RIGHT)
	{
		if (g_Console.m_InputCarretOffset < ARRAY_SIZE(g_Console.m_strInputBuffer) 
						&& g_Console.m_strInputBuffer[g_Console.m_InputCarretOffset])
			g_Console.m_InputCarretOffset++;
	}

	if (ev.eventid == eventTypes::EV_KB_KEY_DOWN && ev.uParam1 == KBD_BACKSPACE)
	{
		if (g_Console.m_InputCarretOffset == 0) 
			return true;
				
		g_Console.m_InputCarretOffset--;

		for(size_t i = (g_Console.m_InputCarretOffset+1) ; i < ARRAY_SIZE(g_Console.m_strInputBuffer) ; i++)
		{
			g_Console.m_strInputBuffer[i-1] = g_Console.m_strInputBuffer[i];
		}
	}

	if (ev.eventid == eventTypes::EV_KB_KEY_DOWN && ev.uParam1 == KBD_DELETE)
	{
		for(size_t i = (g_Console.m_InputCarretOffset+1) ; i < ARRAY_SIZE(g_Console.m_strInputBuffer) ; i++)
		{
			g_Console.m_strInputBuffer[i-1] = g_Console.m_strInputBuffer[i];
		}
	}

	if (ev.eventid == eventTypes::EV_KB_KEY_DOWN && ev.uParam1 == KBD_HOME)
	{
		g_Console.m_InputCarretOffset = 0;
	}

	if (ev.eventid == eventTypes::EV_KB_KEY_DOWN && ev.uParam1 == KBD_END)
	{
		g_Console.m_InputCarretOffset = _tcslen(g_Console.m_strInputBuffer);
	}

	if (ev.eventid == eventTypes::EV_KB_KEY_DOWN && ev.uParam1 == KBD_ENTER)
		HandleEnter();

	return true;
}

/*
 *	Enter handler
 **/
void ME_Console::HandleEnter()
{
	Printf(_T("%s\n"),g_Console.m_strInputBuffer);

	g_Console.m_InputCarretOffset = 0;
	*g_Console.m_strInputBuffer = 0;
}

/*
 *	Console initializier
 **/
void ME_Console::Start()
{
	g_Console.m_vVarsAndProcs.clear();
	g_Console.m_InputCarretOffset = 0;
	g_Console.m_conBufferOffset = 0;
	*g_Console.m_strInputBuffer = 0;
	g_Console.m_strConPrompt = _T('>');
	memset(g_Console.m_strInputBuffer,0,sizeof(g_Console.m_strInputBuffer));
}


/*
 *	Graphics loader
 **/
void ME_Console::LoadGraphics()
{
	g_pConsoleFont = g_pGUIManager->Get_GuiFont(TGuiFontTypes::gfNormal);
}

/*
 *	Console destructor
 **/
void ME_Console::Stop()
{
	g_Console.m_vVarsAndProcs.clear();
	g_Console.m_vVarsAndProcs.shrink_to_fit();
}

/*
 *	Console printf routine
 **/
void ME_Console::Printf(TCHAR* format,...)
{
	va_list		argptr;
		
	TCHAR s[1024];
	size_t destSize = 1024;
	
	va_start( argptr, format );
	int written = _vsntprintf( s, destSize, format, argptr );
	va_end( argptr );

	if (written == -1)
	{
		// null-terminate
		s[destSize-1] = 0;
	}
	
	size_t takenSpace = (written == -1 ? destSize : written);
	size_t writeOffset = g_Console.m_conBufferOffset;

	if ((g_Console.m_conBufferOffset + takenSpace + 1) > ARRAY_SIZE(g_Console.m_strConBuffer))
	{
		size_t offset = 0;
		for(int i = (takenSpace) ; i < ARRAY_SIZE(g_Console.m_strConBuffer)-1 ; i++)
		{
			if (!g_Console.m_strConBuffer[i]) break;
			g_Console.m_strConBuffer[offset] = g_Console.m_strConBuffer[i];
			offset++;
		}

		writeOffset = offset;
	}

	for(size_t i = 0 ; i < takenSpace ; i++)
	{
		g_Console.m_strConBuffer[i + writeOffset] = s[i];
	}

	g_Console.m_strConBuffer[writeOffset+takenSpace] = 0;
	g_Console.m_conBufferOffset = g_Console.m_conBufferOffset + takenSpace;

	if (g_Console.m_conBufferOffset > ARRAY_SIZE(g_Console.m_strConBuffer)) 
		g_Console.m_conBufferOffset = ARRAY_SIZE(g_Console.m_strConBuffer);
}