/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	OpenGLContext.h - OpenGL context class
 *
 **/

namespace ME_Framework
{
	class IOpenGLContext
	{
	protected:
		ME_Math::Vector2D m_vWindowPosition;
		ME_Math::Vector2D m_vWindowExtents;
	public:
		IOpenGLContext() {};
		virtual ~IOpenGLContext() {};
		
		virtual void Activate(){};
		virtual void Resize(ME_Math::Vector2D vNewPos,ME_Math::Vector2D vNewExtents) {};

		virtual void MainLoop() {};
	};
}

