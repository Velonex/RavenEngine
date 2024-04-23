#include <pch.h>
#include "Renderer.h"


namespace rvn {

	scope<Renderer::SceneData> Renderer::s_sceneData = createScope<Renderer::SceneData>();

	void Renderer::init()
	{
		RenderCommand::init();
	}
	void Renderer::shutdown()
	{
	}
	void Renderer::beginScene(const OrthographicCamera& camera)
	{
		s_sceneData->_viewProjectionMatrix = camera.getViewProjectionMatrix();
	}
	void Renderer::beginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_sceneData->_viewProjectionMatrix = camera.getProjection() * glm::inverse(transform);
	}
	void Renderer::endScene()
	{
	}
	// TODO: add functions that don't rebind the shader every time and re-set the vp matrix
	void Renderer::draw(const ref<VertexArray>& vertexArray, const ref<Shader>& shader, const glm::mat4& transform)
	{
		shader->bind();
		vertexArray->bind();
		shader->setMat4("u_ViewProjectionMatrix", s_sceneData->_viewProjectionMatrix);
		shader->setMat4("u_Transform", transform);
		RenderCommand::draw(vertexArray);
	}
	void Renderer::draw(const ref<Mesh>& mesh, const ref<Shader>& shader, const glm::mat4& transform)
	{
		shader->bind();
		mesh->getVertexArray()->bind();
		shader->setMat4("u_ViewProjectionMatrix", s_sceneData->_viewProjectionMatrix);
		shader->setMat4("u_Transform", transform);
		RenderCommand::draw(mesh->getVertexArray());
	}
	void Renderer::onWindowResize(std::uint32_t width, std::uint32_t height)
	{
		RenderCommand::setViewport(0, 0, width, height);
	}
}