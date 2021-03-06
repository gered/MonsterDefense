#include "../debug.h"

#include "simpletextureshader.h"

const char* SimpleTextureShader::m_vertexShaderSource = 
	"attribute vec4 a_position;\n"
	"attribute vec2 a_texcoord0;\n"
	"uniform mat4 u_modelViewMatrix;\n"
	"uniform mat4 u_projectionMatrix;\n"
	"varying vec4 v_color;\n"
	"varying vec2 v_texCoords;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	v_texCoords = a_texcoord0;\n"
	"	gl_Position =  u_projectionMatrix * u_modelViewMatrix * a_position;\n"
	"}\n";

const char* SimpleTextureShader::m_fragmentShaderSource = 
	"#ifdef GL_ES\n"
	"	precision mediump float;\n"
	"#endif\n"
	"varying vec2 v_texCoords;\n"
	"uniform sampler2D u_texture;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(u_texture, v_texCoords);\n"
	"}\n";

SimpleTextureShader::SimpleTextureShader()
	: StandardShader()
{
	STACK_TRACE;
	BOOL result = LoadCompileAndLinkInlineSources(m_vertexShaderSource, m_fragmentShaderSource);
	ASSERT(result == TRUE);

	MapAttributeToStandardAttribType("a_position", VERTEX_POS_3D);
	MapAttributeToStandardAttribType("a_texcoord0", VERTEX_TEXCOORD);
}

SimpleTextureShader::~SimpleTextureShader()
{
	STACK_TRACE;
}
