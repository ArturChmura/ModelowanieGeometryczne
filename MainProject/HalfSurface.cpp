#include "HalfSurface.h"

HalfSurface::HalfSurface(std::shared_ptr<IParameterized> surface, bool topLeft, int halfIndex)
{
	this->surface = surface;
	this->topLeft = topLeft;
	this->halfIndex = halfIndex;
}

DirectX::SimpleMath::Vector3 HalfSurface::GetValue(double u, double v)
{
	auto [uSurface, vSurface] = GetSurfaceUV(u, v);
	auto value = surface->GetValue(uSurface, vSurface);
	return value;
}

DirectX::SimpleMath::Vector3 HalfSurface::GetUDerivativeValue(double u, double v)
{
	auto [uSurface, vSurface] = GetSurfaceUV(u, v);
	auto value = surface->GetUDerivativeValue(uSurface, vSurface);
	
	return value;
}

DirectX::SimpleMath::Vector3 HalfSurface::GetVDerivativeValue(double u, double v)
{
	auto [uSurface, vSurface] = GetSurfaceUV(u, v);
	auto value = surface->GetVDerivativeValue(uSurface, vSurface);
	
	return value;
}

bool HalfSurface::IsUWrapped()
{
	return false;
}

bool HalfSurface::IsVWrapped()
{
	return false;
}

void HalfSurface::OnFilterUpdate()
{
}

std::tuple<double, double> HalfSurface::GetSurfaceUV(double u, double v)
{
	if(topLeft)
	{
		u = u / 2 + 0.5 * (1-halfIndex);
		v = v / 2 + 0.5 * halfIndex;
	}
	else
	{
		u = u / 2 * 0.5 * halfIndex;
		v = v / 2 + 0.5 * halfIndex;
	}
	return std::make_tuple(u, v);
}


