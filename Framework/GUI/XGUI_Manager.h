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


		class XGUI_Tesselator
		{
			ME_Math::Vector2D * m_pVertexes;
			ME_Math::Vector2D * m_pUVMapping;
			color32_t * m_pColors;
			int m_nElements;
			int m_nUsedElements;

			color32_t m_cDefault;

			ME_Math::Vector2D m_vTranslation;
			
		public:
			XGUI_Tesselator(int elementsCount);
			~XGUI_Tesselator();

			void Coord2v(ME_Math::Vector2D & uv);
			void Coord2(float u,float v);
			void Color32(color32_t & c);
			void Vertex2v(ME_Math::Vector2D & vec);
			void Vertex2(vec_t x,vec_t y);

			void Coord2a(ME_Math::Vector2D * pUV,int count);
			void Color32a(color32_t  * c,int count);
			void Vertex2a(ME_Math::Vector2D * pVecs,int count);

			void DefaultColor(color32_t c);
			inline void ResetDefaultColor() { m_cDefault.r = m_cDefault.g = m_cDefault.b = m_cDefault.a = 255;}

			void SetTranslation(ME_Math::Vector2D & vOrigin);

			void Flush();
		};

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

			XGUI_Tesselator * m_pTesselator;
			pgl_texture_t m_GuiAtlas;
		public:
			bool IsInEditorMode() { return m_bInEditorMode; }

			inline XGUI_Tesselator * Tesselator();

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
		void XGUI_DrawRectOutline(xgRect_t & r);
	}
}




#endif