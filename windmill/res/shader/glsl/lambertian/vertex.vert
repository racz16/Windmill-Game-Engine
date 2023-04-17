#version 450

//#extension GL_EXT_debug_printf : enable

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coordinate;

layout(binding = 0) uniform uniform_buffer_object {
    mat4 model;
	mat3 inverse_model;
    mat4 view;
    mat4 projection;
} ubo;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec2 texture_coordinate;

void main() {
	//if(gl_VertexIndex == 16) {
//		debugPrintfEXT("tc: %f %f", vertex_texture_coordinate.x, vertex_texture_coordinate.y);
	//}
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(vertex_position, 1.0);
    normal = ubo.inverse_model * vertex_normal;
	texture_coordinate = vertex_texture_coordinate;
}
