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

	

	inline bool operator == (TWidgetWeakPtr a,TWidgetWeakPtr b)
	{
		if (!a.lock()) return false;
		if (!b.lock()) return false;

		return a.lock().get() == b.lock().get();
	}

	class XGUI_Menu;

	class XGUI_Widget: public std::enable_shared_from_this<XGUI_Widget>
	{
		friend XGUI_Manager;

		bool m_bFocused;
		bool m_bVisible;
	protected:		
		static const int zFocused = 9999;
				
		unsigned int m_ZOrder;
		unsigned int m_PreFocusedZOrder;
		float m_flTimers[4];
		int m_nWidgetNumber;
		int m_nWidgetCounter;
		int m_AlignPriority;
		bool m_bEnabled;
		float m_flHoveroutTimer;		
		bool m_bDragged;

		ME_Math::Vector2D m_vDragOrigin;		
		ME_Math::Vector2D m_vParentStart;		
		ME_Math::Vector2D m_vMargins[2];

		xgRect_t m_WorkRect;
		xgRect_t m_UnAlignedRect;

		color32_t m_Color;
		
		vec_t m_AnchorsCoefs[4];

		TAlign m_Align;
		TAnchor m_Anchors; 
		eMouseCursors m_CurrentCursor;

		XGUI_Widget * m_pParent;
		mSheetGlyph_t * m_pImage[16];
		XGUI_Font * m_pGuiFontNormal;
		XGUI_Font * m_pGuiFontSmall;		
		XGUI_Menu * m_pContextMenu;

		String m_strCaption;
		String m_strHint;
		
		TWidgetVector m_vChilds;
		std::vector<XGUI_Widget*> m_vAlignOrderedChilds;
		
		

		xgRect_t GetParentRect();
		virtual void RecalcDrag();
		void SortChilds();
		void SortChildsByAlignOrder();
		void CalcClientRect(xgRect_t & r);
		void RecalcRectWithAligment();				
		void RecursiveNotifyEveryone(ME_Framework::appEvent_t & ev,XGUI_Widget * filter = NULL);	
		void OnAddToParent(XGUI_Widget * pParent);
		void SetMarginsFromSkinset(mSheetGlyph_t * spr[9]);	
	public:
		void ScreenToClient(ME_Math::Vector2D & v);
		void ClientToScreen(ME_Math::Vector2D & v);
		void AddChildWidget(XGUI_Widget * pWidget);
		
		virtual bool IsBelongsHeirarchy(XGUI_Widget * w);

		// Setters
		void SetParent(XGUI_Widget * pParent);
		void SetAlign(TAlign align);
		void SetAlignPriority(int alignPriority);
		void SetAnchors(TAnchor anchors);
		void SetVisibilty(bool bVisible);
		void SetEnableState(bool bEnabled);
		void SetCaption(String & str);
		void SetZOrder(int order);
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
		bool			IsDragged() const;

		void DoHandleEvent(ME_Framework::appEvent_t & pEvent);

		virtual void HandleEvent(ME_Framework::appEvent_t & pEvent);
		virtual void DrawComponent();

		void Render();		
		void UpdateTimers(float flDelta);
		void SetRect(xgRect_t & rect);
		void RecalcItemsRects();

		void DoThink();
		virtual void Think();

		void GrabFocus();
		bool IsFocused();


		XGUI_Widget* WidgetUnderCursor(ME_Math::Vector2D pt);
			

		XGUI_Widget(xgRect_t & rect);
		XGUI_Widget();
		virtual ~XGUI_Widget();

		const TWidgetVector & GetChilds();
		xgRect_t & GetRect();

		void SetPos(vec_t x,vec_t y);

		inline const bool IsDragged() { return m_bDragged; }
		
		virtual void OnVisibilityChange(bool bNewState) {};

		//virtual void InitializeMetada();
	};
}

}