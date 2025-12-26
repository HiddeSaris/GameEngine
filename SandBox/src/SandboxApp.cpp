#include <GameEngine.h>

#include "imgui/imgui.h"

class ExampleLayer : public GameEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override
	{
		//GE_INFO("ExampleLayer::Update");

		if (GameEngine::Input::IsKeyPressed(GE_KEY_TAB))
			GE_INFO("Tab Key is being pressed!");
	}

	virtual void OnImGuiRender()
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
	}

	void OnEvent(GameEngine::Event& event) override
	{
		if (event.GetEventType() == GameEngine::EventType::KeyPressed)
		{
			GameEngine::KeyPressedEvent& e = (GameEngine::KeyPressedEvent&)event;
			GE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public GameEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

GameEngine::Application* GameEngine::CreateApplication()
{
	return new Sandbox();
}