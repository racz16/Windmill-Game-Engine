struct pixel_input {
	float4 position: SV_POSITION;
	float3 normal: Normal;
	float2 texture_coordinates: Texcoord;
};

Texture2D diffuse_texture;
SamplerState diffuse_sampler_state;

float4 main(pixel_input input): SV_Target{
	float3 N = normalize(input.normal);
	float3 L = normalize(float3(1.0f, 1.0f, 0.0f));
	float3 texture_color = diffuse_texture.Sample(diffuse_sampler_state, input.texture_coordinates).rgb;
	return float4(texture_color * dot(N, L), 1.0f);
}
