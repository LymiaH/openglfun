#version 330 core
out vec4 FragColor;

// Global attribute using uniform
uniform vec4 ourColor;

void main()
{
    FragColor = ourColor;
}
