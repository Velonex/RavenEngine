#include <pch.h>
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace rvn {

	static GLenum shaderDataTypeToGLBaseType(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Bool:		return GL_BOOL;
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		}
		ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &_id);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &_id);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(_id);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const ref<VertexBuffer>& vertexBuffer)
	{
		ASSERT(vertexBuffer->getLayout().getElements().size(), "VertexBuffer has no layout");
		
		glBindVertexArray(_id);
		vertexBuffer->bind();

		std::uint32_t index = 0;
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout) {
			switch (element.type) {
			case ShaderDataType::Bool:
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(
					index,
					element.getComponentCount(),
					shaderDataTypeToGLBaseType(element.type),
					element.normalized,
					layout.getStride(),
					(const void*)element.offset);
				index++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				std::uint8_t locationCount = element.getComponentCount();
				for (std::uint8_t i = 0; i < locationCount; i++) {
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(
						index,
						locationCount,
						shaderDataTypeToGLBaseType(element.type),
						element.normalized,
						layout.getStride(),
						(const void*)(sizeof(float) * i * locationCount));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
			default:
				ASSERT(false, "Unknown ShaderDataType");
			}
		}

		_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(_id);
		_indexBuffer = indexBuffer;
		_indexBuffer->bind();
	}

	const std::vector<ref<VertexBuffer>>& OpenGLVertexArray::getVertexBuffers() const
	{
		return _vertexBuffers;
	}

	const ref<IndexBuffer>& OpenGLVertexArray::getIndexBuffer() const
	{
		return _indexBuffer;
	}
}