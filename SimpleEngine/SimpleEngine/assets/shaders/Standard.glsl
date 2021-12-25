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

uniform samplerCube skybox;

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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ����������ǿ��
    float diff = max(dot(normal, lightDir), 0.0);
    // ���㾵�淴��ǿ��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // �ϲ��������շ���
    vec3 ambient  = light.color  * vec3(texture(material.diffuse, v_TexCoord)) * 0.00f;
    vec3 diffuse  = light.color  * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.color * spec * vec3(texture(material.specular, v_TexCoord));
    return (ambient + diffuse + specular);
}  

// ���㶨�����ȷ��λ�õĹ�����ɫ
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // ����������ǿ��
    float diff = max(dot(normal, lightDir), 0.0);
    // ���㾵�淴��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // ����˥��
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance +
                 light.quadratic * (distance * distance));
    // �����������ϲ�
    vec3 ambient  = light.color  * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse  = light.color  * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.color * spec * vec3(texture(material.specular, v_TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    //return (ambient + diffuse + specular);
	return diffuse;
}

void main()
{
	vec3 normal = normalize(v_Normal);
	vec3 viewDir = normalize(u_CameraPos - v_Position);

	// ��һ��������ƽ�й���
    vec3 result = CalcDirLight(dirLight, normal, viewDir);
    // �ڶ��������㶥�����
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], normal, v_Position, viewDir);

	color = vec4(result, 1.0f);

	//��պз���
	vec3 I = normalize(v_Position - u_CameraPos);
    vec3 R = reflect(I, normalize(normal));
    //color = texture(skybox, R);

	//��պ�����
	float ratio = 1.0f/1.52f;
	vec3 I2 = normalize(v_Position - u_CameraPos);
    vec3 R2 = refract(I, normalize(normal), ratio);
    //color = texture(skybox, R2);
}
