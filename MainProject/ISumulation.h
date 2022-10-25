#pragma once

class ISimulation
{
public:
	virtual void Update(float dt) = 0;
	virtual void Stop() = 0;
	virtual void Start() = 0;
	virtual bool IsRunning() = 0;
};