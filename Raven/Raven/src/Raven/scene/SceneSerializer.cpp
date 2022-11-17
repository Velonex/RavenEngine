#include <pch.h>
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <filesystem>

#include <Raven/scene/Entity.h>
#include <Raven/scene/Components.h>
#include <Raven/utils/NumByteConversion.h>
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

	const std::string SceneSerializer::RUNTIME_SCENEFILE_MAGIC_NUMBER = std::string("RRSC\x00", 5);

#define CAMERA_COMPONENT_FLAG_IS_PRIMARY 1
#define CAMERA_COMPONENT_FLAG_FIXED_ASPECT_RATIO 2
#define CAMERA_COMPONENT_FLAG_PROJECTION_TYPE 3 // 0 : Orthographic, 1 : Perspective

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value) {
		out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value) {
		out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << value.a << YAML::EndSeq;
		return out;
	}
	std::ofstream& operator<<(std::ofstream& out, const glm::vec3& value) {
		out.write(Converter::toBytes<float>(value.x).chars, sizeof(value.x));
		out.write(Converter::toBytes<float>(value.y).chars, sizeof(value.y));
		out.write(Converter::toBytes<float>(value.z).chars, sizeof(value.z));
		return out;
	}
	std::ofstream& operator<<(std::ofstream& out, const glm::vec4& value) {
		out.write(Converter::toBytes<float>(value.x).chars, sizeof(value.x));
		out.write(Converter::toBytes<float>(value.y).chars, sizeof(value.y));
		out.write(Converter::toBytes<float>(value.z).chars, sizeof(value.z));
		out.write(Converter::toBytes<float>(value.w).chars, sizeof(value.w));
		return out;
	}
	std::string& operator+=(std::string& lhs, const glm::vec3& value) {
		lhs.append(Converter::toBytes<float>(value.x).chars, sizeof(value.x));
		lhs.append(Converter::toBytes<float>(value.y).chars, sizeof(value.y));
		lhs.append(Converter::toBytes<float>(value.z).chars, sizeof(value.z));
		return lhs;
	}
	std::string& operator+=(std::string& lhs, const glm::vec4& value) {
		lhs.append(Converter::toBytes<float>(value.x).chars, sizeof(value.x));
		lhs.append(Converter::toBytes<float>(value.y).chars, sizeof(value.y));
		lhs.append(Converter::toBytes<float>(value.z).chars, sizeof(value.z));
		lhs.append(Converter::toBytes<float>(value.w).chars, sizeof(value.w));
		return lhs;
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
			out << YAML::Key << "TextureID" << YAML::Value << entity.getComponent<SpriteRendererComponent>().id;
			out << YAML::Key << "TilingFactor" << YAML::Value << entity.getComponent<SpriteRendererComponent>().tilingFactor;
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
		std::vector<Entity> entities;
		entities.reserve(scene->_registry.alive());
		scene->_registry.each([&](auto entityID) {
			Entity entity(entityID, scene.get());
			if (!entity) return;
			entities.push_back(entity);
			});
		for (auto it = entities.rbegin(); it != entities.rend(); ++it)
			serializeEntity(out, (*it));
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath, std::ios::binary);
		fout << out.c_str();
	}
	enum class ComponentID : std::uint16_t {
		TAG_COMPONENT = 0,
		TRANSFORM_COMPONENT = 1,
		SPRITE_RENDERER_COMPONENT = 2,
		CAMERA_COMPONENT = 3
	};
	std::ofstream& operator<<(std::ofstream& out, ComponentID value) {
		out.write(Converter::toBytes((std::uint16_t)value).chars, 2);
		return out;
	}
	static void serializeEntityRuntime(std::ofstream& out, Entity entity) {
		std::uint32_t length = 4 + 8 + 2; // 4: length, 8: ruid, 2 component count
		struct SerializedData {
			ComponentID id;
			std::string data;
		};
		std::vector<SerializedData> components;
		if (entity.hasComponent<TagComponent>()) {
			auto& comp = entity.getComponent<TagComponent>();
			std::string data = "";
			length += (std::uint32_t)comp.tag.length() + 1; // tag length + 1 byte for tag length
			data += (char)glm::min((int)comp.tag.length(), 255);
			data += comp.tag.substr(0, 255);
			components.push_back({ ComponentID::TAG_COMPONENT, data });
		}
		if (entity.hasComponent<TransformComponent>()) {
			auto& comp = entity.getComponent<TransformComponent>();
			std::string data = "";
			length += 3 * sizeof(glm::vec3); // Translation, Rotation, Scale
			data += comp.translation;// Translation
			data += comp.rotation;// Rotation
			data += comp.scale;// Scale
			components.push_back({ ComponentID::TRANSFORM_COMPONENT, data });
		}
		if (entity.hasComponent<SpriteRendererComponent>()) {
			auto& comp = entity.getComponent<SpriteRendererComponent>();
			std::string data = "";
			length += sizeof(glm::vec4); // Clear Color
			length += sizeof(RUID); // ID
			length += sizeof(float); // Tiling Factor
			data += comp.color;// Clear Color
			data.append(Converter::toBytes<std::uint64_t>(comp.id).chars, 8); // ID
			data.append(Converter::toBytes<float>(comp.tilingFactor).chars, 4); // Tiling Factor
			components.push_back({ ComponentID::SPRITE_RENDERER_COMPONENT, data });
		}
		if (entity.hasComponent<CameraComponent>()) {
			auto& comp = entity.getComponent<CameraComponent>();
			auto& camera = comp.camera;
			std::string data = "";
			length += 1 /*flags*/ + sizeof(float) * 6 /*Perspective and orthographic values*/;
			std::uint8_t flags = 0;
			if (comp.primary) flags |= CAMERA_COMPONENT_FLAG_IS_PRIMARY;
			if (comp.fixedAspectRatio) flags |= CAMERA_COMPONENT_FLAG_FIXED_ASPECT_RATIO;
			if (comp.camera.getProjectionType() == SceneCamera::ProjectionType::Perspective) flags |= CAMERA_COMPONENT_FLAG_PROJECTION_TYPE;
			data += (char)flags;
			data.append(Converter::toBytes(camera.getPerspectiveFOV()).chars, 4);
			data.append(Converter::toBytes(camera.getPerspectiveNearClip()).chars, 4);
			data.append(Converter::toBytes(camera.getPerspectiveFarClip()).chars, 4);
			data.append(Converter::toBytes(camera.getOrthographicSize()).chars, 4);
			data.append(Converter::toBytes(camera.getOrthographicNearClip()).chars, 4);
			data.append(Converter::toBytes(camera.getOrthographicFarClip()).chars, 4);
			components.push_back({ ComponentID::CAMERA_COMPONENT, data });
		}
		length += std::uint32_t(components.size() * 2); // Each conventionally serialized entity has an overhead of 2 bytes

		out.write(Converter::toBytes(length).chars, 4); // length of entity data
		ASSERT(entity.hasComponent<RUIDComponent>(), "Entity needs to have RUIDComponent to be serialized");
		out.write(Converter::toBytes((std::uint64_t)entity.getComponent<RUIDComponent>().ruid).chars, sizeof(RUID::ruid)); // Unique ID
		out.write(Converter::toBytes((std::uint16_t)components.size()).chars, 2); // component count

		for (auto component : components) {
			out << component.id << component.data;
		}
	}
	enum class SceneAttributeID : std::uint8_t {
		CLEAR_COLOR = 0
	};
	std::ofstream& operator<<(std::ofstream& out, SceneAttributeID value) {
		out << (std::uint8_t)value;
		return out;
	}
	void SceneSerializer::serializeRuntime(const ref<Scene>& scene, const std::string& filepath)
	{
		std::ofstream out(filepath, std::ios::binary);
		if (!out) {
			LOG_ENGINE_ERROR("Couldn't open file {0}", filepath);
			return;
		}
		// Magic Number
		out << RUNTIME_SCENEFILE_MAGIC_NUMBER;
		// Scene Name
		out << (std::uint8_t)glm::min((int)scene->getName().length(), 255);
		out << scene->getName().substr(0, 255); 
		// Scene Attributes
		out << (std::uint8_t)1; // Attribute count, to be changed
		out << SceneAttributeID::CLEAR_COLOR << scene->getClearColor();
		// Entities
		std::uint64_t entityCount = (std::uint64_t)scene->_registry.alive();
		out.write(Converter::toBytes(entityCount).chars, sizeof(std::uint64_t));
		std::vector<Entity> entities;
		entities.reserve(scene->_registry.alive());
		scene->_registry.each([&](auto entityID) {
			Entity entity(entityID, scene.get());
			if (!entity) return;
			entities.push_back(entity);
			});
		for (auto it = entities.rbegin(); it != entities.rend(); ++it)
			serializeEntityRuntime(out, (*it));
	}
	bool SceneSerializer::deserialize(const ref<Scene>& scene, const std::string& filepath)
	{
		if (!std::filesystem::exists(filepath)) {
			LOG_ENGINE_WARN("Scene '{0}' doesn't exist", filepath);
			return false;
		}
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
					src.id = spriteRendererComponent["TextureID"].as<std::uint64_t>();
					src.tilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
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
	void SceneSerializer::deserializeRuntime(const ref<Scene>& scene, const std::string& filepath)
	{
		std::ifstream in(filepath, std::ios::binary);
		if (!in) {
			LOG_ENGINE_ERROR("Couldn't open file '{0}'", filepath);
			return;
		}
		std::string buf = std::string(RUNTIME_SCENEFILE_MAGIC_NUMBER.length(), 0);
		char cbuf;
		in.read(&buf[0], buf.length());
		if(buf != RUNTIME_SCENEFILE_MAGIC_NUMBER) {
			LOG_ENGINE_ERROR("File '{0}' is invalid runtime scene", filepath);
			return;
		}
		in.get(cbuf); // Scene name length
		buf.resize((std::uint8_t)cbuf, 0);
		// Read name
		in.read(&buf[0], buf.length());
		scene->_name = buf;
		LOG_ENGINE_TRACE("Deserializing runtime scene '{0}'", scene->_name);
		// Attribute count
		in.get(cbuf);
		std::uint8_t attributeCount = (std::uint8_t)cbuf;
		// Read attributes
		for (int i = 0; i < attributeCount; i++) {
			in.get(cbuf); // Attribute type
			switch ((SceneAttributeID)cbuf) {
			case SceneAttributeID::CLEAR_COLOR: {
				buf.resize(sizeof(float), 0);
				for (int i = 0; i < 4; i++) {
					in.read(&buf[0], buf.length());
					scene->_clearColor[i] = Converter::toNum<float>(buf);
				}
				break;
			}
			}
		}
		// Entity count
		buf.resize(8, 0);
		in.read(&buf[0], buf.length());
		std::uint64_t entityCount = Converter::toNum<std::uint64_t>(buf);
		std::uint64_t start = in.tellg();
		// Read entities
		for (std::uint64_t i = 0; i < entityCount; i++) {
			in.seekg(start);
			// Entity length (maybe obsolete)
			std::uint32_t length = 0;
			buf.resize(4, 0);
			in.read(&buf[0], buf.length());
			length = Converter::toNum<std::uint32_t>(buf);

			Entity entity = scene->createEntity("");
			ASSERT(entity.hasComponent<RUIDComponent>(), "Entity needs to have RUIDComponent");

			// Read RUID
			buf.resize(8, 0);
			in.read(&buf[0], buf.length());
			entity.getComponent<RUIDComponent>().ruid = Converter::toNum<std::uint64_t>(buf);

			// Components
			std::uint16_t componentCount = 0;
			buf.resize(2, 0);
			in.read(&buf[0], buf.length());
			componentCount = Converter::toNum<std::uint16_t>(buf);

			// Read components
			for (std::uint16_t coi = 0; coi < componentCount; coi++) {
				// ComponentID
				buf.resize(2, 0);
				in.read(&buf[0], buf.length());
				switch ((ComponentID)Converter::toNum<std::uint16_t>(buf)) {
				case ComponentID::TAG_COMPONENT: {
					// Tag length
					in.get(cbuf);
					// Tag
					buf.resize((std::uint8_t)cbuf, 0);
					in.read(&buf[0], buf.length());
					if (!entity.hasComponent<TagComponent>()) {
						entity.addComponent<TagComponent>().tag = buf;
					}
					else entity.getComponent<TagComponent>().tag = buf;
					break;
				}
				case ComponentID::TRANSFORM_COMPONENT: {
					auto& trans = entity.getComponent<TransformComponent>();
					buf.resize(sizeof(float), 0);
					// Translation
					for (int ti = 0; ti < 3; ti++) {
						in.read(&buf[0], buf.length());
						trans.translation[ti] = Converter::toNum<float>(buf);
					}
					// Rotation
					for (int ri = 0; ri < 3; ri++) {
						in.read(&buf[0], buf.length());
						trans.rotation[ri] = Converter::toNum<float>(buf);
					}
					// Scale
					for (int si = 0; si < 3; si++) {
						in.read(&buf[0], buf.length());
						trans.scale[si] = Converter::toNum<float>(buf);
					}
					break;
				}
				case ComponentID::SPRITE_RENDERER_COMPONENT: {
					buf.resize(sizeof(float), 0);
					glm::vec4 color = glm::vec4(1.0f);
					// Color
					for (int ci = 0; ci < 4; ci++) {
						in.read(&buf[0], buf.length());
						color[ci] = Converter::toNum<float>(buf);
					}
					// Texture ID
					buf.resize(sizeof(RUID), 0);
					in.read(&buf[0], buf.length());
					std::uint64_t id = Converter::toNum<std::uint64_t>(buf);
					// Tiling Factor
					buf.resize(sizeof(float), 0);
					in.read(&buf[0], buf.length());
					float tilingFactor = Converter::toNum<float>(buf);

					if (!entity.hasComponent<SpriteRendererComponent>()) {
						entity.addComponent<SpriteRendererComponent>().color = color;
						entity.getComponent<SpriteRendererComponent>().id = id;
						entity.getComponent<SpriteRendererComponent>().tilingFactor = tilingFactor;
					}
					else {
						entity.getComponent<SpriteRendererComponent>().color = color;
						entity.getComponent<SpriteRendererComponent>().id = id;
						entity.getComponent<SpriteRendererComponent>().tilingFactor = tilingFactor;
					}
					break;
				}
				case ComponentID::CAMERA_COMPONENT: {
					auto& comp = entity.addComponent<CameraComponent>();
					auto& cam = comp.camera;
					// Flags
					in.get(cbuf);
					comp.primary = cbuf & CAMERA_COMPONENT_FLAG_IS_PRIMARY;
					comp.fixedAspectRatio = cbuf & CAMERA_COMPONENT_FLAG_FIXED_ASPECT_RATIO;
					comp.camera.setProjectionType((SceneCamera::ProjectionType)(cbuf & CAMERA_COMPONENT_FLAG_PROJECTION_TYPE));
					// Perspective
					buf.resize(sizeof(float), 0);
					in.read(&buf[0], buf.length());
					comp.camera.setPerspectiveFOV(Converter::toNum<float>(buf));
					in.read(&buf[0], buf.length());
					comp.camera.setPerspectiveNearClip(Converter::toNum<float>(buf));
					in.read(&buf[0], buf.length());
					comp.camera.setPerspectiveFarClip(Converter::toNum<float>(buf));
					// Orthographic
					in.read(&buf[0], buf.length());
					comp.camera.setOrthographicSize(Converter::toNum<float>(buf));
					in.read(&buf[0], buf.length());
					comp.camera.setOrthographicNearClip(Converter::toNum<float>(buf));
					in.read(&buf[0], buf.length());
					comp.camera.setOrthographicFarClip(Converter::toNum<float>(buf));
					break;
				}
				}
			}
			start += length;
		}
	}
}