// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
    v_Color = a_Color;
	v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

// v_Color is from the vertex shader output, while the u_Color is setup somewhere else and uploaded here.
void main()
{
    // Sample the texture using the texture coordinates adjusted by the tiling factor.
    // The texture index is used to select which texture from the array of textures.
    // TexCood is the UV coordination, TilingFactor can scale it, which will lead to texture repeat
    // Wrapping style is defined by GL_REPEAT.
    vec4 sampledTextureColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor);
    
    // Multiply the sampled texture color by the vertex color to apply the tint.
    color = sampledTextureColor * v_Color;
}