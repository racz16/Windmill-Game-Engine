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
	ptr<interface> class::get_ptr() const { \
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



#define WM_ENUM_FLAG_FUNCTION_DECLARATIONS(type) \
	type operator|(type, type); \
	type operator&(type, type);

#define WM_ENUM_FLAG_FUNCTION_DEFINITIONS(type) \
	WM_ENUM_FLAG_FUNCTION_DEFINITION(type, |) \
	WM_ENUM_FLAG_FUNCTION_DEFINITION(type, &)

#define WM_ENUM_FLAG_FUNCTION_DEFINITION(type, operator_character) \
	type operator operator_character(type left, type right) { \
		return static_cast<type>( \
			static_cast<std::underlying_type_t<type>>(left) operator_character \
			static_cast<std::underlying_type_t<type>>(right) \
		); \
	}

#define WM_CONTAINS_FLAG(variable, flag) (variable & flag) == flag



#ifdef WM_BUILD_DEBUG
	#define HLSL_VERTEX(name) L"res/shader/hlsl/" name L"/_debug_vertex.hlsl.cso"
	#define HLSL_PIXEL(name) L"res/shader/hlsl/" name L"/_debug_pixel.hlsl.cso"

	#define GLSL_VERTEX(name) "res/shader/glsl/" name "/_debug_vertex.vert.spv"
	#define GLSL_FRAGMENT(name) "res/shader/glsl/" name "/_debug_fragment.frag.spv"
#else
	#define HLSL_VERTEX(name) L"res/shader/hlsl/" name L"/_release_vertex.hlsl.cso"
	#define HLSL_PIXEL(name) L"res/shader/hlsl/" name L"/_release_pixel.hlsl.cso"

	#define GLSL_VERTEX(name) "res/shader/glsl/" name "/_release_vertex.vert.spv"
	#define GLSL_FRAGMENT(name) "res/shader/glsl/" name "/_release_fragment.frag.spv"
#endif
