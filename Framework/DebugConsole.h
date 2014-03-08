/*
 *	Molotok Editor, 2013-2014 (C) PrinceOfDispersia
 *	DebugConsole.h - console for debugging
 *
 **/


namespace ME_Framework
{
	namespace ME_Console
	{
		void LoadGraphics();
		void Start();
		void Stop();

		typedef struct convar_s 
		{
			TCHAR strName[256];
			TCHAR strValue[256];
			TCHAR strDefaultValue[256];
			TCHAR strDescription[256];

			void (*ExecutionHandler)(size_t argc,TCHAR * argv[]);

			int intValue;
			float flValue;

			bool isCommand;
		}convar_t;

		void Draw();		
		bool HandleEvent(ME_Framework::appEvent_t & ev);
		void HandleEnter();

		void Printf(TCHAR* fmt,...);

		convar_t* RegisterConsoleObject(TCHAR * strName,TCHAR * strDescription,void (*Handler)(size_t argc,TCHAR * argv[]));
		convar_t* RegisterConsoleObject(TCHAR * strName,TCHAR * strDescription,TCHAR * defaultValue);
	}
}

