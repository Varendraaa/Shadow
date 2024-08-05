#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
 // Calculate the normal dynamically
    vec3 camRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 camUp = vec3(view[0][1], view[1][1], view[2][1]);
    vec3 normal = normalize(cross(camRight, camUp));

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
