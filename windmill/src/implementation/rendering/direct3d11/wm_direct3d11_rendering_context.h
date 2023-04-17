#pragma once

#include "rendering/rendering_context.h"
#include "defines/general_defines.h"
#include "../wm_gpu_vertex.h"

namespace wm {

	class wm_direct3d11_rendering_context: public rendering_context {
	private:
		std::vector<wm_gpu_vertex> vertices;
		std::vector<uint32_t> indices;

		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* device_context = nullptr;
		IDXGISwapChain* swap_chain = nullptr;
		ID3D11Buffer* vertex_buffer = nullptr;
		ID3D11Buffer* index_buffer = nullptr;
		ID3D11Buffer* constant_buffer = nullptr;
		ID3D11RenderTargetView* render_target_view = nullptr;
		ID3D11DepthStencilView* depth_stencil_view = nullptr;
		ID3D11ShaderResourceView* texture_view = nullptr;
		ID3D11SamplerState* sampler_state = nullptr;
		ID3D11VertexShader* vertex_shader = nullptr;
		ID3D11PixelShader* pixel_shader = nullptr;
		ID3D11InputLayout* input_layout = nullptr;
	#ifdef WM_BUILD_DEBUG
		ID3DUserDefinedAnnotation* user_defined_annotation = nullptr;
		IDXGIInfoQueue* dxgi_info_queue = nullptr;
	#endif

		void create_device_and_swap_chain();
	#ifdef WM_BUILD_DEBUG
		void create_dxgi_info_queue();
		void create_user_defined_annotation();
		void set_object_label(ID3D11DeviceChild* const object, const std::string& name) const;
		void start_group(LPCWSTR name) const;
		void stop_group() const;
		void set_marker(LPCWSTR name) const;
		std::string get_message_category(const DXGI_INFO_QUEUE_MESSAGE_CATEGORY category) const;
		std::string get_message_severity(const DXGI_INFO_QUEUE_MESSAGE_SEVERITY severity) const;
		void dxgi_message_callback(const std::string& function, const int32_t line) const;
	#endif
		void register_event_handlers();
		void set_stencil_state() const;
		void create_render_target_view();
		void create_depth_stencil_view();
		void initialize_imgui() const;
		void load_mesh(const std::string& path);
		void create_vertex_buffer();
		void create_index_buffer();
		void create_texture(const std::string& path);
		void create_sampler_state();
		void create_constant_buffer();
		void create_shaders(LPCWSTR vertex_path, LPCWSTR pixel_path);
		void load_constant_data() const;
	public:
		void initialize() override;
		void update() override;
		void destroy() override;
	};

}
