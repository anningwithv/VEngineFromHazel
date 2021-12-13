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
uniform float u_AmbientStrength;
uniform vec3 u_CameraPos;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 position;
    vec3 color;
};
uniform Light light;

void main()
{
    // 环境光
    vec3 ambient = light.color * material.ambient;

	// 漫反射光
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(light.position - v_Position);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = (diff * material.diffuse) * light.color;

	// 镜面高光
	vec3 viewDir = normalize(u_CameraPos - v_Position);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = (material.specular * spec * 10.0f) * light.color;

	vec3 col = (ambient + diffuse + specular);
	//col = specular;
	//col = ambient;
	//col = diffuse;
	//col = texture(u_Texture, v_TexCoord);

	color = vec4(col, 1.0f) * texture(u_Texture, v_TexCoord);
}
