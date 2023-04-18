#include "wm_direct3d11_rendering_context.h"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_glfw.h>

#include "core/engine.h"
#include "defines/log_defines.h"
#include "window/event/window_framebuffer_size_event.h"
#include "window/event/keyboard_character_event.h"
#include "window/event/mouse_scroll_event.h"
#include "component/camera/camera_component.h"
#include "defines/code_generation_defines.h"
#include "../wm_gpu_matrices_ubo.h"

namespace wm {

#ifdef WM_BUILD_DEBUG
	#define D3D11_CALL(expression) expression; dxgi_message_callback(__FUNCTION__, __LINE__)
	#define DX_LABEL(object, name) set_object_label(object, name)
	#define DX_GROUP_START(name) start_group(name)
	#define DX_GROUP_STOP() stop_group()
	#define DX_MARKER(name) set_marker(name)
#else
	#define D3D11_CALL(expression) expression
	#define DX_LABEL(object, name)
	#define DX_GROUP_START(name)
	#define DX_GROUP_STOP()
	#define DX_MARKER(name)
#endif

	void wm_direct3d11_rendering_context::initialize() {
		create_device_and_swap_chain();
	#ifdef WM_BUILD_DEBUG
		create_dxgi_info_queue();
		create_user_defined_annotation();
	#endif
		register_event_handlers();
		set_stencil_state();
		create_render_target_view();
		create_depth_stencil_view();
		initialize_imgui();
		load_mesh("res/mesh/helmet.obj");
		create_vertex_buffer();
		create_index_buffer();
		create_texture("res/mesh/helmet.jpg");
		create_sampler_state();
		create_constant_buffer();
		create_shaders(HLSL_VERTEX(L"lambertian"), HLSL_PIXEL(L"lambertian"));
	}

	void wm_direct3d11_rendering_context::create_device_and_swap_chain() {
		HWND hwnd = std::any_cast<HWND>(get_win32_handle());

		DXGI_SWAP_CHAIN_DESC swap_chain_descriptor{};
		swap_chain_descriptor.BufferCount = 1;
		swap_chain_descriptor.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_descriptor.OutputWindow = hwnd;
		swap_chain_descriptor.SampleDesc.Count = 8;
		swap_chain_descriptor.SampleDesc.Quality = 0;
		swap_chain_descriptor.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_descriptor.Windowed = TRUE;

		std::vector<D3D_FEATURE_LEVEL> feature_levels = {D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1};
		UINT create_device_flags = 0;
	#if WM_BUILD_DEBUG
		create_device_flags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
	#else
		feature_levels.push_back(D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0);
	#endif

		D3D_FEATURE_LEVEL feature_level;

		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			create_device_flags,
			feature_levels.data(),
			static_cast<uint32_t>(feature_levels.size()),
			D3D11_SDK_VERSION,
			&swap_chain_descriptor,
			&swap_chain,
			&device,
			&feature_level,
			&device_context
		);
		WM_LOG_INFO_2("DXGI swap chain created");
		WM_LOG_INFO_2("Direct3D 11 device created");
		WM_LOG_INFO_2("Direct3D 11 device context created");
	}

