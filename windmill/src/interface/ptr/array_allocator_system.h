#pragma once

#include "../core/key.h"
#include "vector_array.h"
#include "../core/system.h"

namespace wm {

	class WM_PUBLIC array_allocator_system: public system {
	private:
		std::unordered_map<int32_t, ptr<vector_array<void>>> arrays;

		array_allocator_system();
	public:
		static ptr<array_allocator_system> create();
		static key<array_allocator_system> get_key();

		template<class T>
		bool contains(const key<T> key) {
			return arrays.find(key.get_hash()) != arrays.end();
		}

		template<class T>
		ptr<T> get(const key<T> key, const int32_t index) {
			WM_ASSERT(contains(key));
			auto va = arrays.at(key.get_hash()).template convert<vector_array<T>>();
			return va->get(index);
		}

		template<class T>
		int32_t get_count(const key<T> key) {
			WM_ASSERT(contains(key));
			return arrays.at(key.get_hash())->get_count();
		}

		template<class T, class CT>
		void add_type(const key<T> key) {
			WM_ASSERT(!contains(key));
			auto va = ptr<vector_array<T>>(new vector_array<T>([](const int32_t array_size) {
				return new CT[array_size];
			}, [](std::vector<T*> arrays, const int32_t vector_index, const int32_t array_index, const int32_t id) {
				auto array = static_cast<CT*>(arrays.at(vector_index));
				array[array_index] = CT(id);
			}, [](std::vector<T*> arrays, const int32_t vector_index, const int32_t array_index) {
				auto array = static_cast<CT*>(arrays.at(vector_index));
				return &array[array_index];
			}, [](std::vector<T*> arrays, const int32_t vector_index, const int32_t array_index, const int32_t vector_index_2, const int32_t array_index_2) {
				arrays.at(vector_index)[array_index] = arrays.at(vector_index_2)[array_index_2];
			}, [](T* array) {
				delete[] static_cast<CT*>(array);
			}
			)).template convert<vector_array<void>>();
			arrays.insert_or_assign(key.get_hash(), va);
		}

		template<class T>
		ptr<T> create(const key<T> key) {
			WM_ASSERT(contains(key));
			auto va = arrays.at(key.get_hash()).template convert<vector_array<T>>();
			return va->create();
		}

		template<class T>
		void destroy(const key<T> key, const ptr<T> ptr) {
			WM_ASSERT(contains(key));
			arrays.at(key.get_hash()).template convert<vector_array<T>>()->destroy(ptr);
		}

		virtual ~array_allocator_system();

	};

}
