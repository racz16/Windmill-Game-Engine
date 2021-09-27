#pragma once

#include "ptr_meta.h"
#include "defines.h"

namespace wm {

	template<class T>
	class ptr {
	private:
		T* raw_pointer;
		int32_t id;
	public:

		ptr(T* const raw_pointer): raw_pointer(raw_pointer), id(ptr_meta::get_next_id()) {
			const ptr_meta pm;
			ptr_meta::add_meta(id, pm);
		}

		ptr(T* const raw_pointer, const int32_t id): raw_pointer(raw_pointer), id(id) {
			ptr_meta::get_meta(id).increase_reference_count();
		}

		ptr(const ptr& other_ptr) {
			raw_pointer = other_ptr.raw_pointer;
			id = other_ptr.id;
			ptr_meta::get_meta(id).increase_reference_count();
		}

		ptr& operator=(const ptr& other_ptr) {
			auto& pm = ptr_meta::get_meta(id);
			pm.decrease_reference_count();
			if(pm.get_reference_count() <= 0 && id != other_ptr.id) {
				destroy();
				ptr_meta::remove_meta(id);
			}

			raw_pointer = other_ptr.raw_pointer;
			id = other_ptr.id;

			pm = ptr_meta::get_meta(id);
			pm.increase_reference_count();

			return *this;
		}

		T* operator->() const {
			WM_ASSERT(ptr_meta::get_meta(id).is_valid());
			return raw_pointer;
		}

		int32_t get_id() const {
			return id;
		}

		int32_t get_reference_count() const {
			return ptr_meta::get_meta(id).get_reference_count();
		}

		int32_t is_valid() const {
			return ptr_meta::get_meta(id).is_valid();
		}

		template<class X>
		ptr<X> convert() const {
			return ptr<X>(static_cast<X*>(raw_pointer), id);
		}

		void destroy() {
			auto& meta = ptr_meta::get_meta(id);
			if(meta.is_valid()) {
				delete raw_pointer;
				meta.invalidate();
			}
		}

		~ptr() {
			auto& pm = ptr_meta::get_meta(id);
			pm.decrease_reference_count();
			if(pm.get_reference_count() <= 0) {
				destroy();
				ptr_meta::remove_meta(id);
			}
		}

	};

}
