#version 450

layout(binding = 0) uniform uniform_buffer_object {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec3 vertex_color;

layout(location = 0) out vec3 color;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(vertex_position, 0.0, 1.0);
    color = vertex_color;
}
