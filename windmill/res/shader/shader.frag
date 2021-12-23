#version 450

layout(location = 0) in vec3 color;
layout(location = 1) in vec2 texture_coordinate;

layout(binding = 1) uniform sampler2D texture_sampler;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture(texture_sampler, vec2(1.0f - texture_coordinate.x, texture_coordinate.y));
}
