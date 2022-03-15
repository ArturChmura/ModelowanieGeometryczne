#pragma once
#include <vector>
#include "Camera.h"
#include <memory>
#include "DxVertexModelDrawer.h"

using namespace std;
class Scene
{
public:
	void AddCamera(shared_ptr<Camera> camera);
	void SetActiveCamera(shared_ptr<Camera> camera);
	void AddModel(shared_ptr<DxVertexModelDrawer> model)
	{
		models.push_back(model);
	}
	vector<shared_ptr<Camera>> cameras;
	shared_ptr<Camera> activeCamera;

	vector<shared_ptr<DxVertexModelDrawer>> models;
private:
};