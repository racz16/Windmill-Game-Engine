#pragma once

#include "ptr_meta.h"
#include "../defines/debug_defines.h"

namespace wm {

	template<class T>
	class base_ptr {
	protected:
		T* raw_pointer;
		int32_t id;

		void destroy() {
			if(is_valid()) {
				delete raw_pointer;
				ptr_meta::get_meta(id).invalidate();
			}
		}

	public:

		base_ptr<T>(T* const raw_pointer): raw_pointer(raw_pointer), id(ptr_meta::get_next_id()) {
			const ptr_meta pm;
			ptr_meta::add_meta(id, pm);
		}

		base_ptr<T>(T* const raw_pointer, const int32_t id) : raw_pointer(raw_pointer), id(id) {
			ptr_meta::get_meta(id).increase_reference_count();
		}

		base_ptr<T>(const base_ptr<T>& other_ptr) {
			raw_pointer = other_ptr.raw_pointer;
			id = other_ptr.id;
			ptr_meta::get_meta(id).increase_reference_count();
		}

		base_ptr<T>& operator=(const base_ptr<T>& other_ptr) {
			auto& pm = ptr_meta::get_meta(id);
			pm.decrease_reference_count();
			if(pm.get_reference_count() <= 0 && id != other_ptr.id) {
				destroy();
				ptr_meta::remove_meta(id);
			}

			raw_pointer = other_ptr.raw_pointer;
			id = other_ptr.id;

			auto& pm2 = ptr_meta::get_meta(id);
			pm2.increase_reference_count();

			return *this;
		}

		T* operator->() const {
			WM_ASSERT(is_valid());
			return raw_pointer;
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
			return ptr_meta::get_meta(id).get_reference_count();
		}

		int32_t is_valid() const {
			return raw_pointer != nullptr && ptr_meta::get_meta(id).is_valid();
		}

		~base_ptr<T>() {
			auto& pm = ptr_meta::get_meta(id);
			pm.decrease_reference_count();
			if(pm.get_reference_count() <= 0) {
				destroy();
				ptr_meta::remove_meta(id);
			}
		}

	};
}
