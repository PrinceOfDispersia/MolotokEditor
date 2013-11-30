/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	XGUI_Manager.h - GUI Manager
*
**/

#ifndef XGUI_MANAGER_H
#define XGUI_MANAGER_H

namespace ME_Framework
{
	namespace ME_XGUI
	{
		typedef struct guiSettings_s
		{
			color32_t m_cDesktopBG;
		}guiSettings_t;

		class XGUI_Manager
		{
			XGUI_Widget * m_pDesktop;
			XGUI_Sheet  * m_pImagesSheet;
			XGUI_Font * m_pGuiFont;

			guiSettings_t m_GuiSettings;

			CConfigVarsManager* m_pGuiVars;
						
			void LoadVars();
			void SaveVars();

			bool m_bInEditorMode;

		public:
			bool IsInEditorMode() { return m_bInEditorMode; }

			guiSettings_t * GuiSettings() { return &m_GuiSettings; }
			mSheetGlyph_t * GetGUISheetGlyph(char * szName);

			void Draw();
			void Think(float flTimeDelta);

			void HandleEvent(ME_Framework::appEvent_t & ev);

			XGUI_Manager();
			~XGUI_Manager();

			void AddWidget(XGUI_Widget * pWidget);
		};

		void XGUI_DrawSheetGlyph(mSheetGlyph_t * pGlyph,xgRect_t & r);
		void XGUI_DrawScalableSheetGlyph(mSheetGlyph_t * pGlyphs[9],xgRect_t & r);
		void XGUI_DrawRect(xgRect_t & r);
	}
}




#endif