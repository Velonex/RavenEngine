#include "TestLayer.h"
#include <string>
#include <Raven/rendering/Renderer.h>

struct Vertex {
	float position[3];
	float color[4];
};

void TestLayer::onAttach()
{
	// Create and bind vertex array
	vao = rvn::VertexArray::create();
	vao->bind();

	// Vertex positions
	Vertex vertices[4] = {
		{-0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f},
		{-0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f},
		{ 0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f},
		{ 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f}
	};

	// Create and bind vertex buffer
	vbo = rvn::VertexBuffer::create(vertices, sizeof(vertices));

	// Set Layout
	vbo->setLayout({
		{ rvn::ShaderDataType::Float3, "a_Position" },
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

	// Create shaders
	const char* vertexShaderSource =
R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

void main()
{
	v_Color = a_Color;
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}

)";

	const char* fragmentShaderSource =
R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

uniform vec4 u_TintColor;

void main()
{
	color = v_Color * u_TintColor;
}
)";

	shader = rvn::Shader::create(vertexShaderSource, fragmentShaderSource, "test");

	camera = rvn::OrthographicCameraController(16.f / 9.f, true);
}

void TestLayer::onUpdate(rvn::Timestep timestep)
{
	camera.onUpdate(timestep);
	shader->bind();
	brightness += brightnessAdd * timestep.getSeconds();
	if (brightness < 0.0f) { brightness = 0.0f; brightnessAdd *= -1; }
	if (brightness > 1.0f) { brightness = 1.0f; brightnessAdd *= -1; }
	shader->setFloat4("u_TintColor", glm::vec4(brightness, brightness, brightness, 1.0f));
	rvn::Renderer::beginScene(camera.getCamera());
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
