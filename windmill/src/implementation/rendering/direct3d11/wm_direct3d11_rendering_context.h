#pragma once

#include "rendering/rendering_context.h"
#include "defines/general_defines.h"
#include "rendering/gpu_mesh.h"

#include "../wm_gpu_vertex.h"

namespace wm {

	class wm_direct3d11_rendering_context: public rendering_context {
	private:

	#ifdef WM_BUILD_DEBUG
		static ID3DUserDefinedAnnotation* user_defined_annotation;
		static IDXGIInfoQueue* dxgi_info_queue;
	#endif

		std::vector<wm_gpu_vertex> vertices;
		std::vector<uint32_t> indices;

		IDXGISwapChain* swap_chain = nullptr;
		ptr<gpu_mesh> mesh = nullptr;
		ptr<gpu_buffer> constant_buffer = nullptr;
		ID3D11RenderTargetView* render_target_view = nullptr;
		ID3D11DepthStencilView* depth_stencil_view = nullptr;
		ID3D11ShaderResourceView* texture_view = nullptr;
		ID3D11SamplerState* sampler_state = nullptr;
		ID3D11VertexShader* vertex_shader = nullptr;
		ID3D11PixelShader* pixel_shader = nullptr;
		ID3D11InputLayout* input_layout = nullptr;
	public:
		static ID3D11Device* device;
		static ID3D11DeviceContext* device_context;

	private:
		void create_device_and_swap_chain();
		void register_event_handlers();
		void set_stencil_state() const;
		void create_render_target_view();
		void create_depth_stencil_view();
		void initialize_imgui() const;
		void create_mesh(const std::string& path);
		void load_mesh(const std::string& path);
		void create_texture(const std::string& path);
		void create_sampler_state();
		void create_constant_buffer();
		void create_shaders(LPCWSTR vertex_path, LPCWSTR pixel_path);
		void load_constant_data() const;

	#ifdef WM_BUILD_DEBUG
		void create_dxgi_info_queue();
		void create_user_defined_annotation();
		void set_debug_label(ID3D11DeviceChild* const object, const std::string& label) const;
	#endif

	public:

	#ifdef WM_BUILD_DEBUG
		static void start_group(LPCWSTR name);
		static void stop_group();
		static void set_marker(LPCWSTR name);
		static std::string get_message_category(const DXGI_INFO_QUEUE_MESSAGE_CATEGORY category);
		static std::string get_message_severity(const DXGI_INFO_QUEUE_MESSAGE_SEVERITY severity);
		static void dxgi_message_callback(const std::string& function, const int32_t line);
	#endif

		void initialize() override;
		void update() override;
		void destroy() override;
	};

}
