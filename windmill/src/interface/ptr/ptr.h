#pragma once

#include "ptr_view.h"

namespace wm {

	template<class T>
	class ptr: public base_ptr<T> {
	public:

		using base_ptr<T>::base_ptr;

		ptr_view<T> to_ptr_view() const {
			return *this;
		}

		operator ptr_view<T>() const {
			return ptr_view<T>(this->id);
		}

		template<class X>
		ptr<X> convert() const {
			return ptr<X>(this->id);
		}

		void destroy() {
			auto& meta = ptr_meta::get(base_ptr<T>::id);
			base_ptr<T>::destroy(meta);
		}

	};

}
