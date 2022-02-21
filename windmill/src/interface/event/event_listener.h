#pragma once

namespace wm {

	template<class T>
	class event_listener {
	private:
		std::function<void(const T)> callback_function;
	public:

		event_listener<T>(const std::function<void(const T)> callback_function): callback_function(callback_function) { }

		void callback(T event) const {
			callback_function(event);
		}

	};

}
