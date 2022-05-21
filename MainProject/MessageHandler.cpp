#include "MessageHandler.h"
#include "ImGui/imgui.h"
using namespace DirectX::SimpleMath;


MessageHandler::MessageHandler(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
}

void MessageHandler::HandleMessage(MSG message)
{
	auto io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		return;
	}
	if (message.message == WM_LBUTTONDOWN)
	{
		WORD x = LOWORD(message.lParam);
		WORD y = HIWORD(message.lParam);
		xWhenDown = x;
		yWhenDown = y;

	}
	if (message.message == WM_LBUTTONUP)
	{
		WORD x = LOWORD(message.lParam);
		WORD y = HIWORD(message.lParam);
		float dx = x - xWhenDown;
		float dy = y - yWhenDown;
		std::vector<std::shared_ptr<IModel>> models;
		if (abs(dx) < 10 && abs(dy) < 10)
		{
			auto model = scene->GetModelFromScreenCoords(x, y);
			if (model)
			{
				models.push_back(model);
			}
			else
			{
				scene->UpdateCursorPositionFromScreenCoords(Vector2(x, y));
				return;
			}
		}
		else
		{
			float left = min(x, xWhenDown);
			float right = max(x, xWhenDown);
			float top = min(y, yWhenDown);
			float bottom = max(y, yWhenDown);
			models = scene->GetModelsFromArea(left, right, top, bottom);
		}

		if (!io.KeyCtrl)
		{
			scene->DeselectAll();
		}
		for (auto model : models)
		{
			scene->ChangeSelection(model);
		}
	}
}
