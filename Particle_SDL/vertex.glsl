#version 410

in vec3 vPosition;

uniform mat4 ModelView;
uniform mat4 projection;

void main()
{
	vec4 pos = vec4(vPosition, 1.0f);
    gl_Position = projection * ModelView * pos;
}