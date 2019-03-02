#ifdef GL_ES
	precision mediump float;
#endif

varying vec2 v_texCoord;
varying vec3 v_normal;

uniform sampler2D u_texture;
uniform vec3 u_lightDir;
uniform vec4 u_lightColor;

void main()
{
	vec4 ambient = vec4(0.5, 0.5, 0.5, 1.0);

	vec3 lightNorm = normalize(u_lightDir);
	float lightWeight = max(dot(lightNorm, v_normal), 0.0);

	vec4 texColor = texture2D(u_texture, v_texCoord);
	vec4 colorWeight = ambient + (lightWeight * u_lightColor);

	gl_FragColor = vec4(texColor.rgb * colorWeight.rgb, texColor.a);
}
