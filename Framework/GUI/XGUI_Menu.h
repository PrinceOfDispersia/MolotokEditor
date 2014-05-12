/*
*	Molotok Editor, 2014 (C) PrinceOfDispersia
*	XGUI_Menu.h - menu classes
*
**/

namespace ME_Framework
{

	namespace ME_XGUI
	{
		class XGUI_MenuItem: public XGUI_Widget
		{
			bool m_bPressed;
			XGUI_Menu * m_pSubMenu;

			void DrawHoverOverlay();
			void DrawTextLabel();

			TMenuItemKinds m_MenuKind;
			int m_SelectionGroup;
			
			friend XGUI_Menu;
		public:

			void SetToggleState(bool state) { m_bPressed = state;}

			int GetSelectionGroup() const { return m_SelectionGroup; }
			void SetSelectionGroup(int val) { m_SelectionGroup = val; }

			void SetKind(TMenuItemKinds kind);

			XGUI_MenuItem(xgRect_t & rect);
			~XGUI_MenuItem();

			virtual void HandleEvent(ME_Framework::appEvent_t & ev);
			virtual void DrawComponent();

			void XGUI_MenuItem::AssingSubmenu(XGUI_Menu * pMenu);


			inline XGUI_Menu * SubMenu()
			{
				return m_pSubMenu;
			}

			
			bool IsBelongsHeirarchy(XGUI_Widget * w);
		
		};

		class XGUI_Menu: public XGUI_Widget
		{
			XGUI_Menu * m_pCurrentPopedUpSubmenu;
			XGUI_MenuItem * m_pSubmenuOwner;
			int m_iSubMenuHideCounter;

			void PopupSubmenu(XGUI_Menu * pMenu);
			
			

			friend XGUI_MenuItem;
		public:
			XGUI_Menu();
			~XGUI_Menu();

			XGUI_MenuItem* AddItem( String strName,TMenuItemKinds kind,int group,bool bChecked = false );



			void Popup(ME_Math::Vector2D pos);
			virtual void OnVisibilityChange(bool bNewState);

			virtual void DrawComponent();

		};

	}

}