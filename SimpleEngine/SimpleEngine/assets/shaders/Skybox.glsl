// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_TexCoord;

void main()
{
	v_TexCoord = a_Position;

	vec4 pos = u_ViewProjection * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_TexCoord;

uniform samplerCube skybox;

void main()
{
    //color = texture(skybox, v_TexCoord);
	color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