#ifdef WM_BUILD_DEBUG

	void wm_direct3d11_rendering_context::set_object_label(ID3D11DeviceChild* const object, const std::string& name) const {
		D3D11_CALL(object->SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str()));
	}

	void wm_direct3d11_rendering_context::start_group(LPCWSTR name) const {
		D3D11_CALL(user_defined_annotation->BeginEvent(name));
	}

	void wm_direct3d11_rendering_context::stop_group() const {
		D3D11_CALL(user_defined_annotation->EndEvent());
	}

	void wm_direct3d11_rendering_context::set_marker(LPCWSTR name) const {
		D3D11_CALL(user_defined_annotation->SetMarker(name));
	}

	void wm_direct3d11_rendering_context::create_dxgi_info_queue() {
		typedef HRESULT(WINAPI* DXGIGetDebugInterface_type)(REFIID, void**);
		auto dxgidebug_dll = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		auto DXGIGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface_type>(reinterpret_cast<void*>(GetProcAddress(dxgidebug_dll, "DXGIGetDebugInterface")));
		DXGIGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&dxgi_info_queue));
		WM_LOG_INFO_2("DXGI info queue created");
	}

	std::string wm_direct3d11_rendering_context::get_message_category(const DXGI_INFO_QUEUE_MESSAGE_CATEGORY category) const {
		switch(category) {
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_UNKNOWN: return "UNKNOWN";
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_MISCELLANEOUS: return "MISCELLANEOUS";
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_INITIALIZATION: return "INITIALIZATION";
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_CLEANUP: return "CLEANUP";
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_COMPILATION: return "COMPILATION";
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_CREATION: return "STATE_CREATION";
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_SETTING: return "STATE_SETTING";
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_GETTING: return "STATE_GETTING";
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_RESOURCE_MANIPULATION: return "RESOURCE_MANIPULATION";
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_EXECUTION: return "EXECUTION";
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_SHADER: return "SHADER";
			default: return "UNKNOWN";
		}
	}

	std::string wm_direct3d11_rendering_context::get_message_severity(const DXGI_INFO_QUEUE_MESSAGE_SEVERITY severity) const {
		switch(severity) {
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION: return "CORRUPTION";
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR: return "ERROR";
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING: return "WARNING";
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO: return "INFO";
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE: return "MESSAGE";
			default: return "UNKNOWN";
		}
	}

	void wm_direct3d11_rendering_context::dxgi_message_callback(const std::string& function, const int32_t line) const {
		uint64_t message_count = dxgi_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for(uint64_t i = 0; i < message_count; i++) {
			size_t message_size = 0;
			dxgi_info_queue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &message_size);
			DXGI_INFO_QUEUE_MESSAGE* message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(malloc(message_size));
			dxgi_info_queue->GetMessage(DXGI_DEBUG_ALL, i, message, &message_size);

			auto category_str = get_message_category(message->Category);
			auto severity_str = get_message_severity(message->Severity);
			if(message->Severity == DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE) {
				WM_LOG_INFO_3("Direct3D 11 info 3: " + severity_str + ", " + category_str + ", " + std::to_string(message->ID) + ", " + message->pDescription);
			} else if(message->Severity == DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO) {
				WM_LOG_INFO_1("Direct3D 11 info 1: " + severity_str + ", " + category_str + ", " + std::to_string(message->ID) + ", " + message->pDescription);
			} else if(message->Severity == DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING) {
				WM_LOG_WARNING("Direct3D 11 warning: " + severity_str + ", " + category_str + ", " + std::to_string(message->ID) + ", " + message->pDescription);
			} else {
				WM_LOG_ERROR("Direct3D 11 error: " + severity_str + ", " + category_str + ", " + std::to_string(message->ID) + ", " + message->pDescription, function, line);
			}
			free(message);
		}
		dxgi_info_queue->ClearStoredMessages(DXGI_DEBUG_ALL);
	}

	void wm_direct3d11_rendering_context::create_user_defined_annotation() {
		D3D11_CALL(device_context->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), reinterpret_cast<void**>(&user_defined_annotation)));
	}

