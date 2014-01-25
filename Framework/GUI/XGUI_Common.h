/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Common.h - common GUI definitions
 *
 **/

#ifndef XGUI_COMMON_H
#define XGUI_COMMON_H

namespace ME_Framework
{
	namespace ME_XGUI
	{
		typedef struct xgRect_s 
		{
			ME_Math::Vector2D pos;
			ME_Math::Vector2D ext;

			inline vec_t Left()
			{
				return pos.x;
			}

			inline vec_t Top()
			{
				return pos.y;
			}

			inline vec_t Right()
			{
				return pos.x + ext.x;
			}

			inline vec_t Bottom()
			{
				return pos.y + ext.y;
			}

			inline ME_Math::Vector2D BottomRight()
			{
				return pos + ext;
			}

			inline xgRect_s operator +(xgRect_s a)
			{
				xgRect_s c;
				c.pos = this->pos;
				c.ext = a.ext + this->ext;
				return c;
			}
			
		}xgRect_t;

		inline bool RectIntersection(xgRect_t & a,xgRect_t & b)
		{
			vec_t mins1[2],mins2[2],maxs1[2],maxs2[2];

			mins1[0] = a.pos.x;
			mins1[1] = a.pos.y;
			
			maxs1[0] = mins1[0] + a.ext.x;
			maxs1[1] = mins1[1] + a.ext.y;

			mins2[0] = b.pos.x;
			mins2[1] = b.pos.y;

			maxs2[0] = mins2[0] + b.ext.x;
			maxs2[1] = mins2[1] + b.ext.y;

			int count = 0;

			for(int i = 0 ; i < 2; i++)
			{
				if ((maxs1[i] <= mins2[i]) || (mins1[i] >= maxs2[i]))
					count++;
			}

			return count == 0;
		}

		class XGUI_Widget;

		typedef std::weak_ptr<XGUI_Widget> TWidgetWeakPtr;
		typedef std::shared_ptr<XGUI_Widget> TWidgetSharedPtr;
		typedef std::vector<TWidgetSharedPtr> TWidgetVector;

	}



}

#endif