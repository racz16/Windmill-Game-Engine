#version 450

layout(location = 0) in vec2 texture_coordinate;
layout(location = 1) in vec4 color;

layout(binding = 0) uniform sampler2D font_sampler;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = color * texture(font_sampler, texture_coordinate);
}
