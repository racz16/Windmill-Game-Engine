#include "ptr_meta.h"

namespace wm {

	std::unordered_map<int32_t, ptr_meta> ptr_meta::meta;
	int32_t ptr_meta::last_id = 0;

	int32_t ptr_meta::get_next_id() {
		return ++last_id;
	}

	ptr_meta& ptr_meta::get_meta(const int32_t id) {
		return meta.at(id);
	}

	void ptr_meta::add_meta(const int32_t id, ptr_meta meta) {
		ptr_meta::meta.insert_or_assign(id, meta);
	}

	void ptr_meta::remove_meta(const int32_t id) {
		ptr_meta::meta.erase(id);
	}

	bool ptr_meta::is_valid() const {
		return valid;
	}

	void ptr_meta::invalidate() {
		valid = false;
	}

	int32_t ptr_meta::get_reference_count() const {
		return reference_count;
	}

	void ptr_meta::increase_reference_count() {
		reference_count++;
	}

	void ptr_meta::decrease_reference_count() {
		reference_count--;
	}

}
