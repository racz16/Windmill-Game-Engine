#version 450

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec2 vertex_texture_coordinate;

layout(push_constant) uniform push_constants {
	vec2 scale;
	vec2 translate;
};

layout(location = 0) out vec2 texture_coordinate;
layout(location = 1) out vec4 color;

void main() {
	gl_Position = vec4(vertex_position * scale + translate, 0.0f, 1.0f);
	texture_coordinate = vertex_texture_coordinate;
	color = vertex_color;
}
