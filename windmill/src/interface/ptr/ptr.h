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
			return ptr_view<T>(this->raw_pointer, this->id);
		}

		template<class X>
		ptr<X> convert() const {
			return ptr<X>(static_cast<X*>(this->raw_pointer), this->id);
		}

		void destroy() {
			base_ptr<T>::destroy();
		}

	};

}
