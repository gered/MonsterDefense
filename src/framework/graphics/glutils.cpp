#include "../debug.h"

#include "glutils.h"

const char* GetGLErrorString(GLenum error)
{
	switch (error)
	{
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
#ifdef MOBILE
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
#elif DESKTOP
	case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
#endif
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
	}

	return "Unknown error";
}
