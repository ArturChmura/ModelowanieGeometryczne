#include "MouseEvents.h"
#include "ImGui/imgui.h"

MouseEvents::MouseEvents(std::shared_ptr<ArcCameraModel> camera)
{
	this->camera = camera;
}

void MouseEvents::HandleMouse()
{
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::IsMouseDown(0))
	{
		float x = io.MouseDelta.x;
		float y = io.MouseDelta.y;
		if (x != 0 || y != 0)
		{
			camera->Rotate(x * -rotationSpeedPerPixel, y * -rotationSpeedPerPixel);
		}
	}

	//case WM_LBUTTONDOWN:
	//case WM_RBUTTONDOWN:
	//{
	//	lastMousePosition = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };
	//	break;
	//}
	//case WM_MOUSEMOVE:
	//{
	//	POINT diff;
	//	if (msg.wParam & MK_LBUTTON || msg.wParam & MK_RBUTTON)
	//	{
	//		POINT point{ GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };
	//		diff = { point.x - lastMousePosition.x, point.y - lastMousePosition.y };
	//		lastMousePosition = point;
	//	}
	//	if (msg.wParam & MK_LBUTTON)
	//	{
	//		camera->Rotate(diff.x * -rotationSpeedPerPixel, diff.y * -rotationSpeedPerPixel);
	//	}
	//	else if (msg.wParam & MK_RBUTTON)
	//	{
	//		/*distance += distancePerPixel * diff;
	//		distance = 0 > distance ? 0 : distance;
	//		distance = 50 < distance ? 50 : distance;
	//		XMStoreFloat4x4(&m_viewMtx, XMLoadFloat4x4(&m_viewMtxWithoutTranslation) * XMMatrixTranslation(0, 0, distance));*/
	//	}
	//	break;
}
