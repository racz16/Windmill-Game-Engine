#include "ptr_meta.h"

namespace wm {

	std::unordered_map<int32_t, ptr_meta> ptr_meta::meta;
	int32_t ptr_meta::last_id = 0;

	ptr_meta& ptr_meta::get(const int32_t id) {
		return meta.at(id);
	}

	int32_t ptr_meta::add(void* raw_pointer) {
		ptr_meta meta;
		meta.raw_pointer = raw_pointer;
		int32_t id = last_id++;
		ptr_meta::meta.insert_or_assign(id, meta);
		return id;
	}

	void ptr_meta::remove(const int32_t id) {
		ptr_meta::meta.erase(id);
	}

	bool ptr_meta::is_valid() const {
		return raw_pointer != nullptr;
	}

	void* ptr_meta::get_raw_pointer() const {
		return raw_pointer;
	}

	void ptr_meta::set_raw_pointer(void* raw_pointer) {
		this->raw_pointer = raw_pointer;
	}

	int32_t ptr_meta::get_array_index() const {
		return array_index;
	}

	void ptr_meta::set_array_index(const int32_t index) {
		array_index = index;
	}

	std::function<void()> ptr_meta::get_array_destroy_callback() const {
		return array_destroy_callback;
	}

	void ptr_meta::set_array_destroy_callback(std::function<void()> array_destroy_callback) {
		this->array_destroy_callback = array_destroy_callback;
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
