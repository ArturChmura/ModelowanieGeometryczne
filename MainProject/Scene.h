#pragma once
#include <vector>
#include "Camera.h"
#include <memory>

using namespace std;
class Scene
{
public:
	void AddCamera(shared_ptr<Camera> camera);
	void SetActiveCamera(shared_ptr<Camera> camera);
private:
	vector<shared_ptr<Camera>> cameras; 
	shared_ptr<Camera> activeCamera;
};