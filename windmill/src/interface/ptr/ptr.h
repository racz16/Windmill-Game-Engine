#pragma once

#include "ptr_view.h"

namespace wm {

	template<class T>
	class ptr: public ptr_view<T> {
	public:

		using ptr_view<T>::ptr_view;

		template<class X>
		ptr<X> convert() const {
			return ptr<X>(ptr_view<T>::id);
		}

		void destroy() {
			auto& meta = ptr_meta::get(ptr_view<T>::id);
			ptr_view<T>::destroy(meta);
		}

	};
}

template<class T>
class WM_PUBLIC std::hash<wm::ptr<T>> {
public:
	size_t operator()(const wm::ptr<T>& ptr) const {
		return ptr.get_id();
	}
};
