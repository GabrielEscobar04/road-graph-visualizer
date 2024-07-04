#version 330 core
in vec3 vertexPosition;
in vec3 vertexColor;

out vec3 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool useVertexColor;
uniform vec3 uniformColor;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    fragColor = useVertexColor ? vertexColor : uniformColor;
}
