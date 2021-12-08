// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_Position;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
	v_Position = worldPos.xyz;

	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_Position;

uniform sampler2D u_Texture;
uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform float u_AmbientStrength;

void main()
{
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPos - v_Position);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec4 diffuse = diff * u_LightColor;
	color = u_AmbientStrength * diffuse * texture(u_Texture, v_TexCoord);
}
