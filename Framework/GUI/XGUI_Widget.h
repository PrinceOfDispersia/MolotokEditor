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

	class XGUI_Widget;

	typedef std::weak_ptr<XGUI_Widget> TWidgetWeakPtr;
	typedef std::shared_ptr<XGUI_Widget> TWidgetSharedPtr;

	inline bool operator == (TWidgetWeakPtr a,TWidgetWeakPtr b)
	{
		if (!a.lock()) return false;
		if (!b.lock()) return false;

		return a.lock().get() == b.lock().get();
	}

	class XGUI_Widget: public std::enable_shared_from_this<XGUI_Widget>
	{
		friend XGUI_Manager;
	protected:
		xgRect_t m_Rect;
		color32_t m_Color;
		mSheetGlyph_t * m_pImage[16];
		unsigned int m_ZOrder;
			
		float m_flTimers[4];
		std::vector<TWidgetSharedPtr> m_vChilds;
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
		

		float m_flHoveroutTimer;
		
		XGUI_Font * m_pGuiFontNormal;
		XGUI_Font * m_pGuiFontSmall;
		
		
		ME_Math::Vector2D m_vDragOrigin;
		bool m_bDragged;

		ME_Math::Vector2D m_vParentStart;

		void RecalcRectWithAligment();
		xgRect_t GetParentRect();

		void RecalcDrag();
		void RecursiveNotifyEveryone(ME_Framework::appEvent_t & ev);

		bool m_bDockable;
		TDockState m_DockState;
		
		XGUI_Widget * m_pDockWidget;
		xgRect_t m_PreDockRect;

		// Left Top Right Bottom
		ME_Math::Vector2D m_vMargins[2];
		
		virtual void OnWidgetDocked(TWidgetSharedPtr w) {};
		virtual void OnWidgetUndocked(TWidgetSharedPtr w) {};

		void RealignDockedItems(TItemAlignOrder order);

		std::vector<TWidgetWeakPtr> m_vDockedWidgets;
	public:
		void PointToClient(ME_Math::Vector2D & v);
		void ClientToScreen(ME_Math::Vector2D & v);
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
		void SetDockWidget(XGUI_Widget * w);
		void SetDockState(TDockState s);
		void SetDragged(bool val);
				
		// Getters
		XGUI_Widget *	GetParent() const;
		TAlign			GetAlign() const;
		TAnchor			GetAnchors() const;
		bool			GetVisibilty() const;
		bool			GetEnableState() const;
		String &		GetCaption();
		int				GetAlignPriority() const;
		int				GetZOrder() const;
		XGUI_Widget*	GetDockWidget() const;
		TDockState		GetDockState()  const;
		bool			IsDragged() const;


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

		const std::vector<TWidgetSharedPtr> & GetChilds();
		xgRect_t & GetRect();

		void SetPos(vec_t x,vec_t y);

		inline const bool IsDragged() { return m_bDragged; }
		inline const bool IsDockable() { return m_bDockable; }
	};
}

}