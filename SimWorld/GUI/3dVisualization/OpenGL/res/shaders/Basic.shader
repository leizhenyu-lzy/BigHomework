#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texSlot;

uniform mat4 u_MVP;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexSlot;

void main()
{
    v_TexSlot = texSlot;
    v_Color = color;
    v_TexCoord = texCoord;
    gl_Position = u_MVP * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture[2];

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexSlot;

void main()
{
    int index = int(v_TexSlot);
    vec4 texColor = texture(u_Texture[index], v_TexCoord);
    color = texColor * u_Color * v_Color;
}