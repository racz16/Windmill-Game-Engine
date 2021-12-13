#pragma once

#include "../defines/general_defines.h"

namespace wm {

	class WM_PUBLIC ptr_meta {
	private:
		static std::unordered_map<int32_t, ptr_meta> meta;
		static int32_t last_id;

		bool valid = true;
		int32_t reference_count = 1;
	public:
		static int32_t get_next_id();
		static ptr_meta& get_meta(const int32_t id);
		static void add_meta(const int32_t id, ptr_meta meta);
		static void remove_meta(const int32_t id);

		bool is_valid() const;
		void invalidate();
		int32_t get_reference_count() const;
		void increase_reference_count();
		void decrease_reference_count();
	};

}
