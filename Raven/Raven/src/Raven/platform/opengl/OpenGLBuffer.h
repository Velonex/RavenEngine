#pragma once
#include <Raven/rendering/Buffer.h>

namespace rvn {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(void* vertices, std::uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setLayout(const BufferLayout& layout) override;
		virtual const BufferLayout& getLayout() const override;
	private:
		std::uint32_t _id = 0;
		BufferLayout _layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(std::uint32_t* indices, std::uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual std::uint32_t getCount() const override;
	private:
		std::uint32_t _id, _count;
	};

}