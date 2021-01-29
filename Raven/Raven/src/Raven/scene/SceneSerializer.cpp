#include <pch.h>
#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <Raven/scene/Entity.h>
#include <Raven/scene/Components.h>
#include <Raven_Core/datatypes/RavenUniqueID.h>
// Conversions
namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<>
		struct convert<glm::vec4> {
		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace rvn {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value) {
		out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value) {
		out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << value.a << YAML::EndSeq;
		return out;
	}
	static void serializeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap;
		if (entity.hasComponent<RUIDComponent>()) {
			out << YAML::Key << "RUID" << YAML::Value << entity.getComponent<RUIDComponent>().ruid;
		}
		else {
			LOG_ENGINE_WARN("Entity should have RUID");
		}
		if (entity.hasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << entity.getComponent<TagComponent>().tag;
			out << YAML::EndMap;
		}
		if (entity.hasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
			auto& comp = entity.getComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << comp.translation;
			out << YAML::Key << "Rotation" << YAML::Value << comp.rotation;
			out << YAML::Key << "Scale" << YAML::Value << comp.scale;
			out << YAML::EndMap;
		}
		if (entity.hasComponent<SpriteRendererComponent>()) {
			out << YAML::Key << "SpriteRendererComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << entity.getComponent<SpriteRendererComponent>().color;
			out << YAML::EndMap;
		}
		if (entity.hasComponent<CameraComponent>()) {
			out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;
			auto& cameraComponent = entity.getComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;
			out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;
			{
				out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
				out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveFOV();
				out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
				out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
				out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
				out << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
				out << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
			}
			out << YAML::EndMap;
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}

	void SceneSerializer::serialize(const ref<Scene>& scene, const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << scene->getName();
		out << YAML::Key << "ClearColor" << YAML::Value << scene->getClearColor();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		scene->_registry.each([&](auto entityID) {
			Entity entity(entityID, scene.get());
			if (!entity) return;
			serializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath, std::ios::binary);
		fout << out.c_str();
	}
	bool SceneSerializer::deserialize(const ref<Scene>& scene, const std::string& filepath)
	{
		YAML::Node data = YAML::LoadFile(filepath);
		if (!data["Scene"])
			return false;

		scene->_name = data["Scene"].as<std::string>();
		LOG_ENGINE_TRACE("Deserializing scene '{0}'", scene->_name);

		auto clearColor = data["ClearColor"];
		if (clearColor) scene->_clearColor = clearColor.as<glm::vec4>();

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {

				auto tagComponent = entity["TagComponent"];

				Entity curEntity;
				if (tagComponent) {
					curEntity = scene->createEntity(tagComponent["Tag"].as<std::string>());
				}
				ASSERT(curEntity.hasComponent<RUIDComponent>(), "Entity doesn't have a RUIDComponent");
				auto ruid = entity["RUID"];
				if (ruid) {
					curEntity.getComponent<RUIDComponent>().ruid = ruid.as<std::uint64_t>();
				}
				else {
					LOG_ENGINE_WARN("Entity should have RUID, it is now set to the default of 0");
					ruid = 0;
				}

				ASSERT(curEntity.hasComponent<TransformComponent>(), "Entity doesn't have a TransformComponent");
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent) {
					auto& tc = curEntity.getComponent<TransformComponent>();
					tc.translation = transformComponent["Translation"].as<glm::vec3>();
					tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent) {
					auto& src = curEntity.addComponent<SpriteRendererComponent>();
					src.color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent) {
					auto& cc = curEntity.addComponent<CameraComponent>();
					auto camera = cameraComponent["Camera"];
					cc.camera.setOrthographic(camera["OrthographicSize"].as<float>(), camera["OrthographicNear"].as<float>(), camera["OrthographicFar"].as<float>());
					cc.camera.setPerspective(camera["PerspectiveFOV"].as<float>(), camera["PerspectiveNear"].as<float>(), camera["PerspectiveFar"].as<float>());
					cc.camera.setProjectionType((SceneCamera::ProjectionType)camera["ProjectionType"].as<int>());
					cc.primary = cameraComponent["Primary"].as<bool>();
					cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}
			}
		}
		else {
			return false;
		}
		return true;
	}
}