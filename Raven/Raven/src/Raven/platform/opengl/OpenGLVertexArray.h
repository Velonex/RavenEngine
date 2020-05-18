#pragma once
#include <Raven/rendering/VertexArray.h>
#include <cstdint>

namespace rvn {
	
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void addVertexBuffer(const ref<VertexBuffer>& vertexBuffer) override;
		virtual void setIndexBuffer(const ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<ref<VertexBuffer>>& getVertexBuffers() const override;
		virtual const ref<IndexBuffer>& getIndexBuffer() const override;
	private:
		std::uint32_t _id = 0;
		std::vector<ref<VertexBuffer>> _vertexBuffers;
		ref<IndexBuffer> _indexBuffer;
	};

}