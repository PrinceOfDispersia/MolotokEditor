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
#define CON_FONT_HEIGHT 10

XGUI_Font * g_pConsoleFont;

class Con_AutoCompleteHelper;

/*
 *	Console data structure
 **/
typedef struct conState_s
{
	std::vector<convar_s*> m_vVarsAndProcs;
	TCHAR m_strConBuffer[32768];
	size_t m_conBufferOffset;

	bool m_bVisible;

	TCHAR m_strConPrompt;
	TCHAR m_strInputBuffer[1024];
	size_t m_InputCarretOffset;
	
	Con_AutoCompleteHelper* m_AutoCompleteHelper;

}conState_t;

conState_t g_Console;

typedef struct acItem_s
{
	TCHAR * strPtr;
	convar_t * var;

	TCHAR repBuffer[256];

	TCHAR * Representation()
	{
		if (var)
		{
			
			Sys_SafeSprintf(repBuffer,ARRAY_SIZE(repBuffer),(var->isCommand ? _T("%s") : _T("%s = %s")),var->strName,var->strValue);
			return repBuffer;
		}

		return strPtr;
	}

	TCHAR * Value()
	{
		if (var)
		{
			return var->strName;
		}

		return strPtr;
	}
}acItem_t;

class Con_AutoCompleteHelper
{
	std::vector<acItem_t> m_vSuggestionsList;
	TCHAR m_strToHighlight[1024];
	int m_SelectedItem;

	TCHAR m_strHistory[2][1024];
	size_t m_HistoryDepth;

public:

	Con_AutoCompleteHelper()
	{
		m_vSuggestionsList.clear();
		m_SelectedItem = 0;
		m_HistoryDepth = 0;
	}

	~Con_AutoCompleteHelper()
	{
		m_vSuggestionsList.clear();
		m_vSuggestionsList.shrink_to_fit();
	}

	void Draw()
	{		
		if (m_vSuggestionsList.size() == 0)
			return;

		ME_Math::Vector2D ext = g_pPlatform->GetClientAreaExtents();

		color32_t c = {0,0,0,192};

		g_pTesselator->SetTranslation(ME_Math::Vector2D(0,0));
		g_pTesselator->DefaultColor(c);

		xgRect_t r;
		r.pos = ME_Math::Vector2D(4,ext.y / 2);
		
		r.ext.Init();

		for(size_t i = 0 ; i < m_vSuggestionsList.size() ; i++)
		{
			xgRect_t tr;
			g_pConsoleFont->Calc_TextRect(ME_Framework::String(m_vSuggestionsList[i].Representation()),&tr);
			
			auto v = ME_Math::Vector2D(r.pos.x,r.pos.y + (i * CON_FONT_HEIGHT));

			tr.pos += v;

			if (r.ext.x < tr.Right()) r.ext.x = tr.Right() - r.pos.x;

			r.ext.y += (CON_FONT_HEIGHT+1);
		}

		r.ext.x *= 1.5;

		XGUI_DrawSheetGlyph(sprWhite,r);
		g_pTesselator->ResetDefaultColor();

		for(size_t i = 0 ; i < m_vSuggestionsList.size() ; i++)
		{
			if (i == m_SelectedItem)
			{
				g_pConsoleFont->SetTextColor(255,0,0,255);
			}

			g_pConsoleFont->Draw(ME_Math::Vector2D(r.pos.x,r.pos.y + (i * CON_FONT_HEIGHT)),m_vSuggestionsList[i].Representation());
		}
	}

	void UpdateList(TCHAR * strInputBuffer)
	{	
		m_vSuggestionsList.clear();
		if (*strInputBuffer)
		{
			
			for(auto v : g_Console.m_vVarsAndProcs)
			{
				if (_tcsstr(v->strName,strInputBuffer))
				{
					acItem_t i;
					i.var = v;
					i.strPtr = 0;

					m_vSuggestionsList.push_back(i);
				}
			}
		}
		else
		{
			for(size_t i = 0 ; i < m_HistoryDepth ; i++)
			{
				acItem_t item;
				item.var = 0;
				item.strPtr = m_strHistory[i];

				m_vSuggestionsList.push_back(item);
			}
		}
		
	}

