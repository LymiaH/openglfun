#version 330 core
layout (location = 0) in vec3 aPos;

// This will be an input to the fragment shader
out vec4 vertexColor;
void main()
{
    // Use of swizzling (The 1.0 is used when clipping verticies from the scene?)
    gl_Position = vec4(aPos, 1.0);
    // Is the 4th value alpha?
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // Dark-red
}
