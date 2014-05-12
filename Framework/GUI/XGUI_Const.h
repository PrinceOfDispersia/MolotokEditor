/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Const.h - GUI constants
 *
 **/

namespace ME_Framework
{
	namespace ME_XGUI
	{
		enum class TAlign
		{
			alTop,
			alLeft,
			alRight,
			alBottom,
			alClientArea,
			alNone
		};
		

		enum class THorizontalAligment
		{
			alhLeft,
			alhCenter,
			alhRight
		};
		

		enum class TVerticalAligment
		{
			alvTop,			
			alvCenter,
			alvBottom
		};

	
		enum class TAnchor
		{
			akNone = 0,
			akTop = (1<<0),
			akLeft = (1<<1),
			akRight = (1<<2),
			akBottom = (1<<3),
		};

		enum AnchorsCoefs
		{
			ackTop = 0,
			ackBottom,
			ackLeft,
			ackRight
		};

		enum class TOrientation
		{
			orHorizontal = 1,
			orVertical
		};

		enum class TDockState
		{
			dsDocked = 1,
			dsFloating
		};

		enum class TItemAlignOrder
		{
			iaLeftToRightUpToDown,
			iaUpDownLeftToRight
		};

		enum class TGuiFontTypes
		{
			gfSmall,
			gfNormal
		};

		enum TWindowButtons
		{
			btnClose,
			btnMaximize,
			btnMinimize,
			btnRestore
		};

		enum class TWindowDragKinds
		{
			wdkNone =	(1<<0),
			wdkPos =	(1<<1),
			wdkXExt =	(1<<2),
			wdkXPos =	(1<<3),
			wdkYExt =	(1<<4),
			wdkYPos =	(1<<5)
		};

		ENUM_FLAGS(TAnchor);
		ENUM_FLAGS(TWindowDragKinds);

		const int xAxis = 0;
		const int yAxis = 1;

		enum TMenuItemKinds
		{
			mikGeneric = 0,
			mikToggle,
			mikSelection
		};
	}

	

	

}