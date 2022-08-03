#include <pch.h>
#include "Renderer2D.h"
#include <Raven/rendering/Renderer.h>
#include <gtc/matrix_transform.hpp>

namespace rvn {

	struct QuadVertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;
		std::uint32_t entityID;
	};

	struct Renderer2DData {
		ref<VertexArray> vertexArray;
		ref<VertexBuffer> vertexBuffer;
		ref<IndexBuffer> indexBuffer;
		ref<Shader> shader;
		ref<Texture2D> whiteTexture;
		
		std::uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		glm::vec4 quadPositions[4];
		glm::vec2 quadTexCoords[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		std::uint32_t textureIndex = 1; // 0 = White Texture
		
		static const std::uint32_t maxQuads = 20000;
		static const std::uint32_t maxVertices = maxQuads * 4;
		static const std::uint32_t maxIndices = maxQuads * 6;
		static const std::uint32_t maxTexSlots = 16; // TODO: Find real max texture slots

		std::array<ref<Texture2D>, maxTexSlots> textures; // 0 = White Texture
	};


	static Renderer2DData s_data;

	void Renderer2D::init()
	{
		s_data.vertexArray = VertexArray::create();

		s_data.vertexBuffer = VertexBuffer::create(sizeof(QuadVertex) * s_data.maxVertices);
		s_data.vertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_TintColor" },
			{ ShaderDataType::Float2, "a_TexCoords" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Uint, "a_EntityID" }
			});
		s_data.vertexArray->addVertexBuffer(s_data.vertexBuffer);

		s_data.quadVertexBufferBase = new QuadVertex[s_data.maxVertices];

		std::uint32_t* quadIndices = new std::uint32_t[s_data.maxIndices];

		std::uint32_t offset = 0;
		for (std::uint32_t i = 0; i < s_data.maxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		s_data.indexBuffer = IndexBuffer::create(quadIndices, s_data.maxIndices);
		s_data.vertexArray->setIndexBuffer(s_data.indexBuffer);
		delete[] quadIndices;

		s_data.quadPositions[0] = { -0.5f, -0.5f,  0.0f,  1.0f };
		s_data.quadPositions[1] = {  0.5f, -0.5f,  0.0f,  1.0f };
		s_data.quadPositions[2] = {  0.5f,  0.5f,  0.0f,  1.0f };
		s_data.quadPositions[3] = { -0.5f,  0.5f,  0.0f,  1.0f };

		s_data.shader = Shader::create("assets/shaders/Renderer2D_Quad.glsl");
		s_data.shader->bind();

		s_data.whiteTexture = Texture2D::create(1, 1);
		int whiteTextureData = 0xFFFFFFFF;
		s_data.whiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));
		s_data.textures[0] = s_data.whiteTexture;

		int* samplers = new int[s_data.maxTexSlots];
		for (std::uint32_t i = 0; i < s_data.maxTexSlots; i++)
			samplers[i] = i;
		s_data.shader->setIntArray("u_Textures", samplers, s_data.maxTexSlots);
		delete[] samplers;
	}

	void Renderer2D::shutdown()
	{
		delete[] s_data.quadVertexBufferBase;
	}

	void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.getProjection() * glm::inverse(transform);

		s_data.shader->bind();
		s_data.shader->setMat4("u_ViewProjectionMatrix", viewProj);

		startBatch();
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		s_data.shader->bind();
		s_data.shader->setMat4("u_ViewProjectionMatrix", camera.getViewProjectionMatrix());

		startBatch();
	}
	void Renderer2D::startBatch()
	{
		s_data.quadIndexCount = 0;
		s_data.textureIndex = 1;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
	}

	void Renderer2D::nextBatch()
	{
		flush();
		startBatch();
	}

	void Renderer2D::flush()
	{
		if (s_data.quadIndexCount == 0)
			return; // Nothing to be rendered

		std::uint32_t dataSize = (std::uint32_t)((std::uint8_t*)s_data.quadVertexBufferPtr - (std::uint8_t*)s_data.quadVertexBufferBase);
		s_data.vertexBuffer->setData(s_data.quadVertexBufferBase, dataSize);

		// Bind textures
		s_data.shader->bind();
		for (std::uint32_t i = 0; i < s_data.textureIndex; i++)
			s_data.textures[i]->bind(i);

		RenderCommand::draw(s_data.vertexArray, s_data.quadIndexCount);
	}

	void Renderer2D::endScene()
	{
		flush();
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, color);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, color, tilingFactor);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, texture, color, tilingFactor);
	}
	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, subtexture, color, tilingFactor);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, subtexture, color, tilingFactor);
	}
	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, color, tilingFactor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, texture, color, tilingFactor);
	}
	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, color, tilingFactor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, subtexture, color, tilingFactor);
	}
	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color, std::uint32_t entityID)
	{
		if (s_data.quadIndexCount >= s_data.maxIndices)
			nextBatch();

		constexpr int vertexCount = 4;
		for (int i = 0; i < vertexCount; i++) {
			s_data.quadVertexBufferPtr->position = transform * s_data.quadPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->texCoord = s_data.quadTexCoords[i];
			s_data.quadVertexBufferPtr->texIndex = 0.0f;
			s_data.quadVertexBufferPtr->tilingFactor = 1.0f;
			s_data.quadVertexBufferPtr->entityID = entityID;
			s_data.quadVertexBufferPtr++;
		}
		s_data.quadIndexCount += 6;
	}
	void Renderer2D::drawQuad(const glm::mat4& transform, const ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor, std::uint32_t entityID)
	{
		if (s_data.quadIndexCount >= s_data.maxIndices)
			nextBatch();

		if (s_data.textureIndex >= s_data.maxTexSlots)
			nextBatch();
		
		int texIndex = 0;
		for (std::uint32_t i = 1; i < s_data.textureIndex; i++) {
			if (texture == s_data.textures[i]) texIndex = i;
		}
		if (texIndex == 0) {
			s_data.textures[s_data.textureIndex] = texture;
			texIndex = s_data.textureIndex;
			s_data.textureIndex++;
		}

		constexpr int vertexCount = 4;
		for (int i = 0; i < vertexCount; i++) {
			s_data.quadVertexBufferPtr->position = transform * s_data.quadPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->texCoord = s_data.quadTexCoords[i];
			s_data.quadVertexBufferPtr->texIndex = (float)texIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_data.quadVertexBufferPtr->entityID = entityID;
			s_data.quadVertexBufferPtr++;
		}
		s_data.quadIndexCount += 6;
	}
	void Renderer2D::drawQuad(const glm::mat4& transform, const ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor)
	{
		if (s_data.quadIndexCount >= s_data.maxIndices)
			nextBatch();

		if (s_data.textureIndex >= s_data.maxTexSlots)
			nextBatch();

		int texIndex = 0;
		for (std::uint32_t i = 1; i < s_data.textureIndex; i++) {
			if (subtexture->getTexture() == s_data.textures[i]) texIndex = i;
		}
		if (texIndex == 0) {
			s_data.textures[s_data.textureIndex] = subtexture->getTexture();
			texIndex = s_data.textureIndex;
			s_data.textureIndex++;
		}

		constexpr int vertexCount = 4;
		for (int i = 0; i < vertexCount; i++) {
			s_data.quadVertexBufferPtr->position = transform * s_data.quadPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->texCoord = subtexture->getTexCoords()[i];
			s_data.quadVertexBufferPtr->texIndex = (float)texIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_data.quadVertexBufferPtr->entityID = -1;
			s_data.quadVertexBufferPtr++;
		}
		s_data.quadIndexCount += 6;
	}



}