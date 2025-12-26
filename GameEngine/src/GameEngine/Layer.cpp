#include "gepch.h"
#include "Layer.h"

namespace GameEngine {

	Layer::Layer(const std::string& debugname)
		: m_DebugName(debugname) {}

	Layer::~Layer() = default;

	void Layer::OnAttach() {}
	void Layer::OnDetach() {}
	void Layer::OnUpdate() {}
	void Layer::OnImGuiRender() {}
	void Layer::OnEvent(Event& event) {}

}