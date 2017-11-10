#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 u_matrix;

void main(){
    gl_Position = vec4((u_matrix * vec4(vertexPosition_modelspace.xyz, 1)).xy, 0, 1);
}
