#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;


void main()
{
    
    vec4 texColor = texture(u_Texture, v_TexCoord);

    texColor.x = v_TexCoord.x;
    texColor.y = v_TexCoord.y;
    texColor.z = 1.0;

    color = texColor;
}