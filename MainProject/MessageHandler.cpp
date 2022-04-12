#include "MessageHandler.h"
#include "ImGui/imgui.h"
using namespace DirectX::SimpleMath;


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
		auto model = scene->GetModelFromScreenCoords(x, y);
		if (model)
		{
			if (io.KeyCtrl)
			{
				scene->ChangeSelection(model);
			}
			else
			{
				scene->Select(model);
			}
		}
		else
		{
			scene->UpdateCursorPositionFromScreenCoords(Vector2(x, y));
		}
	}
}
