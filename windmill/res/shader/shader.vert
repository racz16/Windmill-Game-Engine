#version 450

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coordinate;

layout(binding = 0) uniform uniform_buffer_object {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec2 texture_coordinate;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(vertex_position, 1.0);
    normal = mat3(transpose(inverse(ubo.model))) * vertex_normal;
	texture_coordinate = vertex_texture_coordinate;
}
