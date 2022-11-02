#include "wm_audio_system.h"
#include "defines/log_defines.h"

namespace wm {

	wm_audio_system::wm_audio_system(): context(audio_context::create()) {
		WM_LOG_INFO_1("audio system created");
	}

	void wm_audio_system::update() {
		context->update_listener();
		for(auto audio_source_component : audio_source_components) {
			context->update_source(audio_source_component);
		}
	}

	void wm_audio_system::add_audio_source_component(const ptr<audio_source_component> audio_source_component) {
		WM_ASSERT(audio_source_component.is_valid() && audio_source_component->get_node().is_valid());
		WM_ASSERT(std::find(audio_source_components.begin(), audio_source_components.end(), audio_source_component) == audio_source_components.end());
		audio_source_components.push_back(audio_source_component);
	}

	void wm_audio_system::remove_audio_source_component(const ptr<audio_source_component> audio_source_component) {
		WM_ASSERT(!audio_source_component.is_valid() || !audio_source_component->get_node().is_valid());
		auto find_source = std::find(audio_source_components.begin(), audio_source_components.end(), audio_source_component);
		WM_ASSERT(find_source != audio_source_components.end());
		audio_source_components.erase(find_source);
	}

	float wm_audio_system::get_volume() const {
		return context->get_volume();
	}

	void wm_audio_system::set_volume(const float volume) {
		context->set_volume(volume);
	}

	float wm_audio_system::get_doppler_factor() const {
		return context->get_doppler_factor();
	}

	void wm_audio_system::set_doppler_factor(const float doppler_factor) {
		context->set_doppler_factor(doppler_factor);
	}

	float wm_audio_system::get_speed_of_sound() const {
		return context->get_speed_of_sound();
	}

	void wm_audio_system::set_speed_of_sound(const float speed_of_sound) {
		context->set_speed_of_sound(speed_of_sound);
	}

	audio_distance_model wm_audio_system::get_distance_model() const {
		return context->get_distance_model();
	}

	void wm_audio_system::set_distance_model(const audio_distance_model distance_model) {
		context->set_distance_model(distance_model);
	}

	std::string wm_audio_system::get_vendor() const {
		return context->get_vendor();
	}

	std::string wm_audio_system::get_version() const {
		return context->get_version();
	}

	std::string wm_audio_system::get_renderer() const {
		return context->get_renderer();
	}

	wm_audio_system::~wm_audio_system() {
		for(auto audio_source_component : audio_source_components) {
			audio_source_component.destroy();
		}
		audio_source_components.clear();
		context.destroy();

		WM_LOG_INFO_1("audio system destroyed");
	}

}
