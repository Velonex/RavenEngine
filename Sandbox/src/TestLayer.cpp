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

void main()
{
	v_Color = a_Color;
	gl_Position = vec4(a_Position, 1.0);
}

)";

	const GLchar* fragmentShaderSource =
R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
	color = v_Color;
}
)";

	std::uint32_t vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(shader, infoLogLength, NULL, &ProgramErrorMessage[0]);
		LOG_ENGINE_ERROR("{}", &ProgramErrorMessage[0]);
	}
	ASSERT(result, "Shader linking error");
	glDetachShader(shader, vertexShader);
	glDetachShader(shader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void TestLayer::onUpdate(rvn::Timestep timestep)
{
	glUseProgram(shader);
	vao->bind();
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
