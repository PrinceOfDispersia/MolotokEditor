/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Dock.h - dock and dockable container classes
 *
 **/

namespace ME_Framework
{
	namespace ME_XGUI
	{
		class XGUI_Dock: public XGUI_Widget
		{
			bool m_bRenderOutline;
		public:
			XGUI_Dock(xgRect_t & r);
			virtual void DrawComponent();
			virtual void Think();
		};
		
		class XGUI_DockWindow: public XGUI_Widget
		{
			XGUI_GenericButton * m_pDragButton;
			TOrientation m_Orientation;

			

		public:
			XGUI_DockWindow(xgRect_t & r);
		
			virtual void Think();
			virtual void DrawComponent();
			virtual void HandleEvent(ME_Framework::appEvent_t & pEvent);
		};
	}

}