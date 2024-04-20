#include "TestLayer.h"
#include <string>
#include <Raven/rendering/Renderer.h>
#include <Raven/rendering/RenderCommand.h>

struct Vertex {
	float position[3];
	float tex_coords[2];
	float color[4];
};

void TestLayer::onAttach()
{
	mesh = rvn::Mesh::createFromFile("assets/models/arrow.obj", rvn::MeshFormat::OBJ);

	// Create and bind vertex array
	vao = rvn::VertexArray::create();
	vao->bind();

	// Vertex positions
	Vertex vertices[4] = {
		{-0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f },
		{ 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f },
		{ 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f },
		{-0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f }
	};

	// Create and bind vertex buffer
	vbo = rvn::VertexBuffer::create(vertices, sizeof(vertices));

	// Set Layout
	vbo->setLayout({
		{ rvn::ShaderDataType::Float3, "a_Position" },
		{ rvn::ShaderDataType::Float2, "a_TexCoords" },
		{ rvn::ShaderDataType::Float4, "a_Color" }
		});

	vao->addVertexBuffer(vbo);

	// Indices
	unsigned int indices[2 * 3] = {
		0, 1, 2,
		2, 3, 0
	};

	// Create and bind index buffer
	ibo = rvn::IndexBuffer::create(indices, sizeof(indices) / sizeof(unsigned int));
	vao->setIndexBuffer(ibo);
	/*
	// Create shaders
	const char* vertexShaderSource =
		R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;

out vec2 v_TexCoord;
out vec4 v_Color;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}
)";

	const char* fragmentShaderSource =
		R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord) * v_Color;
}
)";
*/
// Create shaders

	const char* vertexShaderSource =
		R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Normal;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}
)";

	const char* fragmentShaderSource =
		R"(
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;

void main()
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
}
)";

	shader = rvn::Shader::create(vertexShaderSource, fragmentShaderSource, "test");

	texture = rvn::Texture2D::create("assets/textures/chess.png");

	camera = rvn::OrthographicCameraController(16.f / 9.f, true);
}

void TestLayer::onUpdate(rvn::Timestep timestep)
{
	rvn::RenderCommand::setClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	rvn::RenderCommand::clear();

	camera.onUpdate(timestep);
	rvn::Renderer::beginScene(camera.getCamera());
	shader->bind();
	texture->bind();
	rvn::Renderer::draw(mesh, shader, glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {0.0f, 1.0f, 0.0f}));
	rvn::Renderer::endScene();
}

void TestLayer::onDetach()
{
}

void TestLayer::onImGuiRender()
{
}

void TestLayer::onEvent(rvn::Event* e)
{
	camera.onEvent(e);
}