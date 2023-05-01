#include "wm_dx11_buffer.h"

#include "defines/log_defines.h"

#include "wm_dx11_defines.h"
#include "wm_direct3d11_rendering_context.h"

namespace wm {

	wm_dx11_buffer::wm_dx11_buffer(const gpu_buffer_descriptor descriptor)
		: type_flags(descriptor.type_flags), cpu_write_frequency(descriptor.cpu_write_frequency), data_size(descriptor.data_size), stride(descriptor.stride) {

		D3D11_BUFFER_DESC index_buffer_descriptor{};
		index_buffer_descriptor.ByteWidth = data_size;
		index_buffer_descriptor.Usage = get_usage();
		index_buffer_descriptor.BindFlags = get_bind_flags();
		index_buffer_descriptor.CPUAccessFlags = get_cpu_access_flags();
		index_buffer_descriptor.MiscFlags = 0;
		index_buffer_descriptor.StructureByteStride = stride;

		D3D11_SUBRESOURCE_DATA* subresource_pointer = nullptr;
		if(descriptor.data) {
			D3D11_SUBRESOURCE_DATA index_subresource_data{};
			index_subresource_data.pSysMem = descriptor.data;
			index_subresource_data.SysMemPitch = 0;
			index_subresource_data.SysMemSlicePitch = 0;
			subresource_pointer = &index_subresource_data;
		}

		DX_CALL(wm_direct3d11_rendering_context::device->CreateBuffer(&index_buffer_descriptor, subresource_pointer, &native_handle));
		WM_ASSERT(native_handle);
		WM_LOG_INFO_2("Direct3D 11 buffer created");
	}

	UINT wm_dx11_buffer::get_bind_flags() const {
		UINT bind_flags = 0;
		if(WM_CONTAINS_FLAG(type_flags, gpu_buffer_type::VERTEX_BUFFER)) {
			bind_flags |= D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		}
		if(WM_CONTAINS_FLAG(type_flags, gpu_buffer_type::INDEX_BUFFER)) {
			bind_flags |= D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		}
		if(WM_CONTAINS_FLAG(type_flags, gpu_buffer_type::UNIFORM_BUFFER)) {
			bind_flags |= D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		}
		return bind_flags;
	}

	UINT wm_dx11_buffer::get_cpu_access_flags() const {
		// TODO: make the CPU read available
		return cpu_write_frequency == usage_frequency::NEVER ? 0 : D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	}

	D3D11_USAGE wm_dx11_buffer::get_usage() const {
		// TODO: make the CPU read and the GPU write available
		return cpu_write_frequency == usage_frequency::NEVER ? D3D11_USAGE::D3D11_USAGE_IMMUTABLE : D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	}

	std::any wm_dx11_buffer::get_native_handle() const {
		return native_handle;
	}

	int32_t wm_dx11_buffer::get_data_size() const {
		return data_size;
	}

	gpu_buffer_type wm_dx11_buffer::get_type_flags() const {
		return type_flags;
	}

	usage_frequency wm_dx11_buffer::get_cpu_write_frequency() const {
		return cpu_write_frequency;
	}

#ifdef WM_BUILD_DEBUG

	void wm_dx11_buffer::set_debug_label(const std::string& label) {
		DX_CALL(native_handle->SetPrivateData(WKPDID_D3DDebugObjectName, label.size(), label.c_str()));
	}

#endif

	void wm_dx11_buffer::set_data(void* data, const int32_t data_size, const int32_t offset) {
		D3D11_MAPPED_SUBRESOURCE mapped_subresourcce{};
		DX_CALL(wm_direct3d11_rendering_context::device_context->Map(native_handle, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresourcce));
		std::memcpy(mapped_subresourcce.pData, data, data_size);
		DX_CALL(wm_direct3d11_rendering_context::device_context->Unmap(native_handle, 0));
	}

	int32_t wm_dx11_buffer::get_stride() const {
		return stride;
	}

	wm_dx11_buffer::~wm_dx11_buffer() {
		if(native_handle != nullptr) {
			native_handle->Release();
			WM_LOG_INFO_2("Direct3D 11 buffer destroyed");
		}
	}

}
