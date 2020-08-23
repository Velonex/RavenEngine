#pragma once
#include <Raven/rendering/OrthographicCamera.h>
#include <glm.hpp>
#include <Raven_Core.h>
#include <Raven/rendering/Texture.h>

namespace rvn {

	class Renderer2D {
	public:
		static void init();
		static void shutdown();

		static void beginScene(const OrthographicCamera& camera);
		static void endScene();

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture,
			const glm::vec4& color, float tilingFactor);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<Texture2D>& texture,
			const glm::vec4& color, float tilingFactor);
		
		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture,
			const glm::vec4& color, float tilingFactor);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture,
			const glm::vec4& color, float tilingFactor);

		static void drawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void drawQuad(const glm::mat4& transform, const ref<Texture2D>& texture,
			const glm::vec4& color, float tilingFactor);


		static void flush();
		static void flushAndReset();
	};

}