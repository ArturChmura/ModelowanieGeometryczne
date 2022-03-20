#pragma once

class RenderContext
{
public:

	RenderContext() {}

	virtual void pre_render() = 0;

	virtual void post_render() = 0;

	virtual void end() = 0;

};