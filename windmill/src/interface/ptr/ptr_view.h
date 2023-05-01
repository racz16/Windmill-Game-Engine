#pragma once

#include "ptr_meta.h"
#include "../defines/debug_defines.h"

namespace wm {

	template<class T>
	class ptr_view {
	protected:
		int32_t id;

		T* get_raw_pointer(ptr_meta& meta) const {
			return static_cast<T*>(meta.get_raw_pointer());
		}

		void increase() const {
			ptr_meta::get(id).increase_reference_count();
		}

		void decrease() const {
			auto& meta = ptr_meta::get(id);
			meta.decrease_reference_count();
			if(meta.get_reference_count() <= 0) {
				destroy(meta);
				ptr_meta::remove(id);
			}
		}

		void destroy(ptr_meta& meta) const {
			if(meta.is_valid()) {
				if(meta.get_array_index() == -1) {
					delete get_raw_pointer(meta);
				} else {
					meta.get_array_destroy_callback()();
				}
				meta.set_raw_pointer(nullptr);
				meta.set_array_index(-1);
			}
		}

	public:

		ptr_view<T>(T* const raw_pointer = nullptr) {
			id = ptr_meta::add(raw_pointer);
		}

		ptr_view<T>(const int32_t id): id(id) {
			increase();
		}

		ptr_view<T>(const ptr_view<T>& other_ptr) : id(other_ptr.id) {
			increase();
		}

		ptr_view<T>& operator=(const ptr_view<T>& other_ptr) {
			other_ptr.increase();
			decrease();
			id = other_ptr.id;
			return *this;
		}

		T* operator->() const {
			auto& meta = ptr_meta::get(id);
			WM_ASSERT(meta.is_valid());
			return get_raw_pointer(meta);
		}

		template<class X>
		bool operator==(const ptr_view<X>& other_ptr) const {
			return get_id() == other_ptr.get_id();
		}

		template<class X>
		bool operator!=(const ptr_view<X>& other_ptr) const {
			return !(*this == other_ptr);
		}

		int32_t get_id() const {
			return id;
		}

		int32_t get_reference_count() const {
			return ptr_meta::get(id).get_reference_count();
		}

		bool is_valid() const {
			return ptr_meta::get(id).is_valid();
		}

		template<class X>
		ptr_view<X> convert() const {
			return ptr_view<X>(id);
		}

		virtual ~ptr_view<T>() {
			decrease();
		}

	};
}

template<class T>
class WM_PUBLIC std::hash<wm::ptr_view<T>> {
public:
	size_t operator()(const wm::ptr_view<T>& ptr) const {
		return ptr.get_id();
	}
};
