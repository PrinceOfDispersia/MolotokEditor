/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Widget.h - basic gui widget class
 *
 **/

namespace ME_Framework
{

namespace ME_XGUI
{
	class XGUI_Manager;

	class XGUI_Widget
	{
		friend XGUI_Manager;
	protected:
		xgRect_t m_Rect;
		color32_t m_Color;
		mSheetGlyph_t * m_pImage[16];
		unsigned int m_ZOrder;

		double m_flTimers[4]; 

		std::vector<XGUI_Widget*> m_vChilds;
		XGUI_Widget * m_pParent;

		TAlign m_Align;
		TAnchor m_Anchors; 

		bool m_bVisible;
		bool m_bEnabled;

		String m_strCaption;
		String m_strHint;

		void SortChilds();
		void CalcClientRect(xgRect_t & r);

		float m_flHoveroutTimer;
		

		XGUI_Font * m_pGuiFont;
		

	public:

		void AddChildWidget(XGUI_Widget * pWidget);

		// Setters
		void SetParent(XGUI_Widget * pParent);
		void SetAlign(TAlign align);
		void SetAnchors(TAnchor anchors);
		void SetVisibilty(bool bVisible);
		void SetEnableState(bool bEnabled);
		void SetCaption(String & str);

		// Getters
		XGUI_Widget *	GetParent();
		TAlign			GetAlign();
		TAnchor			GetAnchors();
		bool			GetVisibilty();
		bool			GetEnableState();
		String &		GetCaption();
		
		virtual void HandleEvent(ME_Framework::appEvent_t & pEvent);
		virtual void DrawComponent();

		void Render();		
		void UpdateTimers(float flDelta);
		void SetSize(xgRect_t * rect);

		XGUI_Widget* WidgetUnderCursor(ME_Math::Vector2D pt);
			

		XGUI_Widget(xgRect_t * rect);
		virtual ~XGUI_Widget();
	};
}

}