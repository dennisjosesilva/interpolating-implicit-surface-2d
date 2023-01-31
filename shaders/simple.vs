#version 410

// Vertex Shader

layout (location = 0) in vec2 pos;

uniform mat4 projection;

void main()
{
  gl_PointSize = 4;
  gl_Position = projection * vec4(pos, 0, 1.0);
}