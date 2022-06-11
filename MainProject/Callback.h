#pragma once

#include <vector>
#include <functional>
#include <string>

template<typename T>
class Callback
{
public:
	std::vector<std::function<T>> functions;
	std::vector<std::string> pointIds;

	void Add(std::function<T> function, std::string id);
	void Add(std::function<T> function, int id);

	void Add(Callback<T> callback);

	void Remove(std::string id);
	void Remove(int id);
	void Clear();
};

template<typename T>
inline void Callback<T>::Add(std::function<T> function, std::string id)
{
	functions.push_back(function);
	pointIds.push_back(id);
}

template<typename T>
inline void Callback<T>::Add(std::function<T> function, int id)
{
	Add(function, std::to_string(id));
}

template<typename T>
inline void Callback<T>::Add(Callback<T> callback)
{
	for (size_t i = 0; i < callback.pointIds.size(); i++)
	{
		Add(callback.functions[i], callback.pointIds[i]);
	}
}

template<typename T>
inline void Callback<T>::Remove(std::string id)
{
	while (true)
	{
		auto it = find(pointIds.begin(), pointIds.end(), id);

		if (it != pointIds.end())
		{
			int index = it - pointIds.begin();
			pointIds.erase(pointIds.begin() + index);
			functions.erase(functions.begin() + index);
		}
		else
		{
			break;
		}
	}

}
template<typename T>
inline void Callback<T>::Remove(int id)
{
	Remove(std::to_string(id));
}

template<typename T>
inline void Callback<T>::Clear()
{
	functions.clear();
	pointIds.clear();
}
