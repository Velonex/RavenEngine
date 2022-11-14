This shader is used in the editor to render quads and ids
#shader vert
#version 450 core
layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_TintColor;
layout(location = 2) in vec2  a_TexCoords;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in uint a_EntityID;

out vec4  v_Color;
out vec2  v_TexCoord;
out float v_TilingFactor;
out flat float v_TexIndex;
out flat uint v_EntityID;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
	v_Color = a_TintColor;
	v_TexCoord = a_TexCoords;
	v_TilingFactor = a_TilingFactor;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

#shader frag
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out uint entityID;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TilingFactor;
in flat float v_TexIndex;
in flat uint v_EntityID;

uniform sampler2D u_Textures[16];

void main()
{
	vec4 col = v_Color;
	switch(int(v_TexIndex)) {
		case 0: color = col *= texture(u_Textures[0], v_TexCoord * v_TilingFactor); break;
		case 1: color = col *= texture(u_Textures[1], v_TexCoord * v_TilingFactor); break;
		case 2: color = col *= texture(u_Textures[2], v_TexCoord * v_TilingFactor); break;
		case 3: color = col *= texture(u_Textures[3], v_TexCoord * v_TilingFactor); break;
		case 4: color = col *= texture(u_Textures[4], v_TexCoord * v_TilingFactor); break;
		case 5: color = col *= texture(u_Textures[5], v_TexCoord * v_TilingFactor); break;
		case 6: color = col *= texture(u_Textures[6], v_TexCoord * v_TilingFactor); break;
		case 7: color = col *= texture(u_Textures[7], v_TexCoord * v_TilingFactor); break;
		case 8: color = col *= texture(u_Textures[8], v_TexCoord * v_TilingFactor); break;
		case 9: color = col *= texture(u_Textures[9], v_TexCoord * v_TilingFactor); break;
		case 10: color = col *= texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;
		case 11: color = col *= texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;
		case 12: color = col *= texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;
		case 13: color = col *= texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;
		case 14: color = col *= texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;
		case 15: color = col *= texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;
		//case 16: color = col *= texture(u_Textures[16], v_TexCoord * v_TilingFactor); break;
		//case 17: color = col *= texture(u_Textures[17], v_TexCoord * v_TilingFactor); break;
		//case 18: color = col *= texture(u_Textures[18], v_TexCoord * v_TilingFactor); break;
		//case 19: color = col *= texture(u_Textures[19], v_TexCoord * v_TilingFactor); break;
		//case 20: color = col *= texture(u_Textures[20], v_TexCoord * v_TilingFactor); break;
		//case 21: color = col *= texture(u_Textures[21], v_TexCoord * v_TilingFactor); break;
		//case 22: color = col *= texture(u_Textures[22], v_TexCoord * v_TilingFactor); break;
		//case 23: color = col *= texture(u_Textures[23], v_TexCoord * v_TilingFactor); break;
		//case 24: color = col *= texture(u_Textures[24], v_TexCoord * v_TilingFactor); break;
		//case 25: color = col *= texture(u_Textures[25], v_TexCoord * v_TilingFactor); break;
		//case 26: color = col *= texture(u_Textures[26], v_TexCoord * v_TilingFactor); break;
		//case 27: color = col *= texture(u_Textures[27], v_TexCoord * v_TilingFactor); break;
		//case 28: color = col *= texture(u_Textures[28], v_TexCoord * v_TilingFactor); break;
		//case 29: color = col *= texture(u_Textures[29], v_TexCoord * v_TilingFactor); break;
		//case 30: color = col *= texture(u_Textures[30], v_TexCoord * v_TilingFactor); break;
		//case 31: color = col *= texture(u_Textures[31], v_TexCoord * v_TilingFactor); break;
	}
	entityID = v_EntityID;
}