#version 330

out vec4 fragColor;

in vec2 texCoordVarying;

uniform sampler2DRect particles2;

void main()
{
	vec3 col = texture(particles2, texCoordVarying.st).xyz;
    fragColor = vec4(col.xyz, 0.1);
}