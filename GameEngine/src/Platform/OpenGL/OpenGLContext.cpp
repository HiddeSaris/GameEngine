#include "gepch.h"
#include "OpenGLContext.h"

#include "GameEngine/Core/Core.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gl/GL.h>

namespace GameEngine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		GE_CORE_ASSERT(m_WindowHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GE_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		//GE_CORE_TRACE("OpenGL Info: ");
		//GE_CORE_TRACE("  Vendor: {0}", *glGetString(GL_VENDOR));
		//GE_CORE_TRACE("  Renderer: {0}", *glGetString(GL_RENDERER));
		//GE_CORE_TRACE("  Version: {0}", *glGetString(GL_VERSION));

		GE_CORE_INFO("OpenGL Info:");
		GE_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		GE_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		GE_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

#ifdef GE_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		GE_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "GameEngine requires at least OpenGL version 4.5!");
#endif

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}