	bool HandleEvent(ME_Framework::appEvent_t & ev)
	{
		if (ev.eventid == eventTypes::EV_KB_KEY_DOWN)
		{
			switch(ev.uParam1)
			{
			case KBD_DOWN:

				m_SelectedItem++;
				if (m_SelectedItem == m_vSuggestionsList.size())
					m_SelectedItem--;

				return true;
			case KBD_UP:
				m_SelectedItem--;
				if (m_SelectedItem < 0)
					m_SelectedItem = 0;
				return true;
			case KBD_ENTER:
				if (m_vSuggestionsList.size() > 0 && _tcscmp(g_Console.m_strInputBuffer,m_vSuggestionsList[m_SelectedItem].Value()))
				{
					Sys_SafeStrCopy(g_Console.m_strInputBuffer,m_vSuggestionsList[m_SelectedItem].Value(),ARRAY_SIZE(g_Console.m_strInputBuffer));
					g_Console.m_InputCarretOffset = _tcsclen(g_Console.m_strInputBuffer);
					g_Console.m_strInputBuffer[g_Console.m_InputCarretOffset] = _T(' ');
					g_Console.m_InputCarretOffset++;

					assert(g_Console.m_InputCarretOffset < ARRAY_SIZE(g_Console.m_strInputBuffer));

					m_SelectedItem = 0;
					m_vSuggestionsList.clear();
					return true;
				}
				return false;
			}
		}

		return false;
	}

	void AddToHistory(TCHAR * strCmd)
	{
		if (m_HistoryDepth == ARRAY_SIZE(m_strHistory))
		{
			m_HistoryDepth--;
			memmove(m_strHistory[0],m_strHistory[1],sizeof(m_strHistory[0]) * (ARRAY_SIZE(m_strHistory) - 1));
		}

		Sys_SafeStrCopy(m_strHistory[m_HistoryDepth],strCmd,ARRAY_SIZE(m_strHistory[0]));
		m_HistoryDepth++;
	}
};


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
	
	g_Console.m_AutoCompleteHelper->Draw();
}

/*
 *	Console event handler
 **/
bool ME_Console::HandleEvent(ME_Framework::appEvent_t & ev)
{
	

	if (ev.eventid != eventTypes::EV_KB_KEY_DOWN && ev.eventid != eventTypes::EV_KB_CHAR) return false;

	if (g_Console.m_AutoCompleteHelper->HandleEvent(ev)) 
		return true;
	
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

			for(int i = endOfs+1; i >= g_Console.m_InputCarretOffset && i>=0; i--)
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

	g_Console.m_AutoCompleteHelper->UpdateList(g_Console.m_strInputBuffer);

	if (ev.eventid == eventTypes::EV_KB_KEY_DOWN && ev.uParam1 == KBD_ENTER)
		HandleEnter();

	return true;
}

/*
 *	Enter handler
 **/
void ME_Console::HandleEnter()
{
	bool bHandled = false;

	TCHAR* args[16];
	memset(&args,0,sizeof(args));

	g_Console.m_AutoCompleteHelper->AddToHistory(g_Console.m_strInputBuffer);

	size_t c = Sys_DisassembleStringToTokenArray(g_Console.m_strInputBuffer,_T(' '),args,16);


	for(auto v: g_Console.m_vVarsAndProcs)
	{
		if (!_tcscmp(v->strName,g_Console.m_strInputBuffer))
		{
			if (v->isCommand)
			{
				v->ExecutionHandler(c,args);				
			}
			else
			{
				if (c == 1)	
				{
					Printf(_T("%s = %s\n"),v->strName,v->strValue);
				}
				else if (c >= 2 && *args[1])
				{
					Printf(_T("%s set to %s\n"),v->strName,args[1]);

					#pragma warning(disable:4244)
					v->flValue = _ttof(args[1]);
					v->intValue = _ttoi(args[1]);
					
					Sys_SafeStrCopy(v->strValue,args[1],ARRAY_SIZE(v->strValue));
					
				}
				else Printf(_T("%s = %s\n"),v->strName,v->strValue);
			}
			bHandled = true;
			break;
		}
	}

	if (!bHandled) 
		Printf(_T("Unknown command: \"%s\" !\n"),g_Console.m_strInputBuffer);

	

	g_Console.m_InputCarretOffset = 0;
	*g_Console.m_strInputBuffer = 0;
}

