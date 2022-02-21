#pragma once

#include "ptr_meta.h"
#include "../defines/debug_defines.h"

namespace wm {

	template<class T>
	class base_ptr {
	protected:
		int32_t id;

		T* get_raw_pointer(ptr_meta& meta) const {
			return static_cast<T*>(meta.get_raw_pointer());
		}

		bool is_valid(ptr_meta& meta) const {
			return meta.get_raw_pointer() != nullptr && meta.is_valid();
		}

		void destroy() {
			auto& meta = ptr_meta::get(id);
			if(is_valid(meta)) {
				if(meta.get_array_index() == -1) {
					delete get_raw_pointer(meta);
					meta.invalidate();
					meta.set_raw_pointer(nullptr);
				} else {
					meta.get_array_destroy_callback()();
				}
			}
		}

	public:

		base_ptr<T>(T* const raw_pointer) {
			id = ptr_meta::add(raw_pointer);
		}

		base_ptr<T>(const int32_t id): id(id) {
			ptr_meta::get(id).increase_reference_count();
		}

		base_ptr<T>(const base_ptr<T>& other_ptr): id(other_ptr.id) {
			ptr_meta::get(id).increase_reference_count();
		}

		base_ptr<T>& operator=(const base_ptr<T>& other_ptr) {
			auto& pm = ptr_meta::get(id);
			pm.decrease_reference_count();
			if(pm.get_reference_count() <= 0 && id != other_ptr.id) {
				destroy();
				ptr_meta::remove(id);
			}

			id = other_ptr.id;

			ptr_meta::get(id).increase_reference_count();

			return *this;
		}

		T* operator->() const {
			auto& meta = ptr_meta::get(id);
			WM_ASSERT(is_valid(meta));
			return get_raw_pointer(meta);
		}

		template<class X>
		bool operator==(const base_ptr<X>& other_ptr) const {
			return get_id() == other_ptr.get_id();
		}

		template<class X>
		bool operator!=(const base_ptr<X>& other_ptr) const {
			return !(*this == other_ptr);
		}

		int32_t get_id() const {
			return id;
		}

		int32_t get_reference_count() const {
			return ptr_meta::get(id).get_reference_count();
		}

		bool is_valid() const {
			auto& meta = ptr_meta::get(id);
			return is_valid(meta);
		}

		~base_ptr<T>() {
			auto& pm = ptr_meta::get(id);
			pm.decrease_reference_count();
			if(pm.get_reference_count() <= 0) {
				destroy();
				ptr_meta::remove(id);
			}
		}

	};
}
