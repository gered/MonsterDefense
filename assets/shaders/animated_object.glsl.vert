#ifdef GL_ES
	precision mediump float;
#endif

attribute vec4 a_position1;
attribute vec4 a_position2;
attribute vec3 a_normal1;
attribute vec3 a_normal2;
attribute vec2 a_texcoord0;

uniform mat4 u_modelViewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat3 u_modelViewNormalMatrix;
uniform float u_lerp;

varying vec2 v_texCoord;
varying vec3 v_normal;

void main()
{
	v_texCoord = a_texcoord0;
	v_normal = u_modelViewNormalMatrix * mix(a_normal1, a_normal2, u_lerp);
	gl_Position = u_projectionMatrix * u_modelViewMatrix * mix(a_position1, a_position2, u_lerp);
}
