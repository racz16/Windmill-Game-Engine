#include "wm_audio_source_component.h"
#include "core/engine.h"

namespace wm {

	void wm_audio_source_component::initialize(const int32_t id) {
		component::initialize(id);
		source = audio_source::create();
	}

	void wm_audio_source_component::added_to_node(const ptr<node> node) {
		engine::get_audio_system()->add_audio_source_component(get_ptr());
	}

	void wm_audio_source_component::removed_from_node(const ptr<node> node) {
		pause();
		engine::get_audio_system()->remove_audio_source_component(get_ptr());
	}

	void wm_audio_source_component::update() {
		auto node = get_node();
		if(is_active() && node.is_valid()) {
			auto transform = node->get_transform();
			glm::vec3 position = transform->get_absolute_position();
			source->set_position(position);
			source->set_forward(transform->get_forward());
			source->set_velocity(position - previous_position);
			previous_position = position;
		}
	}

	ptr<audio_source_component> wm_audio_source_component::get_ptr() const {
		return get_ptr_impl().convert<audio_source_component>();
	}

	std::any wm_audio_source_component::get_native_id() const {
		return source->get_native_id();
	}

	ptr<audio_buffer> wm_audio_source_component::get_buffer() const {
		return source->get_buffer();
	}

	void wm_audio_source_component::set_buffer(const ptr<wm::audio_buffer> buffer) {
		source->set_buffer(buffer);
	}

	ptr<sound> wm_audio_source_component::get_sound() const {
		return get_buffer()->get_sound();
	}

	float wm_audio_source_component::get_volume() const {
		return source->get_volume();
	}

	void wm_audio_source_component::set_volume(const float volume) {
		source->set_volume(volume);
	}

	float wm_audio_source_component::get_speed() const {
		return source->get_speed();
	}

	void wm_audio_source_component::set_speed(const float speed) {
		source->set_speed(speed);
	}

	bool wm_audio_source_component::is_loop() const {
		return source->is_loop();
	}

	void wm_audio_source_component::set_loop(const bool loop) {
		source->set_loop(loop);
	}

	void wm_audio_source_component::play() const {
		WM_ASSERT(get_node().is_valid() && is_active());
		source->play();
	}

	void wm_audio_source_component::stop() const {
		source->stop();
	}

	void wm_audio_source_component::pause() const {
		source->pause();
	}

	void wm_audio_source_component::rewind() const {
		source->rewind();
	}

	audio_source_state wm_audio_source_component::get_state() const {
		return source->get_state();
	}

	glm::vec3 wm_audio_source_component::get_position() const {
		return source->get_position();
	}

	glm::vec3 wm_audio_source_component::get_forward() const {
		return source->get_forward();
	}

	glm::vec3 wm_audio_source_component::get_velocity() const {
		return source->get_velocity();
	}

	float wm_audio_source_component::get_max_distance() const {
		return source->get_max_distance();
	}

	void wm_audio_source_component::set_max_distance(const float max_distance) {
		source->set_max_distance(max_distance);
	}

	float wm_audio_source_component::get_rolloff_factor() const {
		return source->get_rolloff_factor();
	}

	void wm_audio_source_component::set_rolloff_factor(const float rolloff_factor) {
		source->set_rolloff_factor(rolloff_factor);
	}

	float wm_audio_source_component::get_reference_distance() const {
		return source->get_reference_distance();
	}

	void wm_audio_source_component::set_reference_distance(const float reference_distance) {
		source->set_reference_distance(reference_distance);
	}

	float wm_audio_source_component::get_min_volume() const {
		return source->get_min_volume();
	}

	void wm_audio_source_component::set_min_volume(const float min_volume) {
		source->set_min_volume(min_volume);
	}

	float wm_audio_source_component::get_max_volume() const {
		return source->get_max_volume();
	}

	void wm_audio_source_component::set_max_volume(const float max_volume) {
		source->set_max_volume(max_volume);
	}

	float wm_audio_source_component::get_cone_outer_volume() const {
		return source->get_cone_outer_volume();
	}

	void wm_audio_source_component::set_cone_outer_volume(const float cone_outer_volume) {
		source->set_cone_outer_volume(cone_outer_volume);
	}

	float wm_audio_source_component::get_cone_inner_angle() const {
		return source->get_cone_inner_angle();
	}

	void wm_audio_source_component::set_cone_inner_angle(const float cone_inner_angle) {
		source->set_cone_inner_angle(cone_inner_angle);
	}

	float wm_audio_source_component::get_cone_outer_angle() const {
		return source->get_cone_outer_angle();
	}

	void wm_audio_source_component::set_cone_outer_angle(const float cone_outer_angle) {
		source->set_cone_outer_angle(cone_outer_angle);
	}

	wm_audio_source_component::~wm_audio_source_component() {
		source.destroy();
	}

}
