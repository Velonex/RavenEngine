#include "TestLayer.h"
#include <string>

struct Vertex {
	float position[3];
	float color[4];
};

void TestLayer::onAttach()
{
	glViewport(0, 0, 1280, 720);

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
	const GLchar* vertexShaderSource =
R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
	v_Color = a_Color;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

)";

	const GLchar* fragmentShaderSource =
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

	camera = rvn::createRef<rvn::OrthographicCamera>(-(16.f / 9.f), 16.f / 9.f, -1, 1);
}

void TestLayer::onUpdate(rvn::Timestep timestep)
{
	camera->setPosition({ -0.5f, 0.0f, 0.0f });
	camera->setRotation(45.f);
	shader->bind();
	brightness += brightnessAdd * timestep.getSeconds();
	if (brightness < 0.0f) { brightness = 0.0f; brightnessAdd *= -1; }
	if (brightness > 1.0f) { brightness = 1.0f; brightnessAdd *= -1; }
	shader->setFloat4("u_TintColor", glm::vec4(brightness, brightness, brightness, 1.0f));
	shader->setMat4("u_ViewProjectionMatrix", camera->getViewProjectionMatrix());
	glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void TestLayer::onDetach()
{
}

void TestLayer::onImGuiRender()
{
}

void TestLayer::onEvent(rvn::Event* e)
{
}
