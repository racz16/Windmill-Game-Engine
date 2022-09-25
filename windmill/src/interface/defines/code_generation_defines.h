#pragma once

#define WM_CREATE(interface, implementation) \
	ptr<interface> interface::create() { \
		return ptr<interface>(new implementation()); \
	}



#define WM_CREATE_WITH_INIT(interface, implementation) \
	ptr<interface> interface::create() { \
		auto raw_pointer = new implementation(); \
		auto pointer = ptr<interface>(raw_pointer); \
		const int32_t id = pointer.get_id(); \
		raw_pointer->initialize(id); \
		return pointer; \
	} \

#define WM_GET_PTR(interface, class) \
	const ptr<interface> class::get_ptr() const { \
		if(id == -1) { \
			return ptr<interface>(nullptr); \
		} else { \
			return ptr<interface>(id); \
		} \
	}



#define WM_CREATE_ARRAY_ALLOCATOR(interface, implementation) \
	bool interface::type_added = false; \
	ptr<interface> interface::create() { \
		auto array_allocator_system = engine::get_array_allocator_system(); \
		if(!type_added) { \
			array_allocator_system->add_type<interface, implementation>(get_key()); \
			type_added = true; \
		} \
		return array_allocator_system->create<interface>(get_key()); \
	}



#define WM_GET_KEY(interface, key_name) \
	key<interface> interface::get_key() { \
		static const wm::key<interface> key(key_name); \
		return key; \
	}
