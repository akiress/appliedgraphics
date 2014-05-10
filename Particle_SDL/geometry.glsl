#version 410

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

out vec2 TexCoord;

void main()
{
    for (int i = 0; i < gl_in.length(); i++) {
        gl_Position = gl_in[i].gl_Position + vec4(0.35f, 0.0f, 0.0f, 0.0f);
        TexCoord = vec2(0.0f, 0.0f);
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(0.0f, 0.35, 0.0f, 0.0f);
        TexCoord = vec2(0.0f, 3.0f);
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(-0.35f, 0.0f, 0.35f, 0.0f);
        TexCoord = vec2(3.0f, 0.0f);
        EmitVertex();
    }

    EndPrimitive();
}