void Con_Help(size_t c,TCHAR * args[])
{
	for(auto v : g_Console.m_vVarsAndProcs)
	{
		Printf(_T("%s:\t%s\t%s\n"),v->strName,(v->isCommand ? _T("command") : _T("variable")),v->strDescription);
	}
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
		

	g_Console.m_AutoCompleteHelper = new Con_AutoCompleteHelper;
}


/*
 *	Graphics loader
 **/
void ME_Console::LoadGraphics()
{
	g_pConsoleFont = g_pGUIManager->Get_GuiFont(TGuiFontTypes::gfNormal);

	RegisterConsoleObject(_T("help"),_T("displays this list"),Con_Help);

	RegisterConsoleObject(_T("r_test1"),_T("Debug variable #1"),_T("0"));
	RegisterConsoleObject(_T("r_test2"),_T("Debug variable #2"),_T("0"));
	RegisterConsoleObject(_T("r_test3"),_T("Debug variable #3"),_T("0"));
	RegisterConsoleObject(_T("r_test4"),_T("Debug variable #4"),_T("0"));
}

/*
 *	Console destructor
 **/
void ME_Console::Stop()
{
	for(auto v : g_Console.m_vVarsAndProcs)
	{
		g_pPlatform->MemoryPools()->Free(v);
	}

	g_Console.m_vVarsAndProcs.clear();
	g_Console.m_vVarsAndProcs.shrink_to_fit();

	delete g_Console.m_AutoCompleteHelper;
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
	
	FILE * fp = fopen("system.log","a+t");
	_ftprintf(fp,_T("%s\n"),s);
	fclose(fp);

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

//************************************
// Method:    RegisterConsoleObject
// FullName:  ME_Framework::ME_Console::RegisterConsoleObject
// Access:    public 
// Returns:   convar_t*
// Qualifier:
// Parameter: TCHAR * strName
// Parameter: TCHAR * strDescription
// Parameter: TCHAR * defaultValue
//************************************
convar_t* ME_Console::RegisterConsoleObject(TCHAR * strName,TCHAR * strDescription,TCHAR * defaultValue)
{
	convar_t * v = (convar_t *)g_pPlatform->MemoryPools()->Alloc(sizeof(convar_t));
	
	v->ExecutionHandler = 0;
	v->flValue = _ttof(defaultValue);
	v->intValue = _ttoi(defaultValue);
	Sys_SafeStrCopy(v->strValue,defaultValue,ARRAY_SIZE(v->strValue));
	v->isCommand = false;
	
	Sys_SafeStrCopy(v->strName,strName,ARRAY_SIZE(v->strName));
	Sys_SafeStrCopy(v->strDescription,strDescription,ARRAY_SIZE(v->strDescription));	
	Sys_SafeStrCopy(v->strDefaultValue,defaultValue,ARRAY_SIZE(v->strDefaultValue));
	
	g_Console.m_vVarsAndProcs.push_back(v);

	return v;
}

//************************************
// Method:    RegisterConsoleObject
// FullName:  RegisterConsoleObject
// Access:    public 
// Returns:   convar_t*
// Qualifier:
// Parameter: TCHAR * strName
// Parameter: TCHAR * strDescription
// Parameter: void 
// Parameter: * Handler
// Parameter: 
//************************************
convar_t* ME_Console::RegisterConsoleObject(TCHAR * strName,TCHAR * strDescription,void (*Handler)(size_t argc,TCHAR * argv[]))
{
	convar_t * v = (convar_t *)g_pPlatform->MemoryPools()->Alloc(sizeof(convar_t));

	v->ExecutionHandler = 0;
	v->ExecutionHandler = Handler;
	v->isCommand = true;

	Sys_SafeStrCopy(v->strName,strName,ARRAY_SIZE(v->strName));
	Sys_SafeStrCopy(v->strDescription,strDescription,ARRAY_SIZE(v->strDescription));

	g_Console.m_vVarsAndProcs.push_back(v);

	return v;
}


//************************************
// Method:    Cvar_GetFloat
// FullName:  ME_Framework::ME_Console::Cvar_GetFloat
// Access:    public 
// Returns:   float
// Qualifier:
// Parameter: TCHAR * strName
//************************************
float ME_Console::Cvar_GetFloat(TCHAR * strName)
{
	for(auto v: g_Console.m_vVarsAndProcs)
	{
		if (!_tcscmp(v->strName,strName))
			return v->flValue;
	}

	return 0;
}