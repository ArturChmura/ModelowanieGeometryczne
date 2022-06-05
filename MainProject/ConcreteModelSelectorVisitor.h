#pragma once
#include <vector>
#include <memory>
#include "EmptyModelVisitor.h"
#include "IModel.h"
template<class T>
class ConcreteModelSelectorVisitor : public EmptyModelVisitor
{
public:
	std::vector<std::shared_ptr<T>> GetList(std::vector<std::shared_ptr<IModel>> models);

	virtual void Accept(std::shared_ptr<T> point) override;
private:

	std::vector<std::shared_ptr<T>> list;
};

template<class T>
inline std::vector<std::shared_ptr<T>> ConcreteModelSelectorVisitor<T>::GetList(std::vector<std::shared_ptr<IModel>> models)
{
	list.clear();
	for (auto model : models)
	{
		model->Accept(*this);
	}
	return list;
}

template<class T>
inline void ConcreteModelSelectorVisitor<T>::Accept(std::shared_ptr<T> model)
{
	list.push_back(model);
}
