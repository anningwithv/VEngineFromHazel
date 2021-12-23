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

uniform float u_AmbientStrength;
uniform vec3 u_CameraPos;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 position;
    vec3 color;
};
uniform Light light;

struct DirLight 
{
    vec3 direction;
    vec3 color;
};  
uniform DirLight dirLight;

struct PointLight 
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;  

    vec3 color;
};  
#define NR_POINT_LIGHTS 1  
uniform PointLight pointLights[NR_POINT_LIGHTS];

float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    //color = vec4(vec3(depth), 1.0);
	color = texture(material.diffuse, v_TexCoord);
}
