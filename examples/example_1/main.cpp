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

		auto left_y = input_handler->get_gamepad_axis_state(0, wm::gamepad_axis::axis_left_y);
		auto left_x = input_handler->get_gamepad_axis_state(0, wm::gamepad_axis::axis_left_x);
		auto right_x = input_handler->get_gamepad_axis_state(0, wm::gamepad_axis::axis_right_x);

		auto gamepad_forward = std::max(-left_y.get_value(), 0.0f);
		auto keyboard_forward = static_cast<float>(input_handler->get_keyboard_button_state(wm::keyboard_button::button_w).is_down());
		auto forward = std::max(gamepad_forward, keyboard_forward);

		auto gamepad_backward = std::min(-left_y.get_value(), 0.0f);
		auto keyboard_backward = -static_cast<float>(input_handler->get_keyboard_button_state(wm::keyboard_button::button_s).is_down());
		auto backward = std::min(gamepad_backward, keyboard_backward);

		auto gamepad_right = std::max(left_x.get_value(), 0.0f);
		auto keyboard_right = static_cast<float>(input_handler->get_keyboard_button_state(wm::keyboard_button::button_d).is_down());
		auto right = std::max(gamepad_right, keyboard_right);

		auto gamepad_left = std::min(left_x.get_value(), 0.0f);
		auto keyboard_left = -static_cast<float>(input_handler->get_keyboard_button_state(wm::keyboard_button::button_a).is_down());
		auto left = std::min(gamepad_left, keyboard_left);

		auto movement = (forward + backward) * forward_movement + (right + left) * right_movement;
		camera_transform->move(movement);

		auto gamepad_left_rotation = std::max(-right_x.get_value(), 0.0f);
		auto keyboard_left_rotation = static_cast<float>(input_handler->get_keyboard_button_state(wm::keyboard_button::button_q).is_down());
		auto left_rotation = std::max(gamepad_left_rotation, keyboard_left_rotation);

		auto gamepad_right_rotation = std::min(-right_x.get_value(), 0.0f);
		auto keyboard_right_rotation = -static_cast<float>(input_handler->get_keyboard_button_state(wm::keyboard_button::button_e).is_down());
		auto right_rotation = std::min(gamepad_right_rotation, keyboard_right_rotation);

		auto rotation = (left_rotation + right_rotation) * delta_time * rotation_speed;
		camera_transform->rotate(up, rotation);
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
		wm::rendering_system::set_rendering_api(wm::rendering_api::direct3d11);
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
			if(event.get_new_size().x > 0 && event.get_new_size().y > 0) {
				camera_component->set_scale(glm::vec2(event.get_new_size().x, event.get_new_size().y));
			}
		});
	}

};

int main() {
	example_1_application app("example 1");
	app.run();
}
