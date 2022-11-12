#include <memory>
#include <string>
#include <functional>

#define WM_HIDE_CONSOLE_IN_RELEASE_MODE
#include <windmill/windmill.h>

class player_control_system: public wm::system {
private:
	float movement_speed = 0.01f;
	float rotation_speed = 0.05f;
public:

	void update() override {
		auto camera = wm::engine::get_scene_system()->get_node_by_tag("camera")->get_component(wm::camera_component::get_key());
		auto camera_transform = camera->get_node()->get_transform();
		float delta_time = wm::engine::get_time_system()->get_delta_time();
		auto input_handler = wm::engine::get_window_system()->get_input_handler();
		auto forward_movement = delta_time * movement_speed * camera_transform->get_forward();
		auto right_movement = delta_time * movement_speed * camera_transform->get_right();
		auto up = glm::vec3(0.0f, 1.0f, 0.0f);

		if(input_handler->get_keyboard_button_state(wm::keyboard_button::button_w).is_down()) {
			camera_transform->move(forward_movement);
		}
		if(input_handler->get_keyboard_button_state(wm::keyboard_button::button_a).is_down()) {
			camera_transform->move(-right_movement);
		}
		if(input_handler->get_keyboard_button_state(wm::keyboard_button::button_s).is_down()) {
			camera_transform->move(-forward_movement);
		}
		if(input_handler->get_keyboard_button_state(wm::keyboard_button::button_d).is_down()) {
			camera_transform->move(right_movement);
		}
		if(input_handler->get_keyboard_button_state(wm::keyboard_button::button_q).is_down()) {
			camera_transform->rotate(up, rotation_speed);
		}
		if(input_handler->get_keyboard_button_state(wm::keyboard_button::button_e).is_down()) {
			camera_transform->rotate(up, -rotation_speed);
		}
	}

};

class example_1_application: public wm::application {

	using wm::application::application;

	void add_log_system() override {
		const auto ls = wm::log_system::create();
		const auto clw = wm::log_writer::create_console_log_writer(wm::log_level::debug);
		ls->add_log_writer(clw);
		wm::engine::set_system(wm::log_system::get_key(), ls);
		WM_LOG_INFO_1("log system added");
	}

	void add_audio_listener(const wm::ptr<wm::node> node) {
		auto audio_listener_component = wm::audio_listener_component::create();
		node->add_component(audio_listener_component, wm::audio_listener_component::get_key());
		wm::engine::get_parameters().set(wm::audio_listener_component::get_main_audio_listener_key(), audio_listener_component);
	}

	void add_audio_source(const wm::ptr<wm::node> node) {
		auto sound = wm::engine::get_resource_system()->get_sound("res/audio/test.wav");
		auto audio_buffer = wm::audio_buffer::create();
		audio_buffer->set_sound(sound);
		auto audio_source_component = wm::audio_source_component::create();
		audio_source_component->set_buffer(audio_buffer);
		node->add_component(audio_source_component, wm::audio_source_component::get_key());
		audio_source_component->play();
	}

	void initialize() override {
		wm::application::initialize();

		wm::key<player_control_system> key("WM_PLAYER_CONTROL_SYSTEM");
		wm::engine::set_system(key, wm::ptr<player_control_system>(new player_control_system()));

		wm::engine::get_window_system()->set_icon("res/texture/checked.png");

		auto node = wm::node::create();
		node->add_tag("parent");
		auto node_2 = wm::node::create();
		node_2->add_tag("child");
		node->add_child(node_2);
		node_2->get_transform()->set_relative_rotation(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
		node_2->get_transform()->set_relative_position(glm::vec3(2.0f, 0.0f, 0.0f));

		auto node_3 = wm::node::create();
		node->add_child(node_3);

		auto camera_node = wm::node::create();
		camera_node->get_transform()->set_relative_position(glm::vec3(0.0f, 0.0f, 5.0f));
		camera_node->add_tag("camera");
		auto camera_component = wm::camera_component::create();
		camera_node->add_component(camera_component, wm::camera_component::get_key());

		//add_audio_listener(camera_node);
		//add_audio_source(node_2);

		wm::engine::get_event_system()->add_event_listener<wm::keyboard_button_event>(wm::keyboard_button_event::get_key(), [camera_node](const wm::keyboard_button_event event) {
			if(event.get_button() == wm::keyboard_button::button_escape) {
				wm::engine::get_window_system()->set_closing(true);
			}
		});

		auto framebuffer_size = wm::engine::get_window_system()->get_framebuffer_size();
		camera_component->set_scale(glm::vec2(framebuffer_size.x, framebuffer_size.y));
		wm::engine::get_event_system()->add_event_listener<wm::window_framebuffer_size_event>(wm::window_framebuffer_size_event::get_key(), [camera_component](const wm::window_framebuffer_size_event event) {
			camera_component->set_scale(glm::vec2(event.get_new_size().x, event.get_new_size().y));
		});
	}

};

int main() {
	example_1_application app("example 1");
	app.run();
}
