/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_GenericSkinnedButton.h - general purpose button for gui components
 *
 **/

namespace ME_Framework
{
	namespace ME_XGUI
	{
		class XGUI_GenericButton: public XGUI_Widget
		{		
			int m_nUsedSkins;
			mSheetGlyph_t * m_pSkinsSet[3];
			
			int m_iButtonID;
					
			bool m_bPressed;

		public:
			XGUI_GenericButton(xgRect_t & r);
			
			virtual void DrawComponent();

			void SetButtonID(int id);
			int GetButotnID();

			void SetSkinSet(mSheetGlyph_t ** pSpritesSet,int sprCount);
			virtual void HandleEvent(ME_Framework::appEvent_t & e);
		};
	}
}