#pragma once

#include "base_ptr.h"
#include "ptr_view.h"

namespace wm {

	template<class T>
	class ptr: public base_ptr<T> {
	public:

		using base_ptr::base_ptr;

		ptr_view<T> to_ptr_view() const {
			*this;
		}

		operator ptr_view<T>() const {
			return ptr_view<T>(raw_pointer, id);
		}

		template<class X>
		ptr<X> convert() const {
			return ptr<X>(static_cast<X*>(raw_pointer), id);
		}

		void destroy() {
			base_ptr::destroy();
		}

	};

}
