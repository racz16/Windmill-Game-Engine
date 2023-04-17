struct vertex_input {
	float3 position: Position;
	float3 normal: Normal;
	float2 texture_coordinates: Texcoord;
};

struct vertex_output {
	float4 position: SV_POSITION;
	float3 normal: Normal;
	float2 texture_coordinates: Texcoord;
};

cbuffer constant_buffer {
	float4x4 model;
	float3x3 inverse_model;
	float4x4 view;
	float4x4 projection;
};

vertex_output main(vertex_input input) {
	vertex_output output;
	output.position = mul(mul(mul(projection, view), model), float4(input.position, 1.0f));
	output.normal = mul(input.normal, inverse_model);
	output.texture_coordinates = input.texture_coordinates;
	return output;
}
