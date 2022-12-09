#version 410

// Vertex Shader

layout (location = 0) in vec2 pos;

void main()
{
  gl_PointSize = 12;
  gl_Position = vec4(pos, 0, 1.0);
}