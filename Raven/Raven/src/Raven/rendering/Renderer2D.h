#pragma once
#include <Raven/rendering/Camera.h>
#include <Raven/rendering/OrthographicCamera.h>
#include <glm.hpp>
#include <Raven_Core.h>
#include <Raven/rendering/Texture.h>
#include <Raven/rendering/SubTexture2D.h>

namespace rvn {

	class Renderer2D {
	public:
		static void init();
		static void shutdown();

		static void beginScene(const OrthographicCamera& camera);
		static void beginScene(const Camera& camera, const glm::mat4& transform);
		static void endScene();

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture,
			const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<Texture2D>& texture,
			const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<SubTexture2D>& subtexture,
			const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<SubTexture2D>& subtexture,
			const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		
		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture,
			const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture,
			const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ref<SubTexture2D>& subtexture,
			const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ref<SubTexture2D>& subtexture,
			const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void drawQuad(const glm::mat4& transform, const glm::vec4& color, std::uint32_t entityID = -1);
		static void drawQuad(const glm::mat4& transform, const ref<Texture2D>& texture,
			const glm::vec4& color, float tilingFactor, std::uint32_t entityID = -1);
		static void drawQuad(const glm::mat4& transform, const ref<SubTexture2D>& subtexture,
			const glm::vec4& color, float tilingFactor);


		static void flush();
	private:
		static void startBatch();
		static void nextBatch();
	};

}