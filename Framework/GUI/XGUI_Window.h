/*
 *	Molotok Editor, 2014 (C) PrinceOfDispersia
 *	XGUI_Widget.h - basic gui window class
 *
 **/

namespace ME_Framework
{
	namespace ME_XGUI
	{


		/* Basic window class */
		class XGUI_Window: public XGUI_Widget
		{
			TWindowDragKinds m_DragKind;
						
			void CheckBorders();

			mSheetGlyph_t ** ActiveSkin();

			enum resizeFlags
			{
				rfLockXPos = (1<<0),
				rfLockYPos = (1<<1)
			};

			int ExpandRectByContents();



		protected:
			virtual void RecalcDrag();
		public:
			
			XGUI_Window(xgRect_t & r);
			~XGUI_Window();

			virtual void HandleEvent( ME_Framework::appEvent_t & pEvent );
			virtual void DrawComponent();
			virtual void Think();
		};
	}
}