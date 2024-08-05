#version 400 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture1;

// Lighting Uniforms
uniform vec3 ambientColor;
uniform vec3 viewPos;

// Spotlight Properties
struct Spotlight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};
uniform Spotlight spotlight;
uniform float spotlightIntensity;


void main()
{
    // Normalize inputs
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(spotlight.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Ambient
    vec3 ambient = spotlight.ambient * spotlightIntensity;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * spotlight.diffuse * spotlightIntensity;

    // Specular
    float specularStrength = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * spotlight.specular * spotlightIntensity;

    // Spotlight (soft edges)
    float theta = dot(lightDir, normalize(-spotlight.direction));
    float epsilon = spotlight.cutOff - spotlight.outerCutOff;
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    // Attenuation
    float distance = length(spotlight.position - FragPos);
    float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance + spotlight.quadratic * (distance * distance));

    vec3 result = (ambient + (diffuse + specular) * attenuation) * texture(texture1, TexCoords).rgb;
    FragColor = vec4(result, texture(texture1, TexCoords).a);

    // Discard fragments with a low alpha value to prevent opaque fragments from being rendered
    if (FragColor.a < 0.1) discard;         
}
