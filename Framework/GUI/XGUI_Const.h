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
			alTop = 0,
			alLeft,
			alRight,
			alBottom,
			alClientArea,
			alNone
		};

	
		enum class TAnchor
		{
			akNone = 0,
			akTop = (1<<0),
			akLeft = (1<<1),
			akRight = (1<<2),
			akBottom = (1<<3),
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

		ENUM_FLAGS(TAnchor);
	}

	

}