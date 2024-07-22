#version 400 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoords);
    
    // Discard fragments with a low alpha value to prevent opaque fragments from being rendered
    if (FragColor.a < 0.1) discard;         
}
