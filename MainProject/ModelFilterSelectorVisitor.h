#pragma once
#include <vector>
#include <memory>
#include "EmptyModelVisitor.h"
#include "IModel.h"
template<class T>
class ModelFilterSelectorVisitor : public EmptyModelVisitor
{
public:
	std::vector<std::shared_ptr<IModel>> GetList(std::vector<std::shared_ptr<IModel>> models);

	virtual void Accept(std::shared_ptr<T> point) override;
private:

	std::vector<std::shared_ptr<IModel>> list;
	bool isModel;
};

template<class T>
inline std::vector<std::shared_ptr<IModel>> ModelFilterSelectorVisitor<T>::GetList(std::vector<std::shared_ptr<IModel>> models)
{
	list.clear();
	for (auto model : models)
	{
		isModel = false;
		model->Accept(*this);
		if (!isModel)
		{
			list.push_back(model);
		}
	}
	return list;
}

template<class T>
inline void ModelFilterSelectorVisitor<T>::Accept(std::shared_ptr<T> model)
{
	isModel = true;
}
