// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 lightSpaceMatrix;

out VS_OUT 
{
    vec3 v_Position;
    vec3 v_Normal;
    vec2 v_TexCoord;
    vec4 v_FragPosLightSpace;
} vs_out;

void main()
{
	vs_out.v_TexCoord = a_TexCoord;
	vs_out.v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
	vs_out.v_Position = worldPos.xyz;
	vs_out.v_FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.v_Position, 1.0);

	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in VS_OUT 
{
    vec3 v_Position;
    vec3 v_Normal;
    vec2 v_TexCoord;
    vec4 v_FragPosLightSpace;
} fs_in;

uniform float u_AmbientStrength;
uniform vec3 u_CameraPos;

uniform samplerCube skybox;
uniform sampler2D shadowMap;

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

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // ִ��͸�ӳ���
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // �任��[0,1]�ķ�Χ
    projCoords = projCoords * 0.5 + 0.5;
    // ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // ȡ�õ�ǰƬ���ڹ�Դ�ӽ��µ����
    float currentDepth = projCoords.z;
    // ��鵱ǰƬ���Ƿ�����Ӱ��
	float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ����������ǿ��
    float diff = max(dot(normal, lightDir), 0.0);
    // ���㾵�淴��ǿ��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // �ϲ��������շ���
    vec3 ambient  = light.color  * vec3(texture(material.diffuse, fs_in.v_TexCoord)) * 0.00f;
    vec3 diffuse  = light.color  * diff * vec3(texture(material.diffuse, fs_in.v_TexCoord));
    vec3 specular = light.color * spec * vec3(texture(material.specular, fs_in.v_TexCoord));

    // ������Ӱ
    float shadow = ShadowCalculation(fs_in.v_FragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));  

    return lighting;
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
    vec3 ambient  = light.color  * vec3(texture(material.diffuse, fs_in.v_TexCoord));
    vec3 diffuse  = light.color  * diff * vec3(texture(material.diffuse, fs_in.v_TexCoord));
    vec3 specular = light.color * spec * vec3(texture(material.specular, fs_in.v_TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

	// ������Ӱ
    float shadow = ShadowCalculation(fs_in.v_FragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));  

    //return (ambient + diffuse + specular);
	//return diffuse;
	return lighting;
}

void main()
{
	vec3 normal = normalize(fs_in.v_Normal);
	vec3 viewDir = normalize(u_CameraPos - fs_in.v_Position);

	// ��һ��������ƽ�й���
    vec3 result = CalcDirLight(dirLight, normal, viewDir);
    // �ڶ��������㶥�����
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], normal, fs_in.v_Position, viewDir);

	color = vec4(result, 1.0f);

	//��պз���
	vec3 I = normalize(fs_in.v_Position - u_CameraPos);
    vec3 R = reflect(I, normalize(normal));
    //color = texture(skybox, R);

	//��պ�����
	float ratio = 1.0f/1.52f;
	vec3 I2 = normalize(fs_in.v_Position - u_CameraPos);
    vec3 R2 = refract(I, normalize(normal), ratio);
    //color = texture(skybox, R2);
}
