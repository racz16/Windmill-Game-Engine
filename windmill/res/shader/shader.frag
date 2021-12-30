#version 450

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 texture_coordinate;

layout(binding = 1) uniform sampler2D texture_sampler;

layout(location = 0) out vec4 out_color;

void main() {
	vec3 N = normalize(normal);
	vec3 L = normalize(vec3(1.0f, 1.0f, 0.0f));
	vec3 texture_color = texture(texture_sampler, texture_coordinate).rgb;
	out_color = vec4(texture_color * dot(N, L), 1.0f);
}
