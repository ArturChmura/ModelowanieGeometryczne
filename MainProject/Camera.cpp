#include "Camera.h"

Camera::Camera(std::shared_ptr<ICameraMovement> cameraMovement, std::string name)
	:Named(name)
{
	this->cameraMovement = cameraMovement;
}
