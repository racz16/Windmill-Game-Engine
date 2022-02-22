#pragma once

#include "ptr.h"

namespace wm {

	template<class T>
	class WM_PUBLIC vector_array {
	private:
		std::vector<T*> arrays;
		std::vector<ptr<T>> pointers;
		int32_t array_size;
		std::function<void* (const int32_t)> allocation_function;
		std::function<void(T*)> deallocation_function;

		void destroy(const int32_t id) {
			auto& meta = ptr_meta::get(id);
			meta.set_raw_pointer(nullptr);

			int32_t index = meta.get_array_index();
			int32_t last_item_index = pointers.size() - 1;
			int32_t vector_index2 = last_item_index / array_size;
			int32_t array_index2 = last_item_index % array_size;

			if(index != last_item_index) {
				int32_t vector_index = index / array_size;
				int32_t array_index = index % array_size;

				pointers.at(index) = pointers.at(last_item_index);
				arrays.at(vector_index)[array_index] = arrays.at(vector_index2)[array_index2];

				auto ptr2 = pointers.at(index);
				auto& meta2 = ptr_meta::get(ptr2.get_id());
				auto item = &arrays.at(vector_index)[array_index];
				meta2.set_raw_pointer(item);
				meta2.set_array_index(index);
			}
			pointers.pop_back();
			if(array_index2 == 0) {
				deallocation_function(arrays.at(arrays.size() - 1));
				arrays.pop_back();
			}
		}

	public:

		vector_array(std::function<void* (const int32_t)> allocation_function, std::function<void(T*)> deallocation_function, const int32_t array_size = 1024):
			allocation_function(allocation_function), deallocation_function(deallocation_function), array_size(array_size) {
			WM_ASSERT(array_size > 0);
		}

		ptr<T> create() {
			if(pointers.size() % array_size == 0) {
				auto new_array = allocation_function(array_size);
				arrays.push_back(static_cast<T*>(new_array));
			}
			int32_t vector_index = pointers.size() / array_size;
			int32_t array_index = pointers.size() % array_size;
			arrays.at(vector_index)[array_index] = T();
			auto item = &arrays.at(vector_index)[array_index];
			auto pointer = ptr<T>(item);
			auto& meta = ptr_meta::get(pointer.get_id());
			meta.set_array_index(pointers.size());
			int32_t id = pointer.get_id();
			meta.set_array_destroy_callback([this, id]() {
				destroy(id);
			});
			pointers.push_back(pointer);
			return pointer;
		}

		ptr<T> get(const int32_t index) const {
			return pointers.at(index);
		}

		int32_t get_count() const {
			return pointers.size();
		}

		void destroy(const ptr_view<T> ptr) {
			destroy(ptr.get_id());
		}

		virtual ~vector_array() {
			for(auto pointer : pointers) {
				auto& meta = ptr_meta::get(pointer.get_id());
				meta.set_raw_pointer(nullptr);
			}
			pointers.clear();
			for(auto array : arrays) {
				deallocation_function(array);
			}
			arrays.clear();
		}

	};

}
