#pragma once

#include "OrthographicCamera.h"

namespace GameEngine {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	};

}