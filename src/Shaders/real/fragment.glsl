#version 450 core

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

//in vec3 FragPos;
//in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_height1;

uniform PointLight plight;
uniform DirectionalLight dlight;
uniform vec3 viewPos;

in vec2 TexCoord;

/*
vec3 calcPointLight(PointLight light, vec3 norm, vec3 viewDir);
vec3 calcDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir);
float LinearizeDepth(float depth);
*/

float near = 0.1;
float far = 100.0;

void main() {
    //vec3 norm = normalize(texture(texture_normal1, TexCoords).rgb);
    //vec3 viewDir = normalize(viewPos - FragPos);

    //vec3 result = vec3(0.0);
    //result += calcDirectionalLight(dlight, norm, viewDir);
    //result += calcPointLight(plight, norm, viewDir);

    //float depth = LinearizeDepth(gl_FragCoord.z) / far;

    //FragColor = vec4(result, 1.0);

    FragColor = vec4(texture(texture_diffuse1, TexCoord).rgb, 1.0);
}

/*
vec3 calcPointLight(PointLight light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(lightDir, norm), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(texture_specular1, TexCoords).rgb;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(texture_specular1, TexCoords).rgb;

    return (ambient + diffuse + specular);
}

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}
*/