#endif

	void wm_direct3d11_rendering_context::register_event_handlers() {
		engine::get_event_system()->add_event_listener<window_framebuffer_size_event>(window_framebuffer_size_event::get_key(), [this](const window_framebuffer_size_event event) {
			if(event.get_new_size().x <= 0 || event.get_new_size().y <= 0) {
				return;
			}
			D3D11_CALL(device_context->OMSetRenderTargets(0, nullptr, nullptr));
			if(depth_stencil_view != nullptr) {
				depth_stencil_view->Release();
				depth_stencil_view = nullptr;
			}
			if(render_target_view != nullptr) {
				render_target_view->Release();
				render_target_view = nullptr;
			}
			D3D11_CALL(swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0));
			create_render_target_view();
			create_depth_stencil_view();
			D3D11_CALL(device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view));
		});
	}

	void wm_direct3d11_rendering_context::set_stencil_state() const {
		D3D11_DEPTH_STENCIL_DESC depth_stencil_descriptor{};
		depth_stencil_descriptor.DepthEnable = TRUE;
		depth_stencil_descriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_descriptor.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

		ID3D11DepthStencilState* depth_stencil_state = nullptr;
		D3D11_CALL(device->CreateDepthStencilState(&depth_stencil_descriptor, &depth_stencil_state));
		DX_LABEL(depth_stencil_state, "[DEPTH STENCIL STATE] depth stencil state");
		D3D11_CALL(device_context->OMSetDepthStencilState(depth_stencil_state, 1));

		depth_stencil_state->Release();
	}

	void wm_direct3d11_rendering_context::create_render_target_view() {
		ID3D11Resource* back_buffer = nullptr;
		D3D11_CALL(swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&back_buffer)));
		D3D11_CALL(device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view));
		DX_LABEL(back_buffer, "[TEXTURE 2D] back buffer");
		DX_LABEL(render_target_view, "[RENDER TARGET VIEW] back buffer view");
		WM_LOG_INFO_2("Direct3D 11 back buffer created");
		back_buffer->Release();
	}

	void wm_direct3d11_rendering_context::create_depth_stencil_view() {
		glm::ivec2 size = engine::get_window_system()->get_framebuffer_size();

		D3D11_TEXTURE2D_DESC texture_descriptor{};
		texture_descriptor.Width = size.x;
		texture_descriptor.Height = size.y;
		texture_descriptor.MipLevels = 1;
		texture_descriptor.ArraySize = 1;
		texture_descriptor.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		texture_descriptor.SampleDesc.Count = 8;
		texture_descriptor.SampleDesc.Quality = 0;
		texture_descriptor.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		texture_descriptor.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

		ID3D11Texture2D* depth_stencil_texture = nullptr;
		D3D11_CALL(device->CreateTexture2D(&texture_descriptor, nullptr, &depth_stencil_texture));
		DX_LABEL(depth_stencil_texture, "[TEXTURE 2D] depth buffer");
		WM_LOG_INFO_2("Direct3D 11 depth buffer created");

		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_descriptor{};
		depth_stencil_view_descriptor.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		depth_stencil_view_descriptor.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2DMS;
		depth_stencil_view_descriptor.Texture2D.MipSlice = 0;
		D3D11_CALL(device->CreateDepthStencilView(depth_stencil_texture, &depth_stencil_view_descriptor, &depth_stencil_view));
		DX_LABEL(depth_stencil_view, "[DEPTH STENCIL VIEW] depth buffer view");
		WM_LOG_INFO_2("Direct3D 11 depth buffer view created");

		depth_stencil_texture->Release();
	}

	void wm_direct3d11_rendering_context::initialize_imgui() const {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOther(std::any_cast<GLFWwindow*>(engine::get_window_system()->get_native_id()), true);
		ImGui_ImplDX11_Init(device, device_context);
	}

	void wm_direct3d11_rendering_context::load_mesh(const std::string& path) {
		auto mesh = engine::get_resource_system()->get_mesh(path);
		for(auto& v : mesh->get_vertices()) {
			vertices.push_back({v.get_position(), v.get_normal(), v.get_texture_coordinate()});
		}
		indices = mesh->get_indices();
	}

	void wm_direct3d11_rendering_context::create_vertex_buffer() {
		D3D11_BUFFER_DESC vertex_buffer_descriptor{};
		vertex_buffer_descriptor.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		vertex_buffer_descriptor.ByteWidth = vertices.size() * sizeof(wm_gpu_vertex);
		vertex_buffer_descriptor.CPUAccessFlags = 0;
		vertex_buffer_descriptor.MiscFlags = 0;
		vertex_buffer_descriptor.StructureByteStride = sizeof(wm_gpu_vertex);
		vertex_buffer_descriptor.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA vertex_subresource_data{};
		vertex_subresource_data.pSysMem = vertices.data();

		D3D11_CALL(device->CreateBuffer(&vertex_buffer_descriptor, &vertex_subresource_data, &vertex_buffer));
		DX_LABEL(vertex_buffer, "[BUFFER] helmet vertex buffer");
		WM_LOG_INFO_2("Direct3D 11 vertex buffer created");
	}

	void wm_direct3d11_rendering_context::create_index_buffer() {
		D3D11_BUFFER_DESC index_buffer_descriptor{};
		index_buffer_descriptor.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		index_buffer_descriptor.ByteWidth = indices.size() * sizeof(uint32_t);
		index_buffer_descriptor.CPUAccessFlags = 0;
		index_buffer_descriptor.MiscFlags = 0;
		index_buffer_descriptor.StructureByteStride = sizeof(uint32_t);
		index_buffer_descriptor.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA index_subresource_data{};
		index_subresource_data.pSysMem = indices.data();

		D3D11_CALL(device->CreateBuffer(&index_buffer_descriptor, &index_subresource_data, &index_buffer));
		DX_LABEL(index_buffer, "[BUFFER] helmet index buffer");
		WM_LOG_INFO_2("Direct3D 11 index buffer created");
	}

	void wm_direct3d11_rendering_context::create_texture(const std::string& path) {
		auto image = engine::get_resource_system()->get_image(path);

		D3D11_TEXTURE2D_DESC texture_descriptor{};
		texture_descriptor.Width = image->get_size().x;
		texture_descriptor.Height = image->get_size().y;
		texture_descriptor.MipLevels = 0;
		texture_descriptor.ArraySize = 1;
		texture_descriptor.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_descriptor.SampleDesc.Count = 1;
		texture_descriptor.SampleDesc.Quality = 0;
		texture_descriptor.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		texture_descriptor.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
		texture_descriptor.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_GENERATE_MIPS;

		ID3D11Texture2D* texture = nullptr;
		D3D11_CALL(device->CreateTexture2D(&texture_descriptor, nullptr, &texture));
		DX_LABEL(texture, "[TEXTURE 2D] " + path);
		WM_LOG_INFO_2("Direct3D 11 texture created");

		device_context->UpdateSubresource(texture, 0, nullptr, image->get_pixels(), image->get_size().x * sizeof(uint8_t) * 4, 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC texture_view_descriptor{};
		texture_view_descriptor.Format = texture_descriptor.Format;
		texture_view_descriptor.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
		texture_view_descriptor.Texture2D.MostDetailedMip = 0;
		texture_view_descriptor.Texture2D.MipLevels = -1;
		D3D11_CALL(device->CreateShaderResourceView(texture, &texture_view_descriptor, &texture_view));
		DX_LABEL(texture_view, "[SHADER RESOURCE VIEW] " + path);
		D3D11_CALL(device_context->GenerateMips(texture_view));
		WM_LOG_INFO_2("Direct3D 11 texture view created");

		texture->Release();
	}

	void wm_direct3d11_rendering_context::create_sampler_state() {
		D3D11_SAMPLER_DESC sampler_descriptor{};
		//sampler_descriptor.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_descriptor.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		sampler_descriptor.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_descriptor.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_descriptor.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_descriptor.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;// D3D11_MAX_MAXANISOTROPY;
		sampler_descriptor.MipLODBias = 0.0f;
		sampler_descriptor.MinLOD = 0.0f;
		sampler_descriptor.MaxLOD = D3D11_FLOAT32_MAX;

		D3D11_CALL(device->CreateSamplerState(&sampler_descriptor, &sampler_state));
		DX_LABEL(sampler_state, "[SAMPLER STATE] diffuse texture sampler state");
		WM_LOG_INFO_2("Direct3D 11 sampler created");
	}

	void wm_direct3d11_rendering_context::create_constant_buffer() {
		D3D11_BUFFER_DESC constant_buffer_descriptor{};
		constant_buffer_descriptor.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		constant_buffer_descriptor.ByteWidth = sizeof(wm_gpu_matrices_ubo);
		constant_buffer_descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		constant_buffer_descriptor.MiscFlags = 0;
		constant_buffer_descriptor.StructureByteStride = sizeof(wm_gpu_matrices_ubo);
		constant_buffer_descriptor.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

		D3D11_CALL(device->CreateBuffer(&constant_buffer_descriptor, nullptr, &constant_buffer));
		DX_LABEL(constant_buffer, "[CONSTANT BUFFER] matrices");

		WM_LOG_INFO_2("Direct3D 11 constant buffer created");
	}

	void wm_direct3d11_rendering_context::create_shaders(LPCWSTR vertex_path, LPCWSTR pixel_path) {
		ID3DBlob* blob;

		D3D11_CALL(D3DReadFileToBlob(pixel_path, &blob));
		D3D11_CALL(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixel_shader));
		std::wstring wide(pixel_path);
		std::string str(wide.begin(), wide.end());
		DX_LABEL(pixel_shader, "[PIXEL SHADER] " + str);
		WM_LOG_INFO_2("Direct3D 11 pixel shader created");

		blob->Release();

		D3D11_CALL(D3DReadFileToBlob(vertex_path, &blob));
		D3D11_CALL(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertex_shader));
		wide = std::wstring(vertex_path);
		str = std::string(wide.begin(), wide.end());
		DX_LABEL(vertex_shader, "[VERTEX SHADER] " + str);
		WM_LOG_INFO_2("Direct3D 11 vertex shader created");

		std::array<D3D11_INPUT_ELEMENT_DESC, 3> input_element_descriptors = {
			D3D11_INPUT_ELEMENT_DESC{"Position", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(wm_gpu_vertex, position), D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
			D3D11_INPUT_ELEMENT_DESC{"Normal", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(wm_gpu_vertex, normal), D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
			D3D11_INPUT_ELEMENT_DESC{"Texcoord", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(wm_gpu_vertex, texture_coordinates), D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		D3D11_CALL(device->CreateInputLayout(input_element_descriptors.data(), static_cast<uint32_t>(input_element_descriptors.size()), blob->GetBufferPointer(), blob->GetBufferSize(), &input_layout));
		DX_LABEL(input_layout, "[INPUT LAYOUT] lambertian");
		blob->Release();
	}

	void wm_direct3d11_rendering_context::update() {
		glm::ivec2 size = engine::get_window_system()->get_framebuffer_size();

		D3D11_VIEWPORT viewport{};
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(size.x);
		viewport.Height = static_cast<float>(size.y);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		D3D11_CALL(device_context->RSSetViewports(1, &viewport));

		D3D11_CALL(device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view));
		std::array<float, 4> color = {0.0f, 0.0f, 0.0f, 1.0f};
		D3D11_CALL(device_context->ClearRenderTargetView(render_target_view, color.data()));
		D3D11_CALL(device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0));

		DX_GROUP_START(L"lambertian");

		D3D11_CALL(device_context->VSSetShader(vertex_shader, nullptr, 0));
		D3D11_CALL(device_context->PSSetShader(pixel_shader, nullptr, 0));

		D3D11_CALL(device_context->IASetInputLayout(input_layout));

		load_constant_data();
		D3D11_CALL(device_context->VSSetConstantBuffers(0, 1, &constant_buffer));

		const uint32_t stride = sizeof(wm_gpu_vertex);
		const uint32_t offset = 0;
		D3D11_CALL(device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset));

		D3D11_CALL(device_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0));

		D3D11_CALL(device_context->PSSetSamplers(0, 1, &sampler_state));
		D3D11_CALL(device_context->PSSetShaderResources(0, 1, &texture_view));

		D3D11_CALL(device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		D3D11_CALL(device_context->DrawIndexed(indices.size(), 0, 0));

		DX_GROUP_STOP();

		DX_MARKER(L"TEST MARKER");

		DX_GROUP_START(L"imgui");

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//ImGui::ShowDemoWindow();
		glm::vec2 position{20.0f, 20.0f};
		ImGui::SetNextWindowPos(position);
		ImGui::Begin("Statistics", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_::ImGuiWindowFlags_NoNav);
		ImGui::Text((std::to_string(engine::get_time_system()->get_fps()) + " FPS").c_str());
		ImGui::Text((std::to_string(engine::get_time_system()->get_frame_time()) + " ms").c_str());
		ImGui::Text((std::string("Resolution: ") + std::to_string(size.x) + " x " + std::to_string(size.y) + " px").c_str());
		ImGui::Text("Direct3D 11");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		DX_GROUP_STOP();


		D3D11_CALL(swap_chain->Present(0, 0));
	}

	void wm_direct3d11_rendering_context::load_constant_data() const {
		static const float ROTATION_SPEED = 0.05f;
		static float rotation = 0.0f;
		const float delta_time = engine::get_time_system()->get_delta_time();
		rotation += delta_time * ROTATION_SPEED;

		auto parent = engine::get_scene_system()->get_node_by_tag("parent");
		if(parent.is_valid()) {
			parent->get_transform()->set_relative_rotation(glm::vec3(0.0f, 1.0f, 0.0f), rotation);
		}
		glm::mat4 model_matrix = glm::mat4(1.0f);
		glm::mat3 inverse_model_matrix = glm::mat3(1.0f);
		auto child = engine::get_scene_system()->get_node_by_tag("child");
		if(child.is_valid()) {
			child->get_transform()->set_absolute_rotation(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
			model_matrix = child->get_transform()->get_model_matrix();
			inverse_model_matrix = child->get_transform()->get_inverse_model_matrix();
		}

		auto camera = engine::get_scene_system()->get_node_by_tag("camera")->get_component(camera_component::get_key());

		wm_gpu_matrices_ubo ubo;
		ubo.model = model_matrix;
		ubo.inverse_model = inverse_model_matrix;
		ubo.view = camera->get_view_matrix();
		ubo.projection = camera->get_projection_matrix();

		D3D11_MAPPED_SUBRESOURCE mapped_subresourcce{};
		D3D11_CALL(device_context->Map(constant_buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresourcce));
		memcpy(mapped_subresourcce.pData, &ubo, sizeof(ubo));
		D3D11_CALL(device_context->Unmap(constant_buffer, 0));
	}

	void wm_direct3d11_rendering_context::destroy() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		if(input_layout != nullptr) {
			input_layout->Release();
			input_layout = nullptr;
			WM_LOG_INFO_2("Direct3D 11 input layout destroyed");
		}
		if(pixel_shader != nullptr) {
			pixel_shader->Release();
			pixel_shader = nullptr;
			WM_LOG_INFO_2("Direct3D 11 pixel shader destroyed");
		}
		if(vertex_shader != nullptr) {
			vertex_shader->Release();
			vertex_shader = nullptr;
			WM_LOG_INFO_2("Direct3D 11 vertex shader destroyed");
		}
		if(sampler_state != nullptr) {
			sampler_state->Release();
			sampler_state = nullptr;
			WM_LOG_INFO_2("Direct3D 11 sampler destroyed");
		}
		if(texture_view != nullptr) {
			texture_view->Release();
			texture_view = nullptr;
			WM_LOG_INFO_2("Direct3D 11 texture view destroyed");
		}
		if(constant_buffer != nullptr) {
			constant_buffer->Release();
			constant_buffer = nullptr;
			WM_LOG_INFO_2("Direct3D 11 constant buffer destroyed");
		}
		if(index_buffer != nullptr) {
			index_buffer->Release();
			index_buffer = nullptr;
			WM_LOG_INFO_2("Direct3D 11 index buffer destroyed");
		}
		if(vertex_buffer != nullptr) {
			vertex_buffer->Release();
			vertex_buffer = nullptr;
			WM_LOG_INFO_2("Direct3D 11 vertex buffer destroyed");
		}
		if(depth_stencil_view != nullptr) {
			depth_stencil_view->Release();
			depth_stencil_view = nullptr;
			WM_LOG_INFO_2("Direct3D 11 depth buffer destroyed");
		}
		if(render_target_view != nullptr) {
			render_target_view->Release();
			render_target_view = nullptr;
			WM_LOG_INFO_2("Direct3D 11 back buffer view destroyed");
		}
		if(device_context != nullptr) {
			device_context->Release();
			device_context = nullptr;
			WM_LOG_INFO_2("Direct3D 11 device context destroyed");
		}
	#ifdef WM_BUILD_DEBUG
		if(user_defined_annotation != nullptr) {
			user_defined_annotation->Release();
			user_defined_annotation = nullptr;
			WM_LOG_INFO_2("Direct3D 11 user defined annotation destroyed");
		}
		if(dxgi_info_queue != nullptr) {
			dxgi_info_queue->Release();
			dxgi_info_queue = nullptr;
			WM_LOG_INFO_2("DXGI info queue destroyed");
		}
	#endif
		if(device != nullptr) {
			device->Release();
			device = nullptr;
			WM_LOG_INFO_2("Direct3D 11 device destroyed");
		}
		if(swap_chain != nullptr) {
			swap_chain->Release();
			swap_chain = nullptr;
			WM_LOG_INFO_2("DXGI swap chain destroyed");
		}
	}

}
