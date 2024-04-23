This shader is used in the editor to render meshes and ids
#shader vert
#version 450 core
layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec3  a_Normal;

out vec3 v_Normal;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

void main()
{
	
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}

#shader frag
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out uint entityID;

in vec3 v_Normal;

uniform uint u_EntityID;

void main()
{
	color = vec4(0.3, 0.3, 0.3, 1.0);
	entityID = u_EntityID;
}