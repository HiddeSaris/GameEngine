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
	GE_PROFILE_FUNCTION();

	m_Texture = GameEngine::Texture2D::Create("assets/textures/picture.png");
}

void Sandbox2D::OnDetach()
{
	GE_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(GameEngine::Timestep dt)
{
	GE_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(dt);

	{
		GE_PROFILE_SCOPE("Renderer Prep");
		GameEngine::RenderCommand::SetClearColor({ 0.0f, 0.1f, 0.1f, 1.0f });
		GameEngine::RenderCommand::Clear();
	}

	{
		GE_PROFILE_SCOPE("Renderer Draw");
		GameEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		GameEngine::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_Rotation, { 0.8f, 0.2f, 0.3f, 1.0f });
		GameEngine::Renderer2D::DrawRotatedQuad({ 0.55f, -0.55f }, { 0.5f, 0.75f }, m_Rotation * -3, m_SquareColor);
		GameEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 30.0f, 30.0f }, m_Texture, { 0.4f, 0.8f, 0.9f, 1.0f }, 10.0f);

		GameEngine::Renderer2D::EndScene();
	}

	m_Rotation += 0.1f;
	
}

void Sandbox2D::OnImGuiRender()
{
	GE_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(GameEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}
