#include "MessageHandler.h"
#include "ImGui/imgui.h"


MessageHandler::MessageHandler(std::shared_ptr<Scene> scene)
{
		this->scene = scene;
}

void MessageHandler::HandleMessage(MSG message)
{	auto io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		return;
	}
	if (message.message == WM_LBUTTONDOWN)
	{
		WORD x = LOWORD(message.lParam);
		WORD y = HIWORD(message.lParam);
		scene->ChangeSelectionFromScreenCoords(x, y);
	}
}
