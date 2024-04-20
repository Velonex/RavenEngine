#pragma once

namespace rvn {

	enum class MeshFormat {
		OBJ
	};

	class Mesh {
	public:
		Mesh(const filepath_t& path, MeshFormat format);

		bool parseObj(const filepath_t& path);
	public:
		virtual ~Mesh() {}

		static ref<Mesh> createFromFile(const filepath_t& path, MeshFormat format);

		ref<VertexArray> getVertexArray() const;
	private:
		ref<VertexArray> _va;
	};

}