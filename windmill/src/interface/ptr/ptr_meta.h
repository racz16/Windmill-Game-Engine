#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC ptr_meta {
	private:
		static std::unordered_map<int32_t, ptr_meta> meta;
		static int32_t last_id;

		void* raw_pointer;
		bool valid = true;
		int32_t reference_count = 1;
		int32_t array_index = -1;
		std::function<void ()> array_destroy_callback;
	public:
		static ptr_meta& get(const int32_t id);
		static int32_t add(void* raw_pointer);
		static void remove(const int32_t id);

		bool is_valid() const;
		void invalidate();
		void* get_raw_pointer() const;
		void set_raw_pointer(void* raw_pointer);
		int32_t get_array_index() const;
		void set_array_index(const int32_t index);
		std::function<void()> get_array_destroy_callback() const;
		void set_array_destroy_callback(std::function<void ()> array_destroy_callback);
		int32_t get_reference_count() const;
		void increase_reference_count();
		void decrease_reference_count();
	};

}
