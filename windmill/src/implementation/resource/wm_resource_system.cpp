#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "defines/debug_defines.h"
#include "resource/vertex.h"
#include "defines/log_defines.h"

#include "wm_resource_system.h"

namespace wm {

	wm_resource_system::wm_resource_system() {
		WM_LOG_INFO_1("resource system created");
	}

	ptr<image> wm_resource_system::get_image(const std::string& file_path) {
		int32_t width;
		int32_t height;
		int32_t channel_count;
		auto pixels = stbi_load(file_path.c_str(), &width, &height, &channel_count, STBI_rgb_alpha);
		WM_ASSERT(pixels);
		WM_LOG_INFO_2("image " + file_path + " loaded");
		return ptr<image>(new image(glm::ivec2(width, height), channel_count, pixels));
	}

	ptr<mesh> wm_resource_system::get_mesh(const std::string& file_path) {
		Assimp::Importer a_importer;
		auto a_scene = a_importer.ReadFile(file_path, aiProcess_JoinIdenticalVertices | aiProcess_MakeLeftHanded | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
		if(a_scene == nullptr || (a_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == AI_SCENE_FLAGS_INCOMPLETE || a_scene->mRootNode == nullptr) {
			WM_THROW_ERROR(a_importer.GetErrorString());
		}

		std::vector<vertex> vertices;
		std::vector<uint32_t> indices;
		auto a_mesh = a_scene->mMeshes[0];
		bool has_texture_coordinates = a_mesh->HasTextureCoords(0);
		for(int32_t i = 0; i < a_mesh->mNumVertices; i++) {
			auto a_vertex = a_mesh->mVertices[i];
			auto a_normal = a_mesh->mNormals[i];
			auto a_texture_coordinate = a_mesh->mTextureCoords[0][i];

			auto position = glm::vec3(a_vertex.x, a_vertex.y, a_vertex.z);
			auto normal = glm::vec3(a_normal.x, a_normal.y, a_normal.z);
			auto texture_coordinate = has_texture_coordinates ? glm::vec2(a_texture_coordinate.x, a_texture_coordinate.y) : glm::vec2(0.0f);

			vertices.push_back(vertex(position, normal, texture_coordinate));
		}
		for(int32_t i = 0; i < a_mesh->mNumFaces; i++) {
			auto& a_face = a_mesh->mFaces[i];
			for(int32_t j = 0; j < a_face.mNumIndices; j++) {
				indices.push_back(a_face.mIndices[j]);
			}
		}

		WM_LOG_INFO_2("mesh " + file_path + " loaded");
		return ptr<mesh>(new mesh(vertices, indices, has_texture_coordinates));
	}

	ptr<sound> wm_resource_system::get_sound(const std::string& file_path) {
		SF_INFO info;
		auto file = sf_open(file_path.c_str(), SFM_READ, &info);
		WM_ASSERT(file);

		std::vector<uint16_t> data;
		std::array<int16_t, 4096> read_buffer{};
		size_t read_size = 0;
		while((read_size = sf_read_short(file, read_buffer.data(), read_buffer.size())) != 0) {
			data.insert(data.end(), read_buffer.begin(), read_buffer.begin() + read_size);
		}
		sf_close(file);
		WM_LOG_INFO_2("sound " + file_path + " loaded");
		return ptr<sound>(new sound(data, info.samplerate, info.channels, 1.0f * info.frames / info.samplerate));
	}

	wm_resource_system::~wm_resource_system() {
		WM_LOG_INFO_1("resource system destroyed");
	}

}
