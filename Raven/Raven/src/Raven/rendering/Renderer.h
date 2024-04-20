#pragma once
#include <Raven/rendering/OrthographicCamera.h>
#include <Raven/rendering/Mesh.h>
#include <Raven/rendering/VertexArray.h>
#include <Raven/rendering/Shader.h>
#include <Raven/rendering/RenderCommand.h>

namespace rvn {

	class Renderer {
	public:
		static void init();
		static void shutdown();

		static void beginScene(const OrthographicCamera& camera);
		static void endScene();

		static void draw(const ref<VertexArray>& vertexArray, const ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
		static void draw(const ref<Mesh>& mesh, const ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		static void onWindowResize(std::uint32_t width, std::uint32_t height);
	private:
		struct SceneData {
			glm::mat4 _viewProjectionMatrix;
		};
		static scope<SceneData> s_sceneData;
	};

}