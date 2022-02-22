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

		base_ptr<T>(T* const raw_pointer) {
			id = ptr_meta::add(raw_pointer);
		}

		base_ptr<T>(const int32_t id): id(id) {
			increase();
		}

		base_ptr<T>(const base_ptr<T>& other_ptr) : id(other_ptr.id) {
			increase();
		}

		base_ptr<T>& operator=(const base_ptr<T>& other_ptr) {
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
			return ptr_meta::get(id).is_valid();
		}

		~base_ptr<T>() {
			decrease();
		}

	};
}
