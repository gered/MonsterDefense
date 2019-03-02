#ifdef GL_ES
	precision mediump float;
#endif

attribute vec4 a_position;
attribute vec2 a_texcoord0;

uniform mat4 u_modelViewMatrix;
uniform mat4 u_projectionMatrix;

varying vec2 v_texCoord;

void main()
{
	v_texCoord = a_texcoord0;
	gl_Position = u_projectionMatrix * u_modelViewMatrix * a_position;
}
