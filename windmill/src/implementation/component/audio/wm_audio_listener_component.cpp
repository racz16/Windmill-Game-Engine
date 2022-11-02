#include "wm_audio_listener_component.h"
#include "core/engine.h"

namespace wm {

	wm_audio_listener_component::wm_audio_listener_component(): listener(audio_listener::create()) {}

	void wm_audio_listener_component::update() {
		auto node = get_node();
		if(is_the_main_audio_listener() && is_active() && node.is_valid()) {
			auto transform = node->get_transform();
			glm::vec3 position = transform->get_absolute_position();
			listener->set_position(position);
			listener->set_orientation(transform->get_forward(), transform->get_up());
			listener->set_velocity(position - previous_position);
			previous_position = position;
		}
	}

	const ptr<audio_listener_component> wm_audio_listener_component::get_ptr() const {
		return get_ptr_impl().convert<audio_listener_component>();
	}

	bool wm_audio_listener_component::is_the_main_audio_listener() const {
		if(engine::get_parameters().contains(audio_listener_component::get_main_audio_listener_key())) {
			return engine::get_parameters().get(audio_listener_component::get_main_audio_listener_key()) == get_ptr();
		} else {
			return false;
		}
	}

	wm_audio_listener_component::~wm_audio_listener_component() {
		listener.destroy();
	}

}
