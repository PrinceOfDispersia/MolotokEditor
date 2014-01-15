/*
 *	Molotok Editor, 2013-2014 (C) PrinceOfDispersia
 *	DebugConsole.h - console for debugging
 *
 **/


namespace ME_Framework
{
	namespace ME_Console
	{
		void Start();
		void Stop();

		typedef struct convar_s 
		{
			char strName[256];
			char strDefaultValue[256];
			char strDescription[256];

			void (*ExecutionHandler)();

			int intValue;
			float flValue;

			bool isCommand;
		}convar_t;

		void Draw();		
		bool HandleEvent(ME_Framework::appEvent_t & ev);

		void Printf(TCHAR* fmt,...);
	}
}

