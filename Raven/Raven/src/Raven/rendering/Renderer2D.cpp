#include <pch.h>
#include "Renderer2D.h"
#include <Raven/rendering/Renderer.h>
#include <gtc/matrix_transform.hpp>

namespace rvn {

	struct QuadVertex {
		glm::vec3 position;
		glm::vec4 color;
	};

	struct Renderer2DData {
		ref<VertexArray> vertexArray;
		ref<VertexBuffer> vertexBuffer;
		ref<IndexBuffer> indexBuffer;
		ref<Shader> shader;
		
		std::uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		glm::vec4 quadPositions[4];

		static const std::uint32_t maxQuads = 20000;
		static const std::uint32_t maxVertices = maxQuads * 4;
		static const std::uint32_t maxIndices = maxQuads * 6;
		static const std::uint32_t maxTexSlots = 8; // TODO: Find real max texture slots
	};


	static Renderer2DData s_data;

	void Renderer2D::init()
	{
		s_data.vertexArray = VertexArray::create();
		s_data.vertexBuffer = VertexBuffer::create(sizeof(QuadVertex) * s_data.maxVertices);
		s_data.vertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_TintColor" }
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

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_TintColor;

out vec4 v_Color;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
	v_Color = a_TintColor;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

)";

		const char* fragmentShaderSource =
			R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
	color = v_Color;
}
)";

		s_data.shader = rvn::Shader::create(vertexShaderSource, fragmentShaderSource, "texture");
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

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		if (s_data.quadIndexCount >= s_data.maxIndices)
			flushAndReset();

		constexpr int vertexCount = 4;
		for (int i = 0; i < vertexCount; i++) {
			s_data.quadVertexBufferPtr->position = transform * s_data.quadPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr++;
		}
		s_data.quadIndexCount += 6;
	}

	void Renderer2D::flush()
	{
		if (s_data.quadIndexCount == 0)
			return; // Nothing to be rendered
		s_data.shader->bind();
		RenderCommand::draw(s_data.vertexArray, s_data.quadIndexCount);
	}

	void Renderer2D::flushAndReset()
	{
		endScene();

		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
	}

}