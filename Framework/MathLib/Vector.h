/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Vector.h - vector classes
 *
 **/

namespace ME_Editor
{
	namespace ME_Math
	{
		class Vector2D
		{			
		public:
			vec_t x,y;

			Vector2D(vec_t _x = 0,vec_t _y = 0): x(_x),y(_y) {};
						
			// Array operator
			vec_t operator [](int idx) 
			{
				assert(idx < 0 || idx > 1);

				switch (idx)
				{
				case 0:
					return x;
				case 1:
					return y;
				default:
					assert(_T("Should never happen") && false);
				}
			}

			// Addition
			Vector2D & operator += (Vector2D vOther)
			{
				x+=vOther.x;
				y+=vOther.y;

				return (*this);
			}

			Vector2D operator + (Vector2D vOther)
			{
				return Vector2D(x+vOther.x,y+vOther.y);
			}
			// Subtraction
			Vector2D & operator -= (Vector2D vOther)
			{
				x+=vOther.x;
				y+=vOther.y;

				return (*this);
			}

			Vector2D operator - (Vector2D vOther)
			{
				return Vector2D(x-vOther.x,y-vOther.y);
			}
			// Multiplication with vector
			Vector2D & operator *= (Vector2D vOther)
			{
				x*=vOther.x;
				y*=vOther.y;

				return (*this);
			}

			Vector2D operator * (Vector2D vOther)
			{
				return Vector2D(x*vOther.x,y*vOther.y);
			}

			// with scalar
			Vector2D operator * (float flOther)
			{
				return Vector2D(x*flOther,y*flOther);
			}

			Vector2D & operator *= (float flOther)
			{
				x*=flOther;
				y*=flOther;

				return (*this);
			}

			// Division	with vector
			Vector2D & operator /= (Vector2D vOther)
			{
				x/=vOther.x;
				y/=vOther.y;

				return (*this);
			}

			Vector2D operator / (Vector2D vOther)
			{
				return Vector2D(x/vOther.x,y / vOther.y);
			}

			// with scalar
			Vector2D operator / (float flOther)
			{
				return Vector2D(x/flOther,y/flOther);
			}

			Vector2D & operator /= (float flOther)
			{
				x/=flOther;
				y/=flOther;

				return (*this);
			}
		};
	}
}