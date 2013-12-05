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
			
		float m_flTimers[4];
		std::vector<XGUI_Widget*> m_vChilds;
		std::vector<XGUI_Widget*> m_vAlignOrderedChilds;
		
		XGUI_Widget * m_pParent;

		int m_nWidgetNumber;
		int m_nWidgetCounter;

		int m_AlignPriority;
		TAlign m_Align;
		TAnchor m_Anchors; 

		bool m_bVisible;
		bool m_bEnabled;

		String m_strCaption;
		String m_strHint;

		void SortChilds();
		void SortChildsByAlignOrder();

		void CalcClientRect(xgRect_t & r);
		void PointToClient(ME_Math::Vector2D & v);

		float m_flHoveroutTimer;
		
		XGUI_Font * m_pGuiFont;
		
		ME_Math::Vector2D m_vDragOrigin;
		bool m_bDragged;

		ME_Math::Vector2D m_vParentStart;

		void RecalcRectWithAligment();
		xgRect_t GetParentRect();

		void RecalcDrag();
		void RecursiveNotifyEveryone(ME_Framework::appEvent_t & ev);

	public:

		void AddChildWidget(XGUI_Widget * pWidget);

		// Setters
		void SetParent(XGUI_Widget * pParent);
		void SetAlign(TAlign align);
		void SetAlignPriority(int alignPriority);
		void SetAnchors(TAnchor anchors);
		void SetVisibilty(bool bVisible);
		void SetEnableState(bool bEnabled);
		void SetCaption(String & str);
		void SetZOrder(int order);

		// Getters
		XGUI_Widget *	GetParent();
		TAlign			GetAlign();
		TAnchor			GetAnchors();
		bool			GetVisibilty();
		bool			GetEnableState();
		String &		GetCaption();
		int				GetAlignPriority();
		int				GetZOrder();

		virtual void HandleEvent(ME_Framework::appEvent_t & pEvent);
		virtual void DrawComponent();

		void Render();		
		void UpdateTimers(float flDelta);
		void SetRect(xgRect_t & rect);
		void RecalcItemsRects();

		void DoThink();
		virtual void Think();


		XGUI_Widget* WidgetUnderCursor(ME_Math::Vector2D pt);
			

		XGUI_Widget(xgRect_t & rect);
		virtual ~XGUI_Widget();

		const std::vector<XGUI_Widget*> & GetChilds();
		const xgRect_t GetRect();
		inline const bool IsDragged() { return m_bDragged; }
	};
}

}