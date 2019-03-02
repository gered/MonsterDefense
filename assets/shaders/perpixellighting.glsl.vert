#ifdef GL_ES
	precision mediump float;
#endif

attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord0;

uniform mat4 u_modelViewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat3 u_modelViewNormalMatrix;

varying vec2 v_texCoord;
varying vec3 v_normal;

void main()
{
	v_texCoord = a_texcoord0;
	v_normal = u_modelViewNormalMatrix * a_normal;
	gl_Position = u_projectionMatrix * u_modelViewMatrix * a_position;
}
