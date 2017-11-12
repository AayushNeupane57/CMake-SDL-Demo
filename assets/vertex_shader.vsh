#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertex_uv;
uniform mat4 u_matrix;

out vec2 uv;

void main(){
    gl_Position = vec4((u_matrix * vec4(vertexPosition_modelspace.xyz, 1)).xy, 0, 1);
    uv = vec2(vertex_uv.x, vertex_uv.y);
}
