#version 330 core
// 给光源数据一个结构体
struct Light{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 position;

	 // 光源衰减系数的三个参数
    float constant; // 常数项
    float linear;	// 一次项
    float quadratic;// 二次项
};

// 给物体材质数据一个结构体
struct Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float shininess;
};

in vec3 position;
in vec4 mirror_position;
in vec3 normal;
in vec2 texCoord;
in vec4 FragPosLightSpace;
in vec3 FragPos;
in vec3 color;


// 相机坐标
uniform vec3 eye_position;
// 光源
uniform Light light;
// 物体材质
uniform Material material;

uniform int isShadow;
uniform int useTexture;

out vec4 fColor;

// 纹理数据
uniform sampler2D texture;
uniform sampler2D shadowMap;

//镜子投影矩阵
uniform mat4 PVMatrix;

float ShadowCalculation(vec4 fragPosLightSpace,float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow

	bias=0.005;
	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture2D(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	if(projCoords.z > 1.01)
        shadow = 0.0;
    return shadow;
}

void main()
{
		// 将顶点坐标、光源坐标和法向量转换到相机坐标系
		vec3 norm = (vec4(normal, 0.0)).xyz;

		// 计算四个归一化的向量 N,V,L,R(或半角向量H)
		vec3 N = normalize(norm);
		vec3 V = normalize(eye_position - position);
		vec3 L = normalize(light.position - position);
		vec3 R = normalize(reflect(-L, N));
		
		float bias = max(0.02* (1.0 - dot(N, L)), 0.005);

		// 环境光分量I_a
		vec4 I_a = light.ambient * material.ambient;

		// 计算系数和漫反射分量I_d
		float diffuse_dot = max(dot(L,N), 0);
		vec4 I_d = diffuse_dot * light.diffuse * material.diffuse;

		// 计算系数和镜面反射分量I_s
		float specular_dot_pow = pow(max(dot(R,V), 0), material.shininess);
		vec4 I_s = specular_dot_pow * light.specular * material.specular;

		// 计算衰减参数
		float d = distance(light.position, position);
		float c = 1 / (light.constant + light.linear*d + light.quadratic*d*d);
		// 结合衰减系数，合并三个分量的颜色，修正透明度
		float shadow = ShadowCalculation(FragPosLightSpace,bias);    
		fColor = I_a + (1.0-shadow)*c*(I_d + I_s);
		fColor.a = 1.0;

		// 将光照模型颜色作为对应颜色亮度，与材质颜色相乘
        //position = 
		vec4 mirror_pos = PVMatrix * mirror_position;
		mirror_pos = mirror_pos / mirror_pos.w;
		mirror_pos = (mirror_pos + 1.0) * 0.5;
		vec4 tmp = texture2D( texture, mirror_pos.xy );
		fColor.r *= tmp.r;
		fColor.g *= tmp.g;
		fColor.b *= tmp.b;
		//float depthValue = texture(shadowMap, texCoord).r-0.5;
		//fColor = vec4(vec3(depthValue), 1.0);
}

