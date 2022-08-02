#pragma once
#include <Raven_Core/core/Assert.h>
#include <Raven_Core/datatypes/Memory.h>
#include <initializer_list>

namespace rvn {

	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Uint, Bool
	};

	static std::uint32_t shaderDataTypeSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::Int2: return 4 * 2;
		case ShaderDataType::Int3: return 4 * 3;
		case ShaderDataType::Int4: return 4 * 4;
		case ShaderDataType::Uint: return 4;
		case ShaderDataType::Bool: return 1;
		}
		ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct BufferElement {
		std::string name;
		ShaderDataType type = ShaderDataType::None;
		std::uint32_t size = 0;
		std::size_t offset = 0;
		bool normalized = false;

		BufferElement(ShaderDataType dataType, std::string name, bool normalized = false) : name(name), type(dataType),
			size(shaderDataTypeSize(type)), normalized(normalized), offset(0)
		{}
		BufferElement(ShaderDataType dataType, const char* name, bool normalized = false) : name(name), type(dataType),
			size(shaderDataTypeSize(type)), normalized(normalized), offset(0)
		{}

		std::uint32_t getComponentCount() const {
			switch (type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3;
			case ShaderDataType::Mat4:    return 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Uint:     return 1;
			case ShaderDataType::Bool:    return 1;
			}
			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() : _stride(0) {}

		BufferLayout(const std::initializer_list<BufferElement>& elements) : _elements(elements)
		{
			calculateOffsetsAndStride();
		}

		const std::vector<BufferElement>& getElements() const {
			return _elements;
		}

		std::uint32_t getStride() const {
			return _stride;
		}

		std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
		std::vector<BufferElement>::iterator end() { return _elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }
	private:
		void calculateOffsetsAndStride() {
			size_t offset = 0;
			_stride = 0;
			for (auto& element : _elements) {
				_stride += element.size;
				element.offset = offset;
				offset += element.size;
			}
		}
		std::vector<BufferElement> _elements;
		std::uint32_t _stride;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setData(void* data, std::uint32_t size) = 0;

		virtual void setLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& getLayout() const = 0;

		static ref<VertexBuffer> create(std::uint32_t size);
		static ref<VertexBuffer> create(void* vertices, std::uint32_t size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual std::uint32_t getCount() const = 0;

		static ref<IndexBuffer> create(std::uint32_t* indices, std::uint32_t count);
	};

}