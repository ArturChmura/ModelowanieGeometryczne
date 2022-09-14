#pragma once
#include "../MainProject/IParameterized.h"

using namespace DirectX::SimpleMath;


class ParametrizedMock1 : public IParameterized
{
public:
	// Inherited via IParameterized
	virtual DirectX::SimpleMath::Vector3 GetValue(double u, double v) override
	{
		double x = 40 *u;
		double y = 40 * v;
		double z = 20; 
		return Vector3(x, y, z);
	}
	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(double u, double v) override
	{
		double x = 40;
		double y = 0;
		double z = 0;
		return Vector3(x, y, z);
	}
	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(double u, double v) override
	{
		double x = 0;
		double y = 40;
		double z = 0;
		return Vector3(x, y, z);
	}
	virtual bool IsUWrapped() override
	{
		return false;
	}
	virtual bool IsVWrapped() override
	{
		return false;
	}
};

class ParametrizedMock2 : public IParameterized
{
public:
	// Inherited via IParameterized
	virtual DirectX::SimpleMath::Vector3 GetValue(double u, double v) override
	{
		double x = 2.5 * cos(v) * cos(u) - 2.5 * cos(v) * sin(u) - 3.535 * sin(v) + 0.429 * cos(u) + 7.5 * sin(u);
		double y = 2.5 * cos(v) * cos(u) - 2.5 * cos(v) * sin(u) + 3.535 * sin(v) + 0.429 * cos(u) + 7.5 * sin(u);
		double z = -3.535 * cos(v) * sin(u) - 3.5335 * cos(v) * cos(u) - 0.606 * sin(u) + 10.607 * cos(u) + 20;
		return Vector3(x, y, z);
	}
	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(double u, double v) override
	{
		double x = cos(u) * (7.5 - 2.5 * cos(v)) + (-0.429 - 2.5 * cos(v)) * sin(u);
		double y = cos(u) * (7.5 - 2.5 * cos(v)) + (-0.429 - 2.5 * cos(v)) * sin(u);
		double z = cos(u) * (-0.606 - 3.535 * cos(v)) + (-10.607 + 3.5335 * cos(v)) * sin(u);
		return Vector3(x, y, z);
	}
	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(double u, double v) override
	{
		double x = -3.535 * cos(v) + (-2.5 * cos(u) + 2.5 * sin(u)) * sin(v);
		double y = 3.535 * cos(v) + (-2.5 * cos(u) + 2.5 * sin(u)) * sin(v);
		double z = (3.5335 * cos(u) + 3.535 * sin(u)) * sin(v);
		return Vector3(x, y, z);
	}
	virtual bool IsUWrapped() override
	{
		return false;
	}
	virtual bool IsVWrapped() override
	{
		return false;
	}
};


class ParametrizedMock21 : public IParameterized
{
public:
	// Inherited via IParameterized
	virtual DirectX::SimpleMath::Vector3 GetValue(double u, double v) override
	{
		double x = 2+u*sin(v);
		double y = 3+u*cos(v);
		double z = -2+v/3.0;
		return Vector3(x, y, z);
	}
	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(double u, double v) override
	{
		double x = sin(v);
		double y = cos(v);
		double z = 0;
		return Vector3(x, y, z);
	}
	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(double u, double v) override
	{
		double x = u*cos(v);
		double y = -u*sin(v);
		double z = 1.0/3.0;
		return Vector3(x, y, z);
	}
	virtual bool IsUWrapped() override
	{
		return false;
	}
	virtual bool IsVWrapped() override
	{
		return false;
	}
};


class ParametrizedMock22 : public IParameterized
{
public:
	// Inherited via IParameterized
	virtual DirectX::SimpleMath::Vector3 GetValue(double u, double v) override
	{
		double x = 2*u;
		double y = v*v;
		double z = sin(3.0*v);
		return Vector3(x, y, z);
	}
	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(double u, double v) override
	{
		double x = 2;
		double y = 0;
		double z = 0;
		return Vector3(x, y, z);
	}
	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(double u, double v) override
	{
		double x = 0;
		double y = 2.0*v;
		double z = 3.0*cos(3.0*v);
		return Vector3(x, y, z);
	}
	virtual bool IsUWrapped() override
	{
		return false;
	}
	virtual bool IsVWrapped() override
	{
		return false;
	}
};