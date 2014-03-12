/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	OpenGLContext.h - OpenGL context class
 *
 **/

namespace ME_Framework
{
	class PlatformEnvironment;

	class IOpenGLContext
	{
	protected:
		ME_Math::Vector2D m_vWindowPosition;
		ME_Math::Vector2D m_vWindowExtents;

		friend PlatformEnvironment;
	public:
		IOpenGLContext() {};
		virtual ~IOpenGLContext() {};
		
		virtual void Activate(){};
		virtual void Resize(ME_Math::Vector2D vNewPos,ME_Math::Vector2D vNewExtents) {};

		virtual void MainLoop() {};

		virtual void FlagApplicationStart() {};
		virtual void FlagApplicationShutdown() {};
	};
}

