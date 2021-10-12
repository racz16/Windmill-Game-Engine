#pragma once

#include "base_ptr.h"
#include "ptr.h"

namespace wm {

	template<class T>
	class ptr_view: public base_ptr<T> {
	public:

		using base_ptr::base_ptr;

		template<class X>
		ptr_view<X> convert() const {
			return ptr_view<X>(static_cast<X*>(raw_pointer), id);
		}

	};

}
