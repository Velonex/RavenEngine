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
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	// Vertex positions
	Vertex vertices[4] = {
		{-0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f},
		{-0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f},
		{ 0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f},
		{ 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f}
	};

	// Create and bind vertex buffer
	glGenBuffers(1, &vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set Layout
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)sizeof(Vertex::position));

	// Indices
	unsigned int indices[2 * 3] = {
		0, 1, 2,
		2, 3, 0
	};

	// Create and bind index buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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
