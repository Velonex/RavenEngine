#include <pch.h>
#include "Mesh.h"

namespace rvn {

	ref<Mesh> Mesh::createFromFile(const filepath_t& path, MeshFormat format)
	{
		return createRef<Mesh>(path, format);
	}

	ref<VertexArray> Mesh::getVertexArray() const
	{
		return _va;
	}

	Mesh::Mesh(const filepath_t& path, MeshFormat format)
	{
		ASSERT(format == MeshFormat::OBJ, "The only currently supported mesh format is obj");
		_va = VertexArray::create();
		parseObj(path);
	}

	bool Mesh::parseObj(const filepath_t& path)
	{
		std::ifstream in(path); 
		if (!in) {
			LOG_ENGINE_ERROR("Couldn't open mesh at {0}", path.string());
			return false;
		}
		std::string line;
		std::vector<glm::vec3> normals;
		struct Vertex {
			glm::vec3 position = glm::vec3(0.0f);
			glm::vec3 normal = glm::vec3(0.0f);
		};
		std::vector<Vertex> vertices;
		std::vector<u32> indices;

		while (std::getline(in, line)) {
			auto tokens = util::split(line, " ");
			if (tokens.size() < 2) continue;
			if (tokens[0] == "v") {
				// Vertex positions
				if (tokens.size() != 4) {
					LOG_ENGINE_ERROR("Error parsing '{0}': Vertices from .obj files need exactly 3 coordinates", path.string());
					return false;
				}
				try {
					Vertex vertex;
					vertex.position = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };
					vertices.push_back(vertex);
				}
				catch (std::exception& e) {
					LOG_ENGINE_ERROR("Error parsing '{0}': ", e.what());
					return false;
				}
			}
			if (tokens[0] == "vn") {
				// Vertex normals
				if (tokens.size() != 4) {
					LOG_ENGINE_ERROR("Error parsing '{0}': Normals from .obj files need exactly 3 coordinates", path.string());
					return false;
				}
				try {
					glm::vec3 normal = { std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };
					normals.push_back(glm::normalize(normal));
				}
				catch (std::exception& e) {
					LOG_ENGINE_ERROR("Error parsing '{0}': ", e.what());
					return false;
				}
			}
			if (tokens[0] == "vt") {
				LOG_ENGINE_ERROR("Error parsing '{0}': Texture coordinates in .obj files aren't supported yet", path.string());
				return false;
			}
			if (tokens[0] == "f") {
				// Faces
				if (tokens.size() != 4) {
					LOG_ENGINE_ERROR("Error parsing '{0}': Faces from .obj files need to be triangles", path.string());
					return false;
				}
				// Calculate area of each face to proportionally influence the vertex normals
				float faceArea = 0.0f;
				// First index is indexing to the position/vertex, the second into the normals list
				glm::u32vec2 faceIndices[3] = { glm::u32vec2(0) };
				for (int i = 0; i < 3; i++) {
					auto faceIndicesText = util::split(tokens[i + 1], "/");
					if (faceIndicesText.size() != 2) {
						LOG_ENGINE_ERROR("Error parsing '{0}': Meshes from. obj files need to have positions and normals defined", path.string());
						return false;
					}
					faceIndices[i][0] = std::stoi(faceIndicesText[0]) - 1;
					faceIndices[i][1] = std::stoi(faceIndicesText[1]) - 1;
				}
				glm::vec3 vec1 = vertices[faceIndices[0][0]].position - vertices[faceIndices[1][0]].position;
				glm::vec3 vec2 = vertices[faceIndices[0][0]].position - vertices[faceIndices[2][0]].position;
				faceArea = glm::length(glm::cross(vec1, vec2)) * 0.5f;
				
				for (int i = 0; i < 3; i++) {
					u32 vertexIndex = faceIndices[i][0];
					indices.push_back(vertexIndex);
					vertices[vertexIndex].normal += normals[faceIndices[i][1]] * faceArea;
				}
			}
		}
		for (auto& vert : vertices) {
			vert.normal = glm::normalize(vert.normal);
		}
		auto vertexBuffer = VertexBuffer::create((void*)vertices.data(), vertices.size() * sizeof(vertices));
		vertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" }
			});
		_va->addVertexBuffer(vertexBuffer);
		
		auto indexBuffer = IndexBuffer::create(indices.data(), indices.size());
		_va->setIndexBuffer(indexBuffer);

		return true;
	}
}