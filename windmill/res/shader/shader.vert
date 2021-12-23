#version 450

layout(binding = 0) uniform uniform_buffer_object {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_texture_coordinate;

layout(location = 0) out vec3 color;
layout(location = 1) out vec2 texture_coordinate;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(vertex_position, 1.0);
    color = vertex_color;
	texture_coordinate = vertex_texture_coordinate;
}
