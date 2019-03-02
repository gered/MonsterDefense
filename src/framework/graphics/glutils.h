#ifndef __FRAMEWORK_GRAPHICS_GLUTILS_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_GLUTILS_H_INCLUDED__

#include "glincludes.h"

/**
 * Converts a OpenGL error code to a string equivalent.
 * @param error the error code to convert
 * @return a string equivalent of the given error code
 */
const char* GetGLErrorString(GLenum error);

#define GL_ERROR_DEBUG

/**
 * Macro wrapper for OpenGL calls which will call glGetError()
 * immediately after to validate success if GL_ERROR_DEBUG is defined.
 */
#ifdef GL_ERROR_DEBUG
	#include "../debug.h"
	#include "../log.h"
	#define GL_CALL(exp) \
	{ \
		exp; \
		GLenum error = glGetError(); \
		if (error != GL_NO_ERROR) \
			LOG_ERROR(LOGCAT_OPENGL, "OpenGL error \"%s\" after: %s\n", GetGLErrorString(error), #exp); \
		ASSERT(error == GL_NO_ERROR); \
	}
#else
	#define GL_CALL(exp) exp
#endif

#endif
