#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f)
{
}

void Sandbox2D::OnAttach()
{
	
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(GameEngine::Timestep dt)
{
	m_CameraController.OnUpdate(dt);

	GameEngine::RenderCommand::SetClearColor({ 0.0f, 0.1f, 0.1f, 1.0f });
	GameEngine::RenderCommand::Clear();

	GameEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	GameEngine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_Rotation, { 0.8f, 0.2f, 0.3f, 1.0f });
	GameEngine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_Rotation, { 0.2f, 0.3f, 0.8f, 1.0f });

	GameEngine::Renderer2D::EndScene();

	GE_TRACE(m_Rotation);

	m_Rotation += 1.0f;
	
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(GameEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}
