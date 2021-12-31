#pragma once

#include "key.h"

namespace wm {

	class parameter_container {
	private:
		std::unordered_map<int32_t, std::any> parameters;
	public:

		template<class T>
		bool contains(const key<T> key) const {
			return parameters.find(key.get_hash()) != parameters.end();
		}

		template<class T>
		T get(const key<T> key) const {
			return std::any_cast<T>(parameters.at(key.get_hash()));
		}

		template<class T>
		T get_or_default(const key<T> key, T default_value) const {
			if(contains<T>(key)) {
				return get<T>(key);
			} else {
				return default_value;
			}
		}

		template<class T>
		void set(const key<T> key, T parameter) {
			parameters.insert_or_assign(key.get_hash(), parameter);
		}

		template<class T>
		void remove(const key<T> key) {
			parameters.erase(key.get_hash());
		}

	};

}
