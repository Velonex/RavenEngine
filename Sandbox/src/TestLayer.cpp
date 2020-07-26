#include "TestLayer.h"
#include <string>
#include <Raven/rendering/Renderer.h>

struct Vertex {
	float position[3];
	float tex_coords[2];
};

void TestLayer::onAttach()
{
	// Create and bind vertex array
	vao = rvn::VertexArray::create();
	vao->bind();

	// Vertex positions
	Vertex vertices[4] = {
		{-0.5f, -0.5f,  0.0f,  0.0f,  0.0f, },
		{ 0.5f, -0.5f,  0.0f,  1.0f,  0.0f, },
		{ 0.5f,  0.5f,  0.0f,  1.0f,  1.0f, },
		{-0.5f,  0.5f,  0.0f,  0.0f,  1.0f  }
	};

	// Create and bind vertex buffer
	vbo = rvn::VertexBuffer::create(vertices, sizeof(vertices));

	// Set Layout
	vbo->setLayout({
		{ rvn::ShaderDataType::Float3, "a_Position" },
		{ rvn::ShaderDataType::Float2, "a_TexCoords" }
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

	// Create shaders
	const char* vertexShaderSource =
R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}

)";

	const char* fragmentShaderSource =
R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord);
}
)";

	shader = rvn::Shader::create(vertexShaderSource, fragmentShaderSource, "test");

	texture = rvn::Texture2D::create("assets/textures/chess.png");

	camera = rvn::OrthographicCameraController(16.f / 9.f, true);
}

void TestLayer::onUpdate(rvn::Timestep timestep)
{
	camera.onUpdate(timestep);
	rvn::Renderer::beginScene(camera.getCamera());
	shader->bind();
	texture->bind();
	rvn::Renderer::draw(vao, shader);
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
