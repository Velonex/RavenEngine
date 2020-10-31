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

		// TODO: Move to file
		const char* vertexShaderSource =
			R"(
#version 330 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_TintColor;
layout(location = 2) in vec2  a_TexCoords;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

out vec4  v_Color;
out vec2  v_TexCoord;
out float v_TilingFactor;
out float v_TexIndex;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
	v_Color = a_TintColor;
	v_TexCoord = a_TexCoords;
	v_TilingFactor = a_TilingFactor;
	v_TexIndex = a_TexIndex;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

)";

		const char* fragmentShaderSource =
			R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TilingFactor;
in float v_TexIndex;

uniform sampler2D u_Textures[16];

void main()
{
	vec4 col = v_Color;
	switch(int(v_TexIndex)) {
		case 0: color = col *= texture(u_Textures[0], v_TexCoord * v_TilingFactor); break;
		case 1: color = col *= texture(u_Textures[1], v_TexCoord * v_TilingFactor); break;
		case 2: color = col *= texture(u_Textures[2], v_TexCoord * v_TilingFactor); break;
		case 3: color = col *= texture(u_Textures[3], v_TexCoord * v_TilingFactor); break;
		case 4: color = col *= texture(u_Textures[4], v_TexCoord * v_TilingFactor); break;
		case 5: color = col *= texture(u_Textures[5], v_TexCoord * v_TilingFactor); break;
		case 6: color = col *= texture(u_Textures[6], v_TexCoord * v_TilingFactor); break;
		case 7: color = col *= texture(u_Textures[7], v_TexCoord * v_TilingFactor); break;
		case 8: color = col *= texture(u_Textures[8], v_TexCoord * v_TilingFactor); break;
		case 9: color = col *= texture(u_Textures[9], v_TexCoord * v_TilingFactor); break;
		case 10: color = col *= texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;
		case 11: color = col *= texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;
		case 12: color = col *= texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;
		case 13: color = col *= texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;
		case 14: color = col *= texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;
		case 15: color = col *= texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;
		//case 16: color = col *= texture(u_Textures[16], v_TexCoord * v_TilingFactor); break;
		//case 17: color = col *= texture(u_Textures[17], v_TexCoord * v_TilingFactor); break;
		//case 18: color = col *= texture(u_Textures[18], v_TexCoord * v_TilingFactor); break;
		//case 19: color = col *= texture(u_Textures[19], v_TexCoord * v_TilingFactor); break;
		//case 20: color = col *= texture(u_Textures[20], v_TexCoord * v_TilingFactor); break;
		//case 21: color = col *= texture(u_Textures[21], v_TexCoord * v_TilingFactor); break;
		//case 22: color = col *= texture(u_Textures[22], v_TexCoord * v_TilingFactor); break;
		//case 23: color = col *= texture(u_Textures[23], v_TexCoord * v_TilingFactor); break;
		//case 24: color = col *= texture(u_Textures[24], v_TexCoord * v_TilingFactor); break;
		//case 25: color = col *= texture(u_Textures[25], v_TexCoord * v_TilingFactor); break;
		//case 26: color = col *= texture(u_Textures[26], v_TexCoord * v_TilingFactor); break;
		//case 27: color = col *= texture(u_Textures[27], v_TexCoord * v_TilingFactor); break;
		//case 28: color = col *= texture(u_Textures[28], v_TexCoord * v_TilingFactor); break;
		//case 29: color = col *= texture(u_Textures[29], v_TexCoord * v_TilingFactor); break;
		//case 30: color = col *= texture(u_Textures[30], v_TexCoord * v_TilingFactor); break;
		//case 31: color = col *= texture(u_Textures[31], v_TexCoord * v_TilingFactor); break;
	}
}
)";

		s_data.shader = Shader::create(vertexShaderSource, fragmentShaderSource, "texture");
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

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		s_data.shader->bind();
		s_data.shader->setMat4("u_ViewProjectionMatrix", camera.getViewProjectionMatrix());
		s_data.quadIndexCount = 0;
		s_data.textureIndex = 1;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
	}

	void Renderer2D::endScene()
	{
		std::uint32_t dataSize = (std::uint32_t)((std::uint8_t*)s_data.quadVertexBufferPtr - (std::uint8_t*)s_data.quadVertexBufferBase);
		s_data.vertexBuffer->setData(s_data.quadVertexBufferBase, dataSize);

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
	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		if (s_data.quadIndexCount >= s_data.maxIndices)
			flushAndReset();

		constexpr int vertexCount = 4;
		for (int i = 0; i < vertexCount; i++) {
			s_data.quadVertexBufferPtr->position = transform * s_data.quadPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->texCoord = s_data.quadTexCoords[i];
			s_data.quadVertexBufferPtr->texIndex = 0.0f;
			s_data.quadVertexBufferPtr->tilingFactor = 1.0f;
			s_data.quadVertexBufferPtr++;
		}
		s_data.quadIndexCount += 6;
	}
	void Renderer2D::drawQuad(const glm::mat4& transform, const ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		if (s_data.quadIndexCount >= s_data.maxIndices)
			flushAndReset();

		if (s_data.textureIndex >= s_data.maxTexSlots)
			flushAndReset();
		
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
			s_data.quadVertexBufferPtr++;
		}
		s_data.quadIndexCount += 6;
	}
	void Renderer2D::drawQuad(const glm::mat4& transform, const ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor)
	{
		if (s_data.quadIndexCount >= s_data.maxIndices)
			flushAndReset();

		if (s_data.textureIndex >= s_data.maxTexSlots)
			flushAndReset();

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
			s_data.quadVertexBufferPtr++;
		}
		s_data.quadIndexCount += 6;
	}
	void Renderer2D::flush()
	{
		if (s_data.quadIndexCount == 0)
			return; // Nothing to be rendered
		s_data.shader->bind();
		for (std::uint32_t i = 0; i < s_data.textureIndex; i++)
			s_data.textures[i]->bind(i);
		RenderCommand::draw(s_data.vertexArray, s_data.quadIndexCount);
	}

	void Renderer2D::flushAndReset()
	{
		endScene();

		s_data.quadIndexCount = 0;
		s_data.textureIndex = 1;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
	}

}