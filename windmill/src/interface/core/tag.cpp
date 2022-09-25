#include "tag.h"

namespace wm {

	tag::tag(const std::string& name) {
		hash = utility::hash(name);
		this->name = name;
	}

	tag::tag(const char* name): tag(std::string(name)) {}

	bool tag::operator==(const tag& other_tag) const {
		return get_hash() == other_tag.get_hash();
	}

	bool tag::operator!=(const tag& other_tag) const {
		return !(*this == other_tag);
	}

	int32_t tag::get_hash() const {
		return hash;
	}

	std::string tag::get_name() const {
		return name;
	}

}

size_t std::hash<wm::tag>::operator()(const wm::tag& tag) const {
	return tag.get_hash();
}
