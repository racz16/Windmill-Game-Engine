#pragma once

#include "../core/key.h"
#include "vector_array.h"

namespace wm {

	class WM_PUBLIC array_allocator {
	private:
		static std::unordered_map<int32_t, ptr<vector_array<void>>> arrays;
	public:

		template<class T>
		static bool contains(const key<T> key) {
			return arrays.find(key.get_hash()) != arrays.end();
		}

		template<class T>
		static ptr<T> get(const key<T> key, const int32_t index) {
			WM_ASSERT(contains(key));
			auto va = arrays.at(key.get_hash()).template convert<vector_array<T>>();
			return va->get(index);
		}

		template<class T>
		static int32_t get_count(const key<T> key) {
			WM_ASSERT(contains(key));
			return arrays.at(key.get_hash())->get_count();
		}

		template<class T, class CT>
		static void add_type(const key<T> key) {
			WM_ASSERT(!contains(key));
			auto va = ptr<vector_array<T>>(new vector_array<T>(
				[](const int32_t array_size) {return new CT[array_size]; },
				[](T* array) { delete[] static_cast<CT*>(array); }
			)).template convert<vector_array<void>>();
			arrays.insert_or_assign(key.get_hash(), va);
		}

		template<class T>
		static ptr<T> create(const key<T> key) {
			WM_ASSERT(contains(key));
			auto va = arrays.at(key.get_hash()).template convert<vector_array<T>>();
			return va->create();
		}

		template<class T>
		static void destroy(const key<T> key, const ptr_view<T> ptr) {
			WM_ASSERT(contains(key));
			arrays.at(key.get_hash()).template convert<vector_array<T>>()->destroy(ptr);
		}

		static void destroy();

	};

}
