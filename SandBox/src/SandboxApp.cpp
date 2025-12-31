#include <GameEngine.h>

class ExampleLayer : public GameEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
	{
		m_VertexArray.reset(GameEngine::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.6f, -0.7f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.6f, -0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<GameEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(GameEngine::VertexBuffer::Create(vertices, sizeof(vertices)));
		GameEngine::BufferLayout layout = {
			{ GameEngine::ShaderDataType::Float3, "a_Position" },
			{ GameEngine::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<GameEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(GameEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);


		m_SquareVA.reset(GameEngine::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<GameEngine::VertexBuffer> squareVB;
		squareVB.reset(GameEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ GameEngine::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<GameEngine::IndexBuffer> squareIB;
		squareIB.reset(GameEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
	
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";


		m_Shader.reset(new GameEngine::Shader(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
	
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec4 u_Color;

			in vec3 v_Position;

			void main()
			{
				color = u_Color;
			}
		)";

		m_flatColorShader.reset(new GameEngine::Shader(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
	}

	void OnUpdate(GameEngine::Timestep dt) override
	{
		GameEngine::RenderCommand::SetClearColor({ 0.0f, 0.1f, 0.1f, 1.0f });
		GameEngine::RenderCommand::Clear();

		if (GameEngine::Input::IsKeyPressed(GE_KEY_W)) 
			m_CameraPosition.y += m_CameraMoveSpeed * dt;
		if (GameEngine::Input::IsKeyPressed(GE_KEY_S)) 
			m_CameraPosition.y -= m_CameraMoveSpeed * dt;
		if (GameEngine::Input::IsKeyPressed(GE_KEY_A)) 
			m_CameraPosition.x -= m_CameraMoveSpeed * dt;
		if (GameEngine::Input::IsKeyPressed(GE_KEY_D)) 
			m_CameraPosition.x += m_CameraMoveSpeed * dt;

		if (GameEngine::Input::IsKeyPressed(GE_KEY_E)) 
			m_CameraRotation -= m_CameraRotationSpeed * dt;
		if (GameEngine::Input::IsKeyPressed(GE_KEY_Q)) 
			m_CameraRotation += m_CameraRotationSpeed * dt;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		GameEngine::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);
		for (int y = 0; y < 20; y++)
		{ 
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.105f, y * 0.105f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if ((x + y) % 2 == 0)
					m_flatColorShader->UploadUniformFloat4("u_Color", redColor);
				else
					m_flatColorShader->UploadUniformFloat4("u_Color", blueColor);
				GameEngine::Renderer::Submit(m_flatColorShader, m_SquareVA, transform);
			}
		}
		
		
		GameEngine::Renderer::Submit(m_Shader, m_VertexArray);

		GameEngine::Renderer::EndScene();
	}

	virtual void OnImGuiRender()
	{
		
	}

	void OnEvent(GameEngine::Event& event) override
	{
		
	}
private:
	std::shared_ptr<GameEngine::Shader> m_Shader;
	std::shared_ptr<GameEngine::VertexArray> m_VertexArray;

	std::shared_ptr<GameEngine::Shader> m_flatColorShader;
	std::shared_ptr<GameEngine::VertexArray> m_SquareVA;

	GameEngine::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	const float m_CameraMoveSpeed = 2.0f;

	float m_CameraRotation = 0.0f;
	const float m_CameraRotationSpeed = 50.0f;

	glm::vec3 m_SquarePosition;
	const float m_SquareMoveSpeed = 1.0f;